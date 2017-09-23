#include "normal_tower.h"
#include "towermanager.h"
Normal_Tower::Normal_Tower(TowerProporties *l_proporties, TowerManager *l_towerManager) : AbstractTower(l_proporties, l_towerManager) {}

void Normal_Tower::Draw(sf::RenderWindow* wind)
{
    m_proporties->m_sprite.setPosition(m_position);
    wind->draw(m_proporties->m_sprite);
}
