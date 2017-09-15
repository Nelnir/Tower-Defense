#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
    const std::string m_gameTitle = "Future Defense";
    const std::string m_version = "1.0";
    const std::string m_releaseDate = "15.09.2017";
    const std::string m_license = "GNU General Public License";
public:
    Settings();
    std::string GetTitle() { return m_gameTitle; }
    std::string GetVersion() { return m_version; }
    std::string GetReleaseDate() { return m_releaseDate; }
    std::string GetLicense() { return m_license; }
};

#endif // SETTINGS_H
