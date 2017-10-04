#include "state_mainmenu.h"
#include "gui_manager.h"
#include "statemanager.h"
#include "texturemanager.h"
#include "window.h"

State_MainMenu::State_MainMenu(StateManager *l_stateMgr) : BaseState(l_stateMgr) { }

void State_MainMenu::OnCreate()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    TextureManager* texM = m_stateMgr->GetContext()->m_textureManager;

    texM->RequireResource("Bg");
    m_bg.setTexture(*texM->GetResource("Bg"));

    float scaleX = 1;
    float scaleY = 1;
    sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
    sf::Vector2u textureSize = m_bg.getTexture()->getSize();
    scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    m_bg.setScale(scaleX, scaleY);
    m_bg.setPosition(0, 0);

    guiM->LoadInterface(StateType::MainMenu, "MainMenu.interface", "MainInterface");
    GUI_Interface* interface = guiM->GetInterface(StateType::MainMenu, "MainInterface");
    sf::Vector2f interfaceSize = interface->GetSize();
    interface->SetPosition({windowSize.x / 2.f - interfaceSize.x / 2.f, windowSize.y / 2.f - interfaceSize.y / 2.f});

    eveM->AddCallback(StateType::MainMenu, "MainMenu_Quit", &State_MainMenu::Quit, this);
    eveM->AddCallback(StateType::MainMenu, "MainMenu_Play", &State_MainMenu::Play, this);
    eveM->AddCallback(StateType::MainMenu, "MainMenu_Settings", &State_MainMenu::Settings, this);
    eveM->AddCallback(StateType::MainMenu, "MainMenu_About", &State_MainMenu::About, this);
}

void State_MainMenu::OnDestroy()
{
    GUI_Manager* guiM = m_stateMgr->GetContext()->m_guiManager;
    guiM->RemoveInterface(StateType::MainMenu, "MainInterface");

    TextureManager* texM = m_stateMgr->GetContext()->m_textureManager;
    texM->ReleaseResource("Bg");

    EventManager* eveM = m_stateMgr->GetContext()->m_eventManager;
    eveM->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
    eveM->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
    eveM->RemoveCallback(StateType::MainMenu, "MainMenu_Settings");
    eveM->RemoveCallback(StateType::MainMenu, "MainMenu_About");
}

void State_MainMenu::Activate()
{

}

void State_MainMenu::Deactivate()
{

}

void State_MainMenu::Update(const sf::Time &l_time)
{

}

void State_MainMenu::Draw()
{
    sf::RenderWindow* wind = m_stateMgr->GetContext()->m_wind->getRenderWindow();
    wind->draw(m_bg);
}

void State_MainMenu::Play(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::SelectLevel);
}

void State_MainMenu::Quit(EventDetails *l_details)
{
    m_stateMgr->GetContext()->m_wind->Close(l_details);
}

void State_MainMenu::Settings(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::Settings);
}

void State_MainMenu::About(EventDetails *l_details)
{
    m_stateMgr->SwitchTo(StateType::About);
}
