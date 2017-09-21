#include "State_Game.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "level.h"

State_Game::State_Game(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_Game::~State_Game()
{

}

void State_Game::OnCreate()
{
    m_level = new Level(m_stateMgr->GetContext());
    m_stateMgr->GetContext()->m_level = m_level;
    m_level->LoadLevel("Level-" + std::to_string(m_stateMgr->GetContext()->m_settings->GetCurrentLevel()) + ".level");
}

void State_Game::OnDestroy()
{

}

void State_Game::Update(const sf::Time &l_time)
{
    m_level->Update(l_time.asSeconds());
}

void State_Game::Draw()
{
    for(unsigned int i = 0; i < Sheet::Num_Layers; ++i){
        m_level->Draw(i);
        SharedContext* context = m_stateMgr->GetContext();
    }
}

void State_Game::Activate()
{

}

void State_Game::Deactivate()
{

}
