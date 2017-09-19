#ifndef State_SelectLevel_H
#define State_SelectLevel_H


#include "basestate.h"
#include "eventmanager.h"

class GUI_Interface;

class State_SelectLevel : public BaseState
{
public:
    State_SelectLevel(StateManager* l_stateManager);
    ~State_SelectLevel();
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time &l_time);
    void Draw();

    void MainMenu(EventDetails* l_details);
    void RightArrow(EventDetails* l_details);
    void LeftArrow(EventDetails* l_details);
private:
    int m_currentVisibleLevels;
    bool m_scroll;
    int m_destinationPercent;
    int m_currentPercent;
    float m_elapsed;
    float m_tick;
    float m_speed;

    GUI_Interface* m_levelsInterface;
};

#endif // State_SelectLevel_H
