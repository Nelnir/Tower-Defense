#ifndef MISSILE_H
#define MISSILE_H

#include "bullet.h"

class Missile : public Bullet
{
public:
    Missile(BulletManager* l_manager, BulletProporties* m_proporties, std::shared_ptr<EnemyBase> m_enemy);
    void Update(const float &l_dT);
    void Draw(sf::RenderWindow *l_window);
    void Initialize(TowerBase *l_tower);
private:
    void UpdateDestination();
    float m_speed;
};

#endif // MISSILE_H
