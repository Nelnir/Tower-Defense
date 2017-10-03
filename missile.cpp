#include "missile.h"
#include <iostream>
#include "EnemyBase.h"
#include "bulletmanager.h"
#include "TowerBase.h"
#include "enemymanager.h"
Missile::Missile(BulletManager *l_manager, BulletProporties *m_proporties, std::shared_ptr<EnemyBase> m_enemy) :
    Bullet(l_manager, m_proporties, m_enemy), m_elapsed(0)
{
    UpdateDestination();
}

void Missile::Draw(sf::RenderWindow *l_window)
{
    Bullet::Draw(l_window);
}

void Missile::Update(const float &l_dT)
{
    m_elapsed += l_dT;
    m_unique.m_position.x += m_unique.m_tick.x * l_dT;
    m_unique.m_position.y += m_unique.m_tick.y * l_dT;
    m_unique.m_timeToHitTarget -= l_dT;
    if(m_unique.m_timeToHitTarget <= 0.f && m_unique.m_enemy){
        m_unique.m_enemy->OnBulletHit(this);
        m_manager->RemoveBullet(this);
    }
    UpdateDestination();
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
    sf::Vector2f diff = enemyPos - m_unique.m_position;
    bool xFurther = abs(diff.x) > abs(diff.y);
    m_unique.m_timeToHitTarget = (xFurther ? abs(diff.x) : abs(diff.y)) / m_speed;
    if(m_unique.m_timeToHitTarget < 0.03){
        m_unique.m_timeToHitTarget = 0.f;
    }
    m_unique.m_angle = static_cast<float>(atan2(m_unique.m_position.y - enemyPos.y, m_unique.m_position.x - enemyPos.x) * 180 / 3.141) - 90;

    if(m_elapsed < 0.1f){
        return;
    }
    m_elapsed = 0.f;

    if(xFurther){
        m_unique.m_tick.x = m_speed;
        m_unique.m_tick.y = m_speed / 2.f;
    } else{
        m_unique.m_tick.y = m_speed;
        m_unique.m_tick.x = m_speed / 2.f;
    }

    if(enemyPos.x < m_unique.m_position.x){ m_unique.m_tick.x *= -1; }
    if(enemyPos.y < m_unique.m_position.y){ m_unique.m_tick.y *= -1; }
}

void Missile::OnEnemyDeath()
{
    const auto& enemy = m_manager->GetContext()->m_enemyManager->GetFirstEnemy();
    if(enemy){
        m_unique.m_enemy = enemy;
        UpdateDestination();
    } else{

    }
}
