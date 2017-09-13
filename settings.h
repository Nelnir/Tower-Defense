#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
    const std::string m_gameTitle = "Future Defense";
    const std::string m_version = "1.0";
public:
    Settings();
    std::string GetTitle() { return m_gameTitle; }
    std::string GetVersion() { return m_version; }
};

#endif // SETTINGS_H
