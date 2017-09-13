#ifndef STATE_ABOUT_H
#define STATE_ABOUT_H

#include "basestate.h"
#include "eventmanager.h"

class State_About : public BaseState
{
public:
    State_About(StateManager* l_stateManager);
    ~State_About();
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time &l_time);
    void Draw();

    void MainMenu(EventDetails* l_details);
};

#endif // STATE_ABOUT_H
