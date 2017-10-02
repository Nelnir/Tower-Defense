#ifndef TOWER_MISSILE_H
#define TOWER_MISSILE_H

#include "TowerBase.h"

struct TowerMissileProporties : public TowerProporties{
    ~TowerMissileProporties(){}
    std::string m_missileTexture;
    sf::Sprite m_missileSprite;
};

class Tower_Missile : public TowerBase
{
public:
    Tower_Missile(const std::shared_ptr<TowerProporties>& l_prop, TowerManager* l_towerManager);
    void Draw(sf::RenderWindow *l_wind);
    void Update(const float &l_dT);
    void Shot(const std::shared_ptr<EnemyBase> &l_enemy);
    void SetEnemy(const std::shared_ptr<EnemyBase> &l_enemy);
private:
    std::shared_ptr<TowerMissileProporties> m_missileProporties;
    bool m_justTargeted;
};

#endif // TOWER_MISSILE_H
