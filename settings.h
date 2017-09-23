#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <SFML/Graphics.hpp>

class Settings
{
    const std::string m_gameTitle = "Future Defense";
    const std::string m_version = "1.0";
    const std::string m_releaseDate = "19.09.2017";
    const std::string m_license = "GNU General Public License";
    const std::string m_creator = "Marcin Dzieciatkowski";
    const sf::Vector2u m_resolution;
    const int m_numberOfLevels = 6;
    int m_currentLevel;
    const std::string m_currency;
public:
    Settings();
    std::string GetTitle() { return m_gameTitle; }
    std::string GetVersion() { return m_version; }
    std::string GetReleaseDate() { return m_releaseDate; }
    std::string GetLicense() { return m_license; }
    std::string GetCreator() { return m_creator; }
    sf::Vector2u GetResolution() { return m_resolution; }
    int GetNumberOfLevels() { return m_numberOfLevels; }
    std::string GetCurrency() { return m_currency; }

    int GetCurrentLevel() { return m_currentLevel; }
    void SetCurrentLevel(const int& l_level) { m_currentLevel = l_level; }
};

#endif // SETTINGS_H
