#include "state_selectLevel.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "level.h"

State_SelectLevel::State_SelectLevel(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_SelectLevel::~State_SelectLevel()
{

}

void State_SelectLevel::OnCreate()
{
    SetTransparent(true);
    m_currentVisibleLevels = 3;
    m_scroll = false;
    m_destinationPercent = m_currentPercent = 0;
    m_tick = 0;
    m_speed = 2;
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::SelectLevel, "SelectLevel.interface", "LevelInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::SelectLevel, "LevelInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    guiM->LoadInterface(StateType::SelectLevel, "SelectLevel-Levels.interface", "LevelsInterface");
    m_levelsInterface = guiM->GetInterface(StateType::SelectLevel, "LevelsInterface");
    interfaceSize = m_levelsInterface->GetSize();
    m_levelsInterface->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    eveM->AddCallback(StateType::SelectLevel, "Level_Back", &State_SelectLevel::MainMenu, this);
    eveM->AddCallback(StateType::SelectLevel, "Key_ESC", &State_SelectLevel::MainMenu, this);
    eveM->AddCallback(StateType::SelectLevel, "Level_RightArrow", &State_SelectLevel::RightArrow, this);
    eveM->AddCallback(StateType::SelectLevel, "Level_LeftArrow", &State_SelectLevel::LeftArrow, this);
}

void State_SelectLevel::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::SelectLevel, "LevelInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::SelectLevel, "Level_Back");
    eveM->RemoveCallback(StateType::SelectLevel, "Key_ESC");
    eveM->RemoveCallback(StateType::SelectLevel, "Level_RightArrow");
    eveM->RemoveCallback(StateType::SelectLevel, "Level_LeftArrow");
}

void State_SelectLevel::Update(const sf::Time &l_time)
{
    if(!m_scroll){
        return;
    }

    m_elapsed += l_time.asSeconds();

    if(m_elapsed >= 1.f / m_speed){
        m_elapsed = 0;
        m_scroll = false;
        m_levelsInterface->UpdateScrollHorizontal(m_destinationPercent);
        m_currentPercent = m_destinationPercent;
    }
    else{
        m_levelsInterface->UpdateScrollHorizontal(m_currentPercent + m_elapsed * m_tick);
    }
}

void State_SelectLevel::Draw()
{

}

void State_SelectLevel::Activate()
{

}

void State_SelectLevel::Deactivate()
{

}

void State_SelectLevel::MainMenu(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_SelectLevel::LeftArrow(EventDetails *l_details)
{
    int m_levels = m_stateMgr->GetContext()->m_level->GetNumberOfLevels();
    if(m_levels >= m_currentVisibleLevels){
        return;
    }
    m_scroll = true;
    m_currentVisibleLevels -= 3;
    if(m_currentVisibleLevels == 3){
        m_destinationPercent = 0;
        m_tick = -m_currentPercent * m_speed;
        return;
    }
    m_destinationPercent =  m_currentVisibleLevels / m_levels * 100;
    m_tick = m_destinationPercent * m_speed;
}

void State_SelectLevel::RightArrow(EventDetails *l_details)
{
    int m_levels = m_stateMgr->GetContext()->m_level->GetNumberOfLevels();
    if(m_levels <= m_currentVisibleLevels){
        return;
    }
    m_scroll = true;
    m_currentVisibleLevels += 3;
    m_destinationPercent =  m_currentVisibleLevels / m_levels * 100;
    m_tick = m_destinationPercent * m_speed;
}
