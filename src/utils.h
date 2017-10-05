#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <filesystem>
#include <sstream>
#include "settings.h"

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

    inline std::string GetLevelsDirectory() {
        return GetDataDirectory() + "\\levels";
    }

    inline std::string GetEnemiesDirectory() {
        return GetDataDirectory() + "\\enemies";
    }

    inline std::string GetTowersDirectory() {
        return GetDataDirectory() + "\\towers";
    }

    inline bool CirclesColliding(const float& l_radius, const sf::Vector2f& l_pos, const float& l_radius2, const sf::Vector2f& l_pos2){
        sf::Vector2f dist(l_pos2 - l_pos);
        return (dist.x * dist.x + dist.y * dist.y <= (l_radius + l_radius2) * (l_radius + l_radius2));
    }

    inline bool PointInsideCircle(const sf::Vector2i& l_point, const sf::Vector2f& l_pos, const float& l_radius){
        return pow(l_point.x - l_pos.x, 2) + pow(l_point.y - l_pos.y, 2) <= pow(l_radius, 2);
    }

    template<class T>
    const T& clamp( const T& v, const T& lo, const T& hi )
    {
        if(v < lo) return lo;
        if(hi < v) return hi;
        return v;
    }

    inline bool CircleAABBColliding(const sf::Vector2f& l_circlePos, const float& l_radius, const sf::FloatRect& l_rect){
        const float closestX = clamp(l_circlePos.x, l_rect.left, l_rect.left + l_rect.width);
        const float closestY = clamp(l_circlePos.y, l_rect.top, l_rect.top + l_rect.height);

        // Calculate the distance between the circle's center and this closest point
        const float distanceX = l_circlePos.x - closestX;
        const float distanceY = l_circlePos.y - closestY;

        // If the distance is less than the circle's radius, an intersection occurs
        const float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
        return distanceSquared < (l_radius * l_radius);
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
            } else if(text == "%GITHUB%") {
                l_text.replace(start, length, l_settings->GetGithub());
            }
            else continue;
            end = 1;
        }
    }

}

#endif // UTILS_H
