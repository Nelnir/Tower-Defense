#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <filesystem>
#include <sstream>

namespace Utils
{
    inline std::string GetWorkingDirectory() {
        return std::experimental::filesystem::current_path().string();
    }

    inline std::string GetTexturesDirectory() {
        return std::experimental::filesystem::current_path().string() + "\\data\\textures\\";
    }

    inline std::string GetDataDirectory() {
        return std::experimental::filesystem::current_path().string() + "\\data\\";
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

}

#endif // UTILS_H
