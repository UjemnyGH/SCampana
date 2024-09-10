#pragma once
#ifndef __POPUPS_
#define __POPUPS_

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "core.hpp"
#include <string>
#include <vector>

int gLessonLength = 45;
int gPauseLengths[3] = {5, 10, 15};
int gFirstLesson[2] = {7, 15};
std::string gConfigName;
int gCurrentLoadedItem = 0;
std::string gCurrentLoadedConfiguration;

bool gPopupAddConfigOpen = false;
bool gPopupLoadConfigOpen = false;
bool gPopupDeleteConfigOpen = false;

void PopupAddConfig() {
    if(gPopupAddConfigOpen){
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal(SP_ADD_CONFIG_ID, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Czas podawany jest w minutach");

        ImGui::InputTextWithHint("Nazwa konfiguracji", "Nazwa konfiguracji", &gConfigName);
        ImGui::InputInt("Dlugosc lekcji", &gLessonLength);
        ImGui::InputInt("Dlugosc przerwy 1", &gPauseLengths[0]);
        ImGui::InputInt("Dlugosc przerwy 2", &gPauseLengths[1]);
        ImGui::InputInt("Dlugosc przerwy 3", &gPauseLengths[2]);
        ImGui::InputInt2("Rozpoczecie pierwszej lekcji (format h:m)", gFirstLesson);

        if(ImGui::Button("Zapisz")) {
            gConfigFile.add_section(gConfigName);
            gConfigFile.add_var_to_section(gConfigName, SP_LESSON_LENGTH_VAR_NAME, std::to_string(gLessonLength));
            gConfigFile.add_var_to_section(gConfigName, SP_PAUSE_1_LENGTH_VAR_NAME, std::to_string(gPauseLengths[0]));
            gConfigFile.add_var_to_section(gConfigName, SP_PAUSE_2_LENGTH_VAR_NAME, std::to_string(gPauseLengths[1]));
            gConfigFile.add_var_to_section(gConfigName, SP_PAUSE_3_LENGTH_VAR_NAME, std::to_string(gPauseLengths[2]));
            gConfigFile.add_var_to_section(gConfigName, SP_FIRST_LESSON_START_HOUR_VAR_NAME, std::to_string(gFirstLesson[0]));
            gConfigFile.add_var_to_section(gConfigName, SP_FIRST_LESSON_START_MINUTE_VAR_NAME, std::to_string(gFirstLesson[1]));
            gConfigFile.save_ini(SP_CONFIG_FILE);
            gConfigFile.load_ini(SP_CONFIG_FILE);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if(ImGui::Button("Anuluj")) {
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }
}

void PopupLoadConfig() {
    if(gPopupLoadConfigOpen){
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal(SP_LOAD_CONFIG_ID, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Zaladuj konfiguracje");

        std::vector<const char*> loaded_configs;

        for(auto & sect : gConfigFile.sections) {
            if(sect.first == SP_LAST_CONFIG_SECTION) continue;
            loaded_configs.push_back(sect.first.c_str());
        }

        ImGui::ListBox("##loaded_configs", &gCurrentLoadedItem, loaded_configs.data(), loaded_configs.size());

        if(ImGui::Button("Zaladuj")) {
            gCurrentLoadedConfiguration = loaded_configs[gCurrentLoadedItem];
            
            gConfigFile.sections[SP_LAST_CONFIG_SECTION].vars[SP_CURRENT_CONFIG_VAR].value = gCurrentLoadedConfiguration;
            gConfigFile.save_ini(SP_CONFIG_FILE);
            gLessonLength = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_LESSON_LENGTH_VAR_NAME].value.c_str());
            gPauseLengths[0] = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_PAUSE_1_LENGTH_VAR_NAME].value.c_str());
            gPauseLengths[1] = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_PAUSE_2_LENGTH_VAR_NAME].value.c_str());
            gPauseLengths[2] = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_PAUSE_3_LENGTH_VAR_NAME].value.c_str());
            gFirstLesson[0] = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_FIRST_LESSON_START_HOUR_VAR_NAME].value.c_str());
            gFirstLesson[1] = atoi(gConfigFile.sections[loaded_configs[gCurrentLoadedItem]].vars[SP_FIRST_LESSON_START_MINUTE_VAR_NAME].value.c_str());

            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if(ImGui::Button("Anuluj")) {
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }
}

void PopupDeleteConfig() {
    if(gPopupDeleteConfigOpen){
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal(SP_DELETE_CONFIG_ID, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Usun konfiguracje");

        std::vector<const char*> delete_configs;

        for(auto & sect : gConfigFile.sections) {
            if(sect.first == SP_LAST_CONFIG_SECTION) continue;
            delete_configs.push_back(sect.first.c_str());
        }

        ImGui::ListBox("##delete_configs", &gCurrentLoadedItem, delete_configs.data(), delete_configs.size());

        if(ImGui::Button("Usun")) {
            if(gCurrentLoadedConfiguration == delete_configs[gCurrentLoadedItem]) gCurrentLoadedConfiguration = "";
            if(gConfigFile.sections[SP_LAST_CONFIG_SECTION].vars[SP_CURRENT_CONFIG_VAR].value == delete_configs[gCurrentLoadedItem]) gConfigFile.sections[SP_LAST_CONFIG_SECTION].vars[SP_CURRENT_CONFIG_VAR].value = "";
            gConfigFile.sections.erase(delete_configs[gCurrentLoadedItem]);
            gConfigFile.save_ini(SP_CONFIG_FILE);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if(ImGui::Button("Anuluj")) {
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }
}

#endif