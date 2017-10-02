#ifndef STATE_GAME_H
#define STATE_GAME_H


#include "basestate.h"
#include "eventmanager.h"
#include "towermanager.h"
#include "enemymanager.h"
#include "statistics.h"
#include "bulletmanager.h"
#include <memory>

using Connections = std::unordered_map<std::string, std::shared_ptr<TowerProporties>>;

struct TowerProporties;
class GUI_Element;
class Level;
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

    void TowerPressed(EventDetails* l_details);
    void HandleRelease(EventDetails* l_details);
    void HandleKey(EventDetails* l_details);
    void StartGame(EventDetails* l_details);
    void SpeedChange(EventDetails* l_details);
private:
    inline void UpdateSpeedGUI();

    void PrepareElement(GUI_Element* l_element, const std::shared_ptr<TowerProporties>& l_proporties);
    Statistics m_statistics;
    std::unique_ptr<Level> m_level;
    TowerManager m_towerManager;
    EnemyManager m_enemyManager;
    BulletManager m_bulletManager;
    Connections m_connections;
    bool m_playing;
    unsigned int m_speed;
};

#endif // STATE_GAME_H
