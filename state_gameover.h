#ifndef STATE_GAMEOVER_H
#define STATE_GAMEOVER_H

#include "basestate.h"
#include "eventmanager.h"

class GUI_Interface;
class State_GameOver : public BaseState
{
public:
    State_GameOver(StateManager* l_stateMgr);
    ~State_GameOver();
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time &l_time);
    void Draw();


    void TryAgain(EventDetails* l_details);
private:
    GUI_Interface* m_interface; /// game state interface
    GUI_Interface* m_lostInterface;
    float m_elapsed;
    float m_tick;
    float m_speed;
    bool m_moving;
};

#endif // STATE_GAMEOVER_H
