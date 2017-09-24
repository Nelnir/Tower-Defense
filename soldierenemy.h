#ifndef SOLDIERENEMY_H
#define SOLDIERENEMY_H

#include "abstractenemy.h"

class SoldierEnemy : public AbstractEnemy
{
public:
    SoldierEnemy(EnemyProporties* l_prop, EnemyManager* l_mgr);
    ~SoldierEnemy();

    void Update(const float& l_dT);
    void Draw(sf::RenderWindow *l_window);
private:
    sf::Sprite m_sprite;
};

#endif // SOLDIERENEMY_H
