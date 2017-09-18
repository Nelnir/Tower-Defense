#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <filesystem>
#include <sstream>
#include "settings.h"
#include <iostream>
namespace Utils
{
    inline std::string GetWorkingDirectory() {
        return std::experimental::filesystem::current_path().string();
    }

    inline std::string GetDataDirectory() {
        return GetWorkingDirectory() + "\\data";
    }

    inline std::string GetTexturesDirectory() {
        return GetDataDirectory() + "\\textures";
    }

    inline std::string GetStylesDirectory() {
        return GetDataDirectory() + "\\styles";
    }

    inline std::string GetInterfacesDirectory() {
        return GetDataDirectory() + "\\interfaces";
    }

    inline void ReadQuotedString(std::stringstream& l_stream, std::string& l_string) {
        l_stream >> l_string;
        if (l_string.at(0) == '"'){
            while (l_string.back() != '"' || !l_stream.eof()){
                std::string str;
                l_stream >> str;
                l_string.append(" " + str);
            }
        }
        l_string.erase(std::remove(l_string.begin(), l_string.end(), '"'), l_string.end());
    }

    inline void ReplaceText(std::string& l_text, Settings* l_settings){
        if(l_text.empty()){
            return;
        }

        size_t start = 0, end = -1;
        while(true){
            start = l_text.find('%', end + 1);
            if(start == std::string::npos){
                return;
            }
            end = l_text.find('%', start + 1);
            if(end == std::string::npos){
                return;
            }
            size_t length = end - start + 1;
            std::string text = l_text.substr(start, length);
            if(text == "%VERSION%")
                l_text.replace(start, length , l_settings->GetVersion());
            else if(text == "%RELEASE_DATE%")
                l_text.replace(start, length, l_settings->GetReleaseDate());
            else if(text == "%LICENSE%")
                l_text.replace(start, length, l_settings->GetLicense());
            else if(text == "%CREATOR%") {
                l_text.replace(start, length, l_settings->GetCreator());
            }
            else continue;
            end = 1;
        }
    }

}

#endif // UTILS_H
