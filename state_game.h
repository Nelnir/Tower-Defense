#ifndef STATE_GAME_H
#define STATE_GAME_H


#include "basestate.h"
#include "eventmanager.h"

class State_Game : public BaseState
{
public:
    State_Game(StateManager* l_stateManager);
    ~State_Game();
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time &l_time);
    void Draw();
};

#endif // STATE_GAME_H
