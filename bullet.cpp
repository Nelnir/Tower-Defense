#include "bullet.h"
#include "EnemyBase.h"
#include "TowerBase.h"
#include "bulletmanager.h"
#include <iostream>
Bullet::Bullet(BulletManager* l_manager, BulletProporties* l_proporties, std::shared_ptr<EnemyBase> l_enemy) :
    m_manager(l_manager),
    m_proporties(l_proporties),
    m_unique(l_enemy)
{

}


void Bullet::Draw(sf::RenderWindow *l_window)
{
    m_proporties->m_sprite.setPosition(m_unique.m_position);
    m_proporties->m_sprite.setRotation(m_unique.m_angle);
    l_window->draw(m_proporties->m_sprite);
}

void Bullet::Update(const float &l_dT)
{
    m_unique.m_position.x += m_unique.m_tick.x * l_dT;
    m_unique.m_position.y += m_unique.m_tick.y * l_dT;
    m_unique.m_timeToHitTarget -= l_dT;
    if(m_unique.m_timeToHitTarget <= 0.f && m_unique.m_enemy){
        m_unique.m_enemy->OnBulletHit(this);
        m_manager->RemoveBullet(this);
    }
}

void Bullet::Initialize(TowerBase* l_tower)
{
    if(l_tower){
        sf::Vector2f enemyPos = m_unique.m_enemy->GetProporties()->m_position;
        m_unique.m_position = l_tower->GetPosition();
        m_unique.m_tick = enemyPos - m_unique.m_position;
        float seconds = abs((abs(m_unique.m_tick.x) > abs(m_unique.m_tick.y) ? abs(m_unique.m_tick.x) : abs(m_unique.m_tick.y))) / l_tower->GetUpgradeProporties().m_bulletSpeed;

        enemyPos = m_unique.m_enemy->GetPositionAfter(seconds);

        m_unique.m_tick = enemyPos - m_unique.m_position;
        seconds = abs((abs(m_unique.m_tick.x) > abs(m_unique.m_tick.y) ? m_unique.m_tick.x : m_unique.m_tick.y))/ l_tower->GetUpgradeProporties().m_bulletSpeed;
        m_unique.m_timeToHitTarget = seconds;
        m_unique.m_tick.x /= seconds;
        m_unique.m_tick.y /= seconds;
        m_unique.m_damage = l_tower->GetUpgradeProporties().m_damage;
        m_unique.m_angle = l_tower->GetRotation() + 180;
    }
}
