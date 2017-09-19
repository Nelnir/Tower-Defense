#include "statemanager.h"
#include "eventmanager.h"
#include "window.h"
#include "gui_manager.h"
#include <iostream>

#include "state_mainmenu.h"
#include "state_about.h"
#include "state_settings.h"
#include "state_game.h"
#include "state_selectlevel.h"

StateManager::StateManager(SharedContext *l_shared) :
    m_shared(l_shared)
{
    RegisterState<State_MainMenu>(StateType::MainMenu);
    RegisterState<State_About>(StateType::About);
    RegisterState<State_Settings>(StateType::Settings);
    RegisterState<State_Game>(StateType::Game);
    RegisterState<State_SelectLevel>(StateType::SelectLevel);
}

StateManager::~StateManager()
{
    for(auto &itr : m_states){
        itr.second->OnDestroy();
        delete itr.second;
        itr.second = nullptr;
    }
}

void StateManager::Draw()
{
    if(m_states.empty()){
        return;
    }

    if(m_states.back().second->IsTransparent() && m_states.size() > 1)
    {
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end())
            {
                if (!itr->second->IsTransparent()){
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr){
            m_shared->m_wind->getRenderWindow()->setView(itr->second->GetView());
            itr->second->Draw();
        }
    }
    else{
        m_states.back().second->Draw();
    }
}

void StateManager::Update(const sf::Time &l_time)
{
    if(m_states.empty()){
        return;
    }

    /// std::vector<std::pair<StateType, BaseState*>>;
    if(m_states.back().second->IsTranscendent() && m_states.size() > 1) /// if allows other states to be updated
    {
        /// find state that doesnt allow update other states, or first
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end())
            {
                if (!itr->second->IsTranscendent()){
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr){
            itr->second->Update(l_time);
        }
    }
    else{
        m_states.back().second->Update(l_time);
    }
}

bool StateManager::HasState(const StateType &l_type)
{
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == l_type)
        {
            auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
            if (removed == m_toRemove.end()){
                return true;
            }
            return false;
        }
    }
    return false;
}

void StateManager::ProcessRequests()
{
    while (m_toRemove.begin() != m_toRemove.end())
    {
        RemoveState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}
void StateManager::SwitchTo(const StateType &l_type)
{
    m_shared->m_eventManager->SetCurrentState(l_type);
    m_shared->m_guiManager->SetCurrentState(l_type);
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == l_type)
         {
            m_states.back().second->Deactivate();
            StateType tmp_type = itr->first;
            BaseState* tmp_state = itr->second;

            m_shared->m_wind->getRenderWindow()->setView(tmp_state->GetView());

            m_states.erase(itr);
            m_states.emplace_back(tmp_type, tmp_state);
            tmp_state->Activate();
            return;
        }
    }

    // State with l_type wasn't found.
    if (!m_states.empty()){
        m_states.back().second->Deactivate();
    }

    if(!CreateState(l_type)){
        std::cerr << "Failed creating state: " << static_cast<int>(l_type) << std::endl;
        return;
    }
    m_states.back().second->Activate();
    m_shared->m_wind->getRenderWindow()->setView(m_states.back().second->GetView());
}

bool StateManager::CreateState(const StateType &l_type)
{
    auto& newState = m_stateFactory.find(l_type);
    if(newState == m_stateFactory.end()){
        return false;
    }
    BaseState* state = newState->second();

    state->m_view = m_shared->m_wind->getRenderWindow()->getDefaultView();

    m_states.emplace_back(l_type, state);
    state->OnCreate();
    return true;
}

void StateManager::RemoveState(const StateType &l_type)
{
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == l_type)
        {
            itr->second->OnDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}
