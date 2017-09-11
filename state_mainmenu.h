#ifndef STATE_MAINMENU_H
#define STATE_MAINMENU_H

#include "basestate.h"
#include "eventmanager.h"

class State_MainMenu : public BaseState
{
public:
    State_MainMenu(StateManager* l_stateMgr);
    ~State_MainMenu() {}
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time& l_time);
    void Draw();

    void Quit(EventDetails* l_details);
    void Play(EventDetails* l_details);
    void About(EventDetails* l_details);
    void Settings(EventDetails* l_details);
private:
    sf::Sprite m_bg;
};

#endif // STATE_MAINMENU_H
