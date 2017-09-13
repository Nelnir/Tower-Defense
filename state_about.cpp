#include "state_about.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"

State_About::State_About(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_About::~State_About()
{

}

void State_About::OnCreate()
{
    SetTransparent(true);
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::About, "About.interface", "AboutInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::About, "AboutInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    eveM->AddCallback(StateType::About, "About_Back", &State_About::MainMenu, this);
}

void State_About::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::About, "AboutInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::About, "About_Back");
}

void State_About::Update(const sf::Time &l_time)
{

}

void State_About::Draw()
{

}

void State_About::Activate()
{

}

void State_About::Deactivate()
{

}

void State_About::MainMenu(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::MainMenu);
   // m_stateMgr->Remove(StateType::About);
}
