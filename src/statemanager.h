#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "basestate.h"
#include "sharedcontext.h"
#include <vector>
#include <unordered_map>
#include <functional>


using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager
{
public:
    StateManager(SharedContext* l_shared);
    ~StateManager();
    void Update(const sf::Time& l_time);
    void Draw();
    void ProcessRequests();
    inline SharedContext* GetContext() { return m_shared; }
    bool HasState(const StateType& l_type);
    void SwitchTo(const StateType& l_type);
    void Remove(const StateType& l_type){
        m_toRemove.push_back(l_type);
    }
private:
    // Methods.
    bool CreateState(const StateType& l_type);
    void RemoveState(const StateType& l_type);

    template<class T>
    void RegisterState(const StateType& l_type)
    {
        m_stateFactory[l_type] = [this] () -> BaseState *
        {
            return new T(this);
        };
    }

    // Members.
    SharedContext* m_shared;
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_stateFactory;
};

#endif // STATEMANAGER_H
