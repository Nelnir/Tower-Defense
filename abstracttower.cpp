#include "abstracttower.h"
#include "abstractenemy.h"

AbstractTower::AbstractTower(TowerProporties *l_proporties, TowerManager *l_towerManager) :
    m_proporties(l_proporties),
    m_towerManager(l_towerManager),
    m_lookinAt(nullptr),
    m_currentUpgrade(0),
    m_strategy(AttackStrategy::First),
    m_angle(0.f) { }

void AbstractTower::RotateToEnemy(AbstractEnemy* l_enemy)
{
    if(m_proporties->m_towerRotation && m_lookinAt){
        sf::Vector2f pos = l_enemy->GetProporties()->m_position;
        m_angle = static_cast<float>(atan2(m_position.y - pos.y, m_position.x - pos.x) * 180 / 3.141);
        m_angle -= 90;
    }
    m_proporties->m_sprite.setRotation(m_angle);
}
