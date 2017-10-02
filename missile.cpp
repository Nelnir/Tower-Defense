#include "missile.h"
#include <iostream>
#include "EnemyBase.h"
#include "bulletmanager.h"
#include "TowerBase.h"
Missile::Missile(BulletManager *l_manager, BulletProporties *m_proporties, std::shared_ptr<EnemyBase> m_enemy) :
    Bullet(l_manager, m_proporties, m_enemy)
{

}

void Missile::Draw(sf::RenderWindow *l_window)
{
    Bullet::Draw(l_window);
}

void Missile::Update(const float &l_dT)
{
    UpdateDestination();
    m_unique.m_position.x += m_unique.m_tick.x * l_dT;
    m_unique.m_position.y += m_unique.m_tick.y * l_dT;
    m_unique.m_timeToHitTarget -= l_dT;
    if(m_unique.m_timeToHitTarget <= 0.f && m_unique.m_enemy){
        m_unique.m_enemy->OnBulletHit(this);
        m_manager->RemoveBullet(this);
    }
}

void Missile::Initialize(TowerBase *l_tower)
{
    m_unique.m_damage = l_tower->GetUpgradeProporties().m_damage;
    m_unique.m_angle = l_tower->GetRotation() - 90;
    m_unique.m_position = l_tower->GetPosition();
    m_speed = l_tower->GetUpgradeProporties().m_bulletSpeed;
    UpdateDestination();
}

void Missile::UpdateDestination()
{
    const auto& enemyPos = m_unique.m_enemy->GetProporties()->m_position;
    m_unique.m_angle = static_cast<float>(atan2(m_unique.m_position.y - enemyPos.y, m_unique.m_position.x - enemyPos.x) * 180 / 3.141) - 90;
    m_unique.m_tick = enemyPos - m_unique.m_position;
    m_unique.m_timeToHitTarget = (abs(m_unique.m_tick.x) > abs(m_unique.m_tick.y) ? abs(m_unique.m_tick.x) : abs(m_unique.m_tick.y)) / m_speed;

    m_unique.m_tick.x = m_speed;
    m_unique.m_tick.y = m_speed;
    if(enemyPos.x < m_unique.m_position.x){ m_unique.m_tick.x *= -1; }
    if(enemyPos.y < m_unique.m_position.y){ m_unique.m_tick.y *= -1; }
}
