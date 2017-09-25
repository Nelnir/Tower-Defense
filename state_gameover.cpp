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

    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    m_interface = guiM->GetInterface(StateType::Game, "GameInterface");
    guiM->LoadInterface(StateType::GameOver, "GameOver.interface", "GameOverInterface");
    m_lostInterface = guiM->GetInterface(StateType::GameOver, "GameOverInterface");
    m_lostInterface->SetPosition(sf::Vector2f(0.f, -m_lostInterface->GetSize().y));
    m_tick = m_lostInterface->GetSize().y;

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->AddCallback(StateType::GameOver, "TryAgain", &State_GameOver::TryAgain, this);
}

void State_GameOver::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::GameOver, "GameOverInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::GameOver, "TryAgain");
}

void State_GameOver::Update(const sf::Time &l_time)
{
    if(!m_moving){
        return;
    }
    m_elapsed += l_time.asSeconds();
    if(m_elapsed > 1.f / m_speed){
        m_lostInterface->SetPosition(sf::Vector2f(0.f, 0.f));
        m_moving = false;
    } else{
        m_lostInterface->SetPosition(sf::Vector2f(0.f, (m_elapsed * m_tick * m_speed) - m_tick));
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
    m_stateMgr->GetContext()->m_level->Restart();
    m_stateMgr->GetContext()->m_enemyManager->Restart();
    m_stateMgr->GetContext()->m_towerManager->Restart();
    m_stateMgr->GetContext()->m_statistics->Reset();
    m_stateMgr->SwitchTo(StateType::Game);
    m_stateMgr->Remove(StateType::GameOver);
}
