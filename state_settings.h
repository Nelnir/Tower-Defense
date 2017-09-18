#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H


#include "basestate.h"
#include "eventmanager.h"

class State_Settings : public BaseState
{
public:
    State_Settings(StateManager* l_stateManager);
    ~State_Settings();
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time &l_time);
    void Draw();

    void MainMenu(EventDetails* l_details);
};

#endif // STATE_SETTINGS_H
