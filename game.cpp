#include "game.h"
#include <iostream>
Game::Game() : m_window("Very Basic Engine", sf::Vector2u(1280, 720)),
    m_stateManager(&m_context),
    m_guiManager(m_window.GetEventManager(), &m_context)
{
    m_context.m_wind = &m_window;
    m_context.m_eventManager = m_window.GetEventManager();
    m_context.m_textureManager = &m_textureManager;
    m_context.m_fontManager = &m_fontManager;
    m_context.m_guiManager = &m_guiManager;
    m_context.m_settings = &m_settings;

    m_stateManager.SwitchTo(StateType::MainMenu);
}

Game::~Game()
{

}

/*void Game::HandleInput()
{

}*/

void Game::Update()
{
    /// handling events & input
    m_window.Update();
    m_stateManager.Update(m_elapsed);
    m_guiManager.Update(m_elapsed.asSeconds());
    GUI_Event guiEvent;
    while(m_guiManager.PollEvent(guiEvent)){
        m_window.GetEventManager()->HandleEvent(guiEvent);;
    }
}

void Game::Render()
{
    m_window.BeginDraw();
    m_stateManager.Draw();

    sf::RenderWindow* window = m_window.getRenderWindow();
    sf::View CurrentView = window->getView();
    window->setView(window->getDefaultView());
    m_guiManager.Render(window);
    window->setView(CurrentView);

    m_window.EndDraw();
}
void Game::LateUpdate()
{
    m_stateManager.ProcessRequests();
    RestartClock();
}

void Game::RestartClock()
{
     m_elapsed = m_clock.restart();
}
