#include "State_Game.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "level.h"
#include "towermanager.h"

State_Game::State_Game(StateManager *l_stateManager) : BaseState(l_stateManager), m_zoom(1.5), m_towerManager(m_stateMgr->GetContext(), m_zoom) {}

State_Game::~State_Game()
{

}

void State_Game::OnCreate()
{
    m_stateMgr->GetContext()->m_towerManager = &m_towerManager;
    m_level = new Level(m_stateMgr->GetContext());
    m_stateMgr->GetContext()->m_level = m_level;
    m_level->LoadLevel("Level-" + std::to_string(m_stateMgr->GetContext()->m_settings->GetCurrentLevel()) + ".level");
    m_view.setCenter(1087, 544);
    m_view.zoom(m_zoom);


    SetTransparent(true);
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::Game, "Game.interface", "GameInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::Game, "GameInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x - interfaceSize.x, windowSize.y - interfaceSize.y});

    PrepareElement(interface->GetElement("Tower1"), m_towerManager.GetProporties(TowerType::Neutral));

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->AddCallback(StateType::Game, "Tower1", &State_Game::TowerPressed, this);
    eveM->AddCallback(StateType::Game, "Mouse_Left_Release", &State_Game::HandleRelease, this);
    eveM->AddCallback(StateType::Game, "Mouse_Right_Release", &State_Game::HandleRelease, this);
    eveM->AddCallback(StateType::Game, "Key_ESC", &State_Game::HandleKey, this);
}

void State_Game::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::Game, "GameInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::Game, "Tower1");
    eveM->RemoveCallback(StateType::Game, "Mouse_Left_Release");
    eveM->RemoveCallback(StateType::Game, "Mouse_Right_Release");
    eveM->RemoveCallback(StateType::Game, "Key_ESC");
}

void State_Game::Update(const sf::Time &l_time)
{
    m_level->Update(l_time.asSeconds());
    m_towerManager.Update(l_time.asSeconds());
}

void State_Game::Draw()
{
    for(unsigned int i = 0; i < Sheet::Num_Layers; ++i){
        m_level->Draw(i);
    }
    m_towerManager.Draw();
}

void State_Game::Activate()
{

}

void State_Game::Deactivate()
{

}

void State_Game::TowerPressed(EventDetails* l_details)
{
    m_towerManager.Pressed(m_connections.find(l_details->m_name)->second);
}

void State_Game::HandleRelease(EventDetails *l_details)
{
    m_towerManager.HandleRelease(l_details);
}

void State_Game::PrepareElement(GUI_Element *l_element, TowerProporties *l_proporties)
{
    m_connections.emplace(l_element->GetName(), l_proporties);
    l_element->SetText(std::to_string(l_proporties->m_cost) + m_stateMgr->GetContext()->m_settings->GetCurrency());
    for(int i = static_cast<int>(GUI_ElementState::Neutral); i <= static_cast<int>(GUI_ElementState::Clicked); ++i){
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
