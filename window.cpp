#include "window.h"

Window::Window(const std::string &l_title, const sf::Vector2u &l_size, const bool& l_resizable)
{
    Setup(l_title, l_size, l_resizable);
}

Window::~Window()
{
    Destroy();
}

void Window::Setup(const std::string &l_title, const sf::Vector2u &l_size, const bool& l_resizable)
{
    m_windowTitle = l_title;
    m_windowSize = l_size;
    m_isFullscreen = false;
    m_isDone = false;
    m_isResizable = l_resizable;

    m_isFocused = true;

    m_eventManager.AddCallback(StateType(0), "Window_close", &Window::Close, this);

    Create();
}

void Window::Create()
{
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    if(!m_isResizable)
        style = sf::Style::Close;
    m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y, 32), m_windowTitle, style);
    m_window.setFramerateLimit(600);
    m_window.setKeyRepeatEnabled(false);
}

void Window::Destroy()
{
    m_window.close();
}

void Window::Update()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if (event.type == sf::Event::LostFocus){
            m_isFocused = false;
            m_eventManager.SetFocus(false);
        }
        else if (event.type == sf::Event::GainedFocus){
            m_isFocused = true;
            m_eventManager.SetFocus(true);
        }
        m_eventManager.HandleEvent(event);
    }
    m_eventManager.Update(); /// real time input and calling callbacks
}

void Window::ToggleFullscreen(EventDetails* l_details)
{
    m_isFullscreen = !m_isFullscreen;
    Destroy();
    Create();
}

void Window::UpdateResolution(sf::Vector2u &l_size)
{
    m_windowSize = l_size;
    Destroy();
    Create();
}

void Window::BeginDraw()
{
    m_window.clear(sf::Color::Black);
}

void Window::EndDraw()
{
    m_window.display();
}

void Window::Draw(sf::Drawable &l_drawable)
{
    m_window.draw(l_drawable);
}

sf::FloatRect Window::GetViewSpace()
{
    sf::Vector2f viewCenter = m_window.getView().getCenter();
    sf::Vector2f viewSize = m_window.getView().getSize();
    sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
    sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
    return viewSpace;
}
