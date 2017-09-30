#include "abstracttower.h"
#include "abstractenemy.h"
#include "towermanager.h"
#include "window.h"
#include "bulletmanager.h"

AbstractTower::AbstractTower(TowerProporties *l_proporties, TowerManager *l_towerManager) :
    m_proporties(l_proporties),
    m_towerManager(l_towerManager),
    m_lookinAt(nullptr),
    m_currentUpgrade(0),
    m_strategy(AttackStrategy::First),
    m_angle(0.f),
    m_shootElapsed(0.f) { }

void AbstractTower::RotateToEnemy(const std::shared_ptr<EnemyBase>& l_enemy)
{
    if(m_proporties->m_towerRotation && m_lookinAt){
        sf::Vector2f pos = l_enemy->GetProporties()->m_position;
        m_angle = static_cast<float>(atan2(m_position.y - pos.y, m_position.x - pos.x) * 180 / 3.141);
        m_angle -= 90;
    }
    m_proporties->m_sprite.setRotation(m_angle);
}

void AbstractTower::Shot(const std::shared_ptr<EnemyBase> &l_enemy)
{
    m_towerManager->GetContext()->m_bulletManager->SpawnBullet(this, l_enemy);
}

void AbstractTower::Draw(sf::RenderWindow *l_wind)
{
    RotateToEnemy(m_lookinAt);
    l_wind->draw(m_proporties->m_sprite);
}

void AbstractTower::Update(const float &l_dT)
{

}
