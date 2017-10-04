#include "state_settings.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"

State_Settings::State_Settings(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_Settings::~State_Settings()
{

}

void State_Settings::OnCreate()
{
    SetTransparent(true);
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::Settings, "Settings.interface", "SettingsInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::Settings, "SettingsInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    eveM->AddCallback(StateType::Settings, "Settings_Back", &State_Settings::MainMenu, this);
    eveM->AddCallback(StateType::Settings, "Key_ESC", &State_Settings::MainMenu, this);
}

void State_Settings::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::Settings, "SettingsInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::Settings, "Settings_Back");
    eveM->RemoveCallback(StateType::Settings, "Key_ESC");
}

void State_Settings::Update(const sf::Time &l_time)
{

}

void State_Settings::Draw()
{

}

void State_Settings::Activate()
{

}

void State_Settings::Deactivate()
{

}

void State_Settings::MainMenu(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::MainMenu);
}
