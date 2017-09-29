#include "bullet.h"
#include "abstractenemy.h"
#include "abstracttower.h"
#include "bulletmanager.h"
#include <iostream>
Bullet::Bullet(BulletManager* l_manager, BulletProporties* l_proporties, std::shared_ptr<EnemyBase> l_enemy, AbstractTower* l_tower) :
    m_manager(l_manager),
    m_proporties(l_proporties),
    m_unique(l_enemy)
{
    Initialize(l_tower);
}


void Bullet::Draw(sf::RenderWindow *l_window)
{
    m_proporties->m_sprite.setPosition(m_unique.m_position);
    m_proporties->m_sprite.setRotation(m_unique.m_angle);
    l_window->draw(m_proporties->m_sprite);
}

void Bullet::Update(const float &l_dT)
{
    m_unique.m_position.x += m_unique.m_tick.x * l_dT / m_unique.m_baseTimeToHitTarget;
    m_unique.m_position.y += m_unique.m_tick.y * l_dT / m_unique.m_baseTimeToHitTarget;
    m_unique.m_timeToHitTarget -= l_dT;
    if(m_unique.m_timeToHitTarget <= 0.f && m_unique.m_enemy){
        m_unique.m_enemy->OnBulletHit(this);
        m_manager->RemoveBullet(this);
    }
}

void Bullet::Initialize(AbstractTower* l_tower)
{
    if(l_tower){
        sf::Vector2f pos = m_unique.m_enemy->GetProporties()->m_position;
        m_unique.m_tick = pos - l_tower->GetPosition();
        float seconds = abs((m_unique.m_tick.x > m_unique.m_tick.y ? m_unique.m_tick.x : m_unique.m_tick.y ) / l_tower->GetUpgradeProporties().m_bulletSpeed);

        pos = m_unique.m_enemy->GetPositionAfter(seconds);
        m_unique.m_tick = pos - l_tower->GetPosition();
        seconds = abs((m_unique.m_tick.x > m_unique.m_tick.y ? m_unique.m_tick.x : m_unique.m_tick.y ) / l_tower->GetUpgradeProporties().m_bulletSpeed);
        m_unique.m_timeToHitTarget = m_unique.m_baseTimeToHitTarget = seconds;
        m_unique.m_damage = l_tower->GetUpgradeProporties().m_damage;
        m_unique.m_position = l_tower->GetPosition();
        m_unique.m_angle = static_cast<float>(atan2(m_unique.m_position.y - pos.y, m_unique.m_position.x - pos.x) * 180 / 3.141);
        m_unique.m_angle += 90;
    }
}
