#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "eventmanager.h"

class Window
{
public:
    Window(const std::string& l_title = "Window", const sf::Vector2u& l_size = sf::Vector2u(640, 480), const bool& l_resizable = true);
    ~Window();

    void BeginDraw(); // Clear the window.
    void EndDraw(); // Display the changes.
    void Update();

    inline bool IsDone() { return m_isDone; }
    inline bool IsFullscreen() { return m_isFullscreen; }
    inline sf::Vector2u GetWindowSize() { return m_windowSize; }
    inline std::string GetWindowTitle() { return m_windowTitle; }

    inline bool isFocused() { return m_isFocused; }
    EventManager* GetEventManager() { return &m_eventManager; }
    void ToggleFullscreen(EventDetails* l_details);
    void Close(EventDetails* l_details = nullptr) { m_isDone = true; }

    void Draw(sf::Drawable& l_drawable);
    sf::RenderWindow * getRenderWindow() { return &m_window; }

    sf::FloatRect GetViewSpace();
    void UpdateResolution(sf::Vector2u& l_size);
private:
    void Setup(const std::string& l_title, const sf::Vector2u& l_size, const bool& l_resizable);
    void Destroy();
    void Create();
    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize;
    std::string m_windowTitle;
    bool m_isDone;
    bool m_isFullscreen;
    bool m_isResizable;

    EventManager m_eventManager;
    bool m_isFocused;
};

#endif // WINDOW_H
