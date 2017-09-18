#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "statemanager.h"
#include "texturemanager.h"
#include "fontmanager.h"
#include "gui_manager.h"
#include "sharedcontext.h"
#include "settings.h"

class Game
{
public:
    Game();
    ~Game();
   // void HandleInput();
    void Update();
    void Render();
    inline Window* GetWindow() { return &m_window; }
  //  inline sf::Time getElapsed() { return m_elapsed; }
    //inline void restartClock() { elapsed += m_clock.restart().asSeconds(); }
    void RestartClock();
    void LateUpdate();
private:
    Settings m_settings;

    Window m_window;
    sf::Clock m_clock;
    sf::Time m_elapsed;

    SharedContext m_context;
    TextureManager m_textureManager;
    FontManager m_fontManager;
    GUI_Manager m_guiManager;
    StateManager m_stateManager;
};

#endif // GAME_H
