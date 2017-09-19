#include "State_Game.h"
#include "statemanager.h"
#include "sharedcontext.h"
#include "window.h"
#include "texturemanager.h"
#include "gui_interface.h"
#include "gui_manager.h"

State_Game::State_Game(StateManager *l_stateManager) : BaseState(l_stateManager) {}

State_Game::~State_Game()
{

}

void State_Game::OnCreate()
{
    SetTransparent(true);
}

void State_Game::OnDestroy()
{

}

void State_Game::Update(const sf::Time &l_time)
{

}

void State_Game::Draw()
{

}

void State_Game::Activate()
{

}

void State_Game::Deactivate()
{

}
