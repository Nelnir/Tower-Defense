#include "state_about.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"

#ifdef WIN32
#include <windows.h>
#include <ShellApi.h>
#pragma comment(lib, "SHELL32.LIB")
#endif
State_About::State_About(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_About::~State_About()
{

}

void State_About::OnCreate()
{
    SetTransparent(true);
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

    guiM->LoadInterface(StateType::About, "About.interface", "AboutInterface");
    GUI_Interface* interfac = guiM->GetInterface(StateType::About, "AboutInterface");
    sf::Vector2f interfaceSize = interfac->GetSize();
    interfac->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->AddCallback(StateType::About, "About_Back", &State_About::MainMenu, this);
    eveM->AddCallback(StateType::About, "Key_ESC", &State_About::MainMenu, this);
    eveM->AddCallback(StateType::About, "GitHub_Clicked", &State_About::GitHubClicked, this);
}

void State_About::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::About, "AboutInterface");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::About, "About_Back");
    eveM->RemoveCallback(StateType::About, "Key_ESC");
    eveM->RemoveCallback(StateType::About, "GitHub_Clicked");
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
}

void State_About::GitHubClicked(EventDetails *l_details)
{
#ifdef WIN32
    ShellExecute(NULL, NULL, L"https://github.com/Nelnir/Tower-Defense", NULL, NULL, SW_SHOW);
#endif
}
