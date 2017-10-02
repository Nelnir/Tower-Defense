#ifndef TANK_ENEMY_H
#define TANK_ENEMY_H

#include "EnemyBase.h"

struct TankProporties : public EnemyProporties{
    ~TankProporties() {}
    std::string m_tankTexture;
    sf::Sprite m_tankSprite;
};

class Tank_Enemy : public EnemyBase
{
public:
    Tank_Enemy(EnemyProporties* l_proporties, EnemyManager* l_manager);
    ~Tank_Enemy();
    void Draw(sf::RenderWindow *l_window);
    TankProporties* m_tankProporties;
};

#endif // TANK_ENEMY_H
