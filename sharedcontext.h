#ifndef SHAREDCONTEXT_H
#define SHAREDCONTEXT_H

class StateManager;
class Window;
class EventManager;
class TextureManager;
class FontManager;
class GUI_Manager;
class Settings;
class Level;

struct SharedContext{
    SharedContext():m_wind(nullptr),m_eventManager(nullptr), m_textureManager(nullptr), m_fontManager(nullptr), m_guiManager(nullptr){}
    Window* m_wind;
    EventManager* m_eventManager;
    TextureManager* m_textureManager;
    FontManager* m_fontManager;
    GUI_Manager* m_guiManager;
    Settings* m_settings;
    Level* m_level;
};

#endif // SHAREDCONTEXT_H
