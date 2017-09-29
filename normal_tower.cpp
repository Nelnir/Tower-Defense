#include "normal_tower.h"
#include "towermanager.h"
#include "bulletmanager.h"
#include <iostream>
Normal_Tower::Normal_Tower(TowerProporties *l_proporties, TowerManager *l_towerManager) : AbstractTower(l_proporties, l_towerManager) {}

void Normal_Tower::Draw(sf::RenderWindow* wind)
{
    RotateToEnemy(m_lookinAt);
    wind->draw(m_proporties->m_sprite);
}

void Normal_Tower::Update(const float &l_dT)
{

}

void Normal_Tower::Shot(const std::shared_ptr<EnemyBase>& l_enemy)
{
    m_towerManager->GetContext()->m_bulletManager->SpawnBullet(this, l_enemy);
}
