#ifndef SHAREDCONTEXT_H
#define SHAREDCONTEXT_H

class StateManager;
class Window;
class EventManager;
class TextureManager;
class FontManager;
class GUI_Manager;

struct SharedContext{
    SharedContext():m_wind(nullptr),m_eventManager(nullptr), m_textureManager(nullptr), m_fontManager(nullptr), m_guiManager(nullptr){}
    Window* m_wind;
    EventManager* m_eventManager;
    TextureManager* m_textureManager;
    FontManager* m_fontManager;
    GUI_Manager* m_guiManager;
};

#endif // SHAREDCONTEXT_H
