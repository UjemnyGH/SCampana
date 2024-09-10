#pragma once
#ifndef __INI_LOADER_
#define __INI_LOADER_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>


const std::string __ini_extension = ".cfg";

struct Vars {
    std::string name;
    std::string value;

    Vars() = default;
};

struct Section {
    std::string name;
    std::map<std::string, Vars> vars;

    Section() = default;
};

class IniLoader {
private:
    const std::string comment = "; That is comment described in IniLoader class!\n; Have fun! Piotr UjemnyGH Plombon\n";

public:
    std::map<std::string, Section> sections;

    std::string ini_file_data;
    std::string file_path;

    void add_section(std::string const name) {
        sections.insert(std::make_pair(name, Section()));

        sections[name].name = name;
    }

    void add_var_to_section(std::string const section_name, std::string const var_name, std::string const value_str) {
        sections[section_name].vars.insert(std::make_pair(var_name, Vars()));

        sections[section_name].vars[var_name].name = var_name;
        sections[section_name].vars[var_name].value = value_str;
    }

    void save_ini(std::string const file_name) {
        ini_file_data = comment;
        ini_file_data += "; ";
        ini_file_data += file_name + __ini_extension;
        ini_file_data += "\n\n";

        for(auto section : sections) {
            ini_file_data += "[";
            ini_file_data += section.second.name;
            ini_file_data += "]\n";

            for(auto var : section.second.vars) {
                ini_file_data += var.second.name;
                ini_file_data += " = ";
                ini_file_data += var.second.value;
                ini_file_data += "\n";
            }

            ini_file_data += "\n";
        }

        std::ofstream f;

        file_path = file_name;

        f.open(file_path + __ini_extension, std::ios_base::binary);

        f.write(ini_file_data.data(), ini_file_data.size());

        f.close();
    }

    bool load_ini(std::string const file_name) {
        sections.clear();

        std::ifstream f;

        file_path = file_name;

        f.open(file_path + __ini_extension, std::ios_base::binary);

        if(f.bad()) {
            f.close();
            
            return false;
        }

        if(!f.is_open()) {
            f.close();
            
            return false;
        }

        std::string line;

        std::string section_name;
        std::string var_name;
        std::string var_value;

        while(!f.eof()) {
            std::getline(f, line);

            if(line.find("[") == 0) {
                section_name.clear();
                std::copy(line.begin() + 1, line.end() - 1, std::back_inserter(section_name));

                sections.insert(std::make_pair(section_name, Section()));
                sections[section_name].name = section_name;
            }
            else if(line.find(";") == 0 || line.find("#") == 0 || line.length() < 2) {
                continue;
            }
            else {
                var_name.clear();
                var_value.clear();

                uint32_t equal_char = line.find("=");

                std::copy(line.begin(), line.begin() + (equal_char - 1), std::back_inserter(var_name));
                std::copy(line.begin() + (equal_char + 2), line.end(), std::back_inserter(var_value));

                sections[section_name].vars.insert(std::make_pair(var_name, Vars()));

                sections[section_name].vars[var_name].name = var_name;
                sections[section_name].vars[var_name].value = var_value;
            }
        }

        f.close();

        return true;
    }
};


#endif