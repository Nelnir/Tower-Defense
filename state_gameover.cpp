#include "state_gameover.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "level.h"
#include "enemymanager.h"
#include "towermanager.h"
#include "bulletmanager.h"

State_GameOver::State_GameOver(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_GameOver::~State_GameOver()
{

}

void State_GameOver::OnCreate()
{
    SetTransparent(true);
    m_moving = true;
    m_speed = 0.75f;
    m_elapsed = 0;


    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;

    /// interface from game
    m_interface = guiM->GetInterface(StateType::Game, "GameInterface");

    /// game over interface
    if(!m_stateMgr->GetContext()->m_settings->Won()){
        guiM->LoadInterface(StateType::GameOver, "GameOver.interface", "GameOverInterface");
    } else{
        guiM->LoadInterface(StateType::GameOver, "GameOverWin.interface", "GameOverInterface");
    }
    m_overInterface = guiM->GetInterface(StateType::GameOver, "GameOverInterface");
    m_overInterface->SetPosition(sf::Vector2f(0.f, -m_overInterface->GetSize().y));
    m_tick = m_overInterface->GetSize().y;

    /// statistics interface
    guiM->LoadInterface(StateType::GameOver, "Statistics.interface", "StatisticsInterface");
    m_statisticsInterface = guiM->GetInterface(StateType::GameOver, "StatisticsInterface");
    sf::Vector2f size = m_statisticsInterface->GetSize();
    m_destination = sf::Vector2f(windowSize.x / 2.f - size.x / 2.f, windowSize.y / 2.f - size.y - 25);
    m_statisticsInterface->SetPosition(sf::Vector2f(m_destination.x, -m_destination.y));

    m_stateMgr->GetContext()->m_statistics->SetupInterface(m_statisticsInterface);


    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->AddCallback(StateType::GameOver, "TryAgain", &State_GameOver::TryAgain, this);
    eveM->AddCallback(StateType::GameOver, "ExitToMenu", &State_GameOver::ExitToMenu, this);
    eveM->AddCallback(StateType::GameOver, "ExitFromGame", &State_GameOver::ExitFromGame, this);
}

void State_GameOver::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::GameOver, "GameOverInterface");
    guiM->RemoveInterface(StateType::GameOver, "StatisticsInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::GameOver, "TryAgain");
    eveM->RemoveCallback(StateType::GameOver, "ExitFromGame");
    eveM->RemoveCallback(StateType::GameOver, "ExitToMenu");
}

void State_GameOver::Update(const sf::Time &l_time)
{
    if(!m_moving){
        return;
    }
    m_elapsed += l_time.asSeconds();
    if(m_elapsed > 1.f / m_speed){
        m_overInterface->SetPosition(sf::Vector2f(0.f, 0.f));
        m_statisticsInterface->SetPosition(sf::Vector2f(m_destination.x, m_destination.y));
        m_moving = false;
    } else{
        m_overInterface->SetPosition(sf::Vector2f(0.f, (m_elapsed * m_tick * m_speed) - m_tick));
        m_statisticsInterface->SetPosition(sf::Vector2f(m_destination.x, (m_elapsed * m_destination.y * m_speed * 3) - m_destination.y * 2));
    }
}

void State_GameOver::Draw()
{
    m_stateMgr->GetContext()->m_guiManager->Render(m_interface);
}

void State_GameOver::Activate()
{

}

void State_GameOver::Deactivate()
{

}

void State_GameOver::TryAgain(EventDetails *l_details)
{
    m_stateMgr->GetContext()->m_bulletManager->Purge();
    m_stateMgr->GetContext()->m_level->Restart();
    m_stateMgr->GetContext()->m_enemyManager->Restart();
    m_stateMgr->GetContext()->m_towerManager->Restart();
    m_stateMgr->GetContext()->m_statistics->Reset();
    m_stateMgr->SwitchTo(StateType::Game);
    m_stateMgr->Remove(StateType::GameOver);
}

void State_GameOver::ExitFromGame(EventDetails *l_details)
{
    m_stateMgr->GetContext()->m_wind->Close(l_details);
}

void State_GameOver::ExitToMenu(EventDetails *l_details)
{
    m_stateMgr->Remove(StateType::Game);
    m_stateMgr->Remove(StateType::GameOver);
    m_stateMgr->SwitchTo(StateType::MainMenu);
}
