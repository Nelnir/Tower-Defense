#include "State_Game.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "level.h"
#include "towermanager.h"

State_Game::State_Game(StateManager *l_stateManager) :
    BaseState(l_stateManager),
    m_towerManager(m_stateMgr->GetContext(), &m_statistics),
    m_enemyManager(m_stateMgr->GetContext(), &m_statistics),
    m_bulletManager(m_stateMgr->GetContext()),
    m_playing(false),
    m_speed(1){}

State_Game::~State_Game()
{

}

void State_Game::OnCreate()
{
    SetTransparent(true);
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::Game, "Game.interface", "GameInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::Game, "GameInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x - interfaceSize.x, windowSize.y - interfaceSize.y});

    PrepareElement(interface->GetElement("Tower1"), m_towerManager.GetProporties(Tower::Basic));
    PrepareElement(interface->GetElement("Tower2"), m_towerManager.GetProporties(Tower::BasicPRO));
    PrepareElement(interface->GetElement("Tower3"), m_towerManager.GetProporties(Tower::Missile));

    m_level = std::make_unique<Level>(m_stateMgr->GetContext(), interface, &m_connections, &m_statistics);
    m_level->LoadTiles("tiles.cfg");
    m_level->LoadLevel("Level-" + std::to_string(m_stateMgr->GetContext()->m_settings->GetCurrentLevel()) + ".level");
    m_view.setCenter(m_level->GetCenter());
    m_view.zoom(m_level->GetZoom());
    m_towerManager.SetZoom(m_level->GetZoom());

    m_stateMgr->GetContext()->m_towerManager = &m_towerManager;
    m_stateMgr->GetContext()->m_enemyManager = &m_enemyManager;
    m_stateMgr->GetContext()->m_statistics = &m_statistics;
    m_stateMgr->GetContext()->m_level = m_level.get();
    m_stateMgr->GetContext()->m_bulletManager = &m_bulletManager;

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->AddCallback(StateType::Game, "Tower1", &State_Game::TowerPressed, this);
    eveM->AddCallback(StateType::Game, "Tower2", &State_Game::TowerPressed, this);
    eveM->AddCallback(StateType::Game, "Tower3", &State_Game::TowerPressed, this);
    eveM->AddCallback(StateType::Game, "Mouse_Left_Release", &State_Game::HandleRelease, this);
    eveM->AddCallback(StateType::Game, "Mouse_Right_Release", &State_Game::HandleRelease, this);
    eveM->AddCallback(StateType::Game, "Key_ESC", &State_Game::HandleKey, this);
    eveM->AddCallback(StateType::Game, "Start_Game", &State_Game::StartGame, this);
    eveM->AddCallback(StateType::Game, "Key_P", &State_Game::StartGame, this);
    eveM->AddCallback(StateType::Game, "SpeedChange", &State_Game::SpeedChange, this);

    UpdateSpeedGUI();

}

void State_Game::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::Game, "GameInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::Game, "Tower1");
    eveM->RemoveCallback(StateType::Game, "Tower2");
    eveM->RemoveCallback(StateType::Game, "Tower3");
    eveM->RemoveCallback(StateType::Game, "Mouse_Left_Release");
    eveM->RemoveCallback(StateType::Game, "Mouse_Right_Release");
    eveM->RemoveCallback(StateType::Game, "Key_ESC");
    eveM->RemoveCallback(StateType::Game, "Start_Game");
    eveM->RemoveCallback(StateType::Game, "SpeedChange");
    eveM->RemoveCallback(StateType::Game, "Key_P");
}

void State_Game::Update(const sf::Time &l_time)
{
    if(!m_playing){
        return;
    }
    m_level->Update(l_time.asSeconds() * m_speed);
    m_towerManager.Update(l_time.asSeconds() * m_speed);
    m_bulletManager.Update(l_time.asSeconds() * m_speed);
    m_bulletManager.ProcessRequests();
    m_enemyManager.Update(l_time.asSeconds() * m_speed);
    m_enemyManager.ProcessRequests();
    m_statistics.AddTimePlayed(l_time.asSeconds());
}

void State_Game::Draw()
{
    for(unsigned int i = 0; i < Sheet::Num_Layers; ++i){
       m_level->Draw(i);
    }
    m_towerManager.Draw();
    m_enemyManager.Draw();
    m_bulletManager.Draw();
}

void State_Game::Activate()
{
    m_speed = 1;
    UpdateSpeedGUI();
    m_playing = false; /// When we clicked in game_over state try again, this needs to be set to false to prevent immediatly starting first wave
}

void State_Game::Deactivate()
{
    m_playing = false;
}

void State_Game::TowerPressed(EventDetails* l_details)
{
    m_towerManager.Pressed(m_connections.find(l_details->m_name)->second);
}

void State_Game::HandleRelease(EventDetails *l_details)
{
    m_towerManager.HandleRelease(l_details);
}

void State_Game::PrepareElement(GUI_Element *l_element, const std::shared_ptr<TowerProporties>& l_proporties)
{
    m_connections.emplace(l_element->GetName(), l_proporties);
    l_element->SetText(std::to_string(l_proporties->m_cost) + m_stateMgr->GetContext()->m_settings->GetCurrency());
    for(int i = static_cast<int>(GUI_ElementState::Neutral); i < static_cast<int>(GUI_ElementState::Count); ++i){
        GUI_Style style = l_element->GetStyle(static_cast<GUI_ElementState>(i));
        style.m_backgroundImage = l_proporties->m_texture;
        style.m_imageRect = l_proporties->m_sprite.getTextureRect();
        l_element->UpdateBgStyle(static_cast<GUI_ElementState>(i), style);
    }
}

void State_Game::HandleKey(EventDetails *l_details)
{
    m_towerManager.HandleKey(l_details);
}

void State_Game::StartGame(EventDetails *l_details)
{
    if(!m_playing){
        m_playing = true;
        m_level->GetInterface()->GetElement("Start")->SetText("Pause");
    } else{
        m_playing = false;
        m_level->GetInterface()->GetElement("Start")->SetText("Resume");
    }
}

void State_Game::SpeedChange(EventDetails *l_details)
{
    m_speed *= 2;
    if(m_speed > 8 || m_speed <= 0){
        m_speed = 1;
    }
    UpdateSpeedGUI();
}

void State_Game::UpdateSpeedGUI()
{
    m_level->GetInterface()->GetElement("Speed")->SetText("Speed: " + std::to_string(m_speed) + "x");
}
