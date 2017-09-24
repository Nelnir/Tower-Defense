#ifndef NORMAL_TOWER_H
#define NORMAL_TOWER_H

#include "abstracttower.h"

class Normal_Tower : public AbstractTower
{
public:
    Normal_Tower(TowerProporties* l_proporties, TowerManager* l_towerManager);
    void Draw(sf::RenderWindow* l_wind);
    void Update(const float &l_dT, AbstractEnemy* l_lookinAt) {}
protected:
    void Shot(AbstractEnemy *l_enemy) {}
};

#endif // NORMAL_TOWER_H
