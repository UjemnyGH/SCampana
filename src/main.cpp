#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_stdlib.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "ini_loader.hpp"
#include "core.hpp"

#include "popups.hpp"

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SCampana", nullptr, nullptr);

    if(!window) {
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(SP_OPENGL_VERSION);

    if(!gConfigFile.load_ini(SP_CONFIG_FILE)) {
        gConfigFile.add_section(SP_LAST_CONFIG_SECTION);
        gConfigFile.add_var_to_section(SP_LAST_CONFIG_SECTION, SP_LESSONS_AMOUNT_VAR, "3");
        gConfigFile.add_var_to_section(SP_LAST_CONFIG_SECTION, SP_CURRENT_CONFIG_VAR, "");
        gConfigFile.save_ini(SP_CONFIG_FILE);
    }

    gCurrentLoadedConfiguration = gConfigFile.sections[SP_LAST_CONFIG_SECTION].vars[SP_CURRENT_CONFIG_VAR].value;

    if(!gCurrentLoadedConfiguration.empty()) {
        gLessonLength = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_LESSON_LENGTH_VAR_NAME].value.c_str());
        gPauseLengths[0] = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_PAUSE_1_LENGTH_VAR_NAME].value.c_str());
        gPauseLengths[1] = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_PAUSE_2_LENGTH_VAR_NAME].value.c_str());
        gPauseLengths[2] = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_PAUSE_3_LENGTH_VAR_NAME].value.c_str());
        gFirstLesson[0] = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_FIRST_LESSON_START_HOUR_VAR_NAME].value.c_str());
        gFirstLesson[1] = atoi(gConfigFile.sections[gCurrentLoadedConfiguration].vars[SP_FIRST_LESSON_START_MINUTE_VAR_NAME].value.c_str());
    }

    int temp = 5;

    while(!glfwWindowShouldClose(window)) {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::Begin("##main_window", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {
            if(ImGui::BeginMenuBar()) {
                if(ImGui::BeginMenu("Konfiguracje")) {
                    if(ImGui::MenuItem("Dodaj")) gPopupAddConfigOpen = true;
                    if(ImGui::MenuItem("Zaladuj")) gPopupLoadConfigOpen = true;
                    if(ImGui::MenuItem("Usun")) gPopupDeleteConfigOpen = true;

                    ImGui::EndMenu();
                }
                
                ImGui::EndMenuBar();
            }

            ImGui::Text("Zaladowana konfiguracja: %s", gCurrentLoadedConfiguration.c_str());

            for(uint8_t i = 0; i < atoi(gConfigFile.sections[SP_LAST_CONFIG_SECTION].vars[SP_LESSONS_AMOUNT_VAR].value.c_str()); i++) {
                ImGui::InputInt("Czas", &temp);
            }
            
            ImGui::End();
        }

        if(gPopupAddConfigOpen) { 
            ImGui::OpenPopup(SP_ADD_CONFIG_ID); 
            gPopupAddConfigOpen = false;
        }
        
        PopupAddConfig();

        if(gPopupLoadConfigOpen) { 
            ImGui::OpenPopup(SP_LOAD_CONFIG_ID); 
            gPopupLoadConfigOpen = false;
        }

        PopupLoadConfig();

        if(gPopupDeleteConfigOpen) { 
            ImGui::OpenPopup(SP_DELETE_CONFIG_ID); 
            gPopupDeleteConfigOpen = false;
        }

        PopupDeleteConfig();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}