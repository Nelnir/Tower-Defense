#include "TowerBase.h"
#include "EnemyBase.h"
#include "towermanager.h"
#include "window.h"
#include "bulletmanager.h"
TowerBase::TowerBase(const std::shared_ptr<TowerProporties>& l_proporties, TowerManager *l_towerManager) :
    m_proporties(l_proporties),
    m_towerManager(l_towerManager),
    m_lookinAt(nullptr),
    m_currentUpgrade(0),
    m_strategy(AttackStrategy::First),
    m_angle(0.f),
    m_shootElapsed(0.f) { }

void TowerBase::RotateToEnemy(const std::shared_ptr<EnemyBase>& l_enemy)
{
    if(m_proporties->m_towerRotation && m_lookinAt){
        sf::Vector2f pos = l_enemy->GetProporties()->m_position;
        m_angle = static_cast<float>(atan2(m_position.y - pos.y, m_position.x - pos.x) * 180 / 3.141);
        m_angle += m_proporties->m_towerRotationOffset;
    }
    m_proporties->m_sprite.setRotation(m_angle);
}

void TowerBase::Shot(const std::shared_ptr<EnemyBase> &l_enemy)
{
    m_towerManager->GetContext()->m_bulletManager->SpawnBullet(this, l_enemy);
}

void TowerBase::Draw(sf::RenderWindow *l_wind)
{
    RotateToEnemy(m_lookinAt);
    m_proporties->m_sprite.setPosition(m_position);
    l_wind->draw(m_proporties->m_sprite);
}

void TowerBase::Update(const float &l_dT)
{

}

UpgradeProporties* TowerBase::GetNextUpgradeProporties()
{
    if(m_currentUpgrade + 1 < m_proporties->m_upgrades.size()){
        return &m_proporties->m_upgrades[m_currentUpgrade + 1];
    } else return nullptr;
}
