#include "EnemyBase.h"
#include "enemymanager.h"
#include "bullet.h"
#include "level.h"
#include "statistics.h"
#include <iostream>

EnemyBase::~EnemyBase()
{

}

void EnemyBase::Update(const float &l_dT)
{
    switch(m_unique.m_direction){
    case MoveDirection::Left:
        m_unique.m_position.x -= m_unique.m_speed * l_dT;
        if(m_unique.m_position.x < m_unique.m_destination.x){
            m_unique.m_position.x = m_unique.m_destination.x;
            SetDestination(m_enemyManager->GiveNextWaypoint(this));
        }
        break;
    case MoveDirection::Right:
        m_unique.m_position.x += m_unique.m_speed * l_dT;
        if(m_unique.m_position.x > m_unique.m_destination.x){
            m_unique.m_position.x = m_unique.m_destination.x;
            SetDestination(m_enemyManager->GiveNextWaypoint(this));
        }
        break;
    case MoveDirection::Up:
        m_unique.m_position.y -= m_unique.m_speed * l_dT;
        if(m_unique.m_position.y < m_unique.m_destination.y){
            m_unique.m_position.y = m_unique.m_destination.y;
            SetDestination(m_enemyManager->GiveNextWaypoint(this));
        }
        break;
    case MoveDirection::Down:
        m_unique.m_position.y += m_unique.m_speed * l_dT;
        if(m_unique.m_position.y > m_unique.m_destination.y){
            m_unique.m_position.y = m_unique.m_destination.y;
            SetDestination(m_enemyManager->GiveNextWaypoint(this));
        }
        break;
    }
}

void EnemyBase::Draw(sf::RenderWindow *l_window)
{
    m_proporties->m_sprite.setPosition(m_unique.m_position);
    m_proporties->m_sprite.setRotation(static_cast<int>(m_unique.m_direction) * 90);
    l_window->draw(m_proporties->m_sprite);
}

void EnemyBase::OnBulletHit(Bullet *l_bullet)
{
    if(m_unique.m_hp <= 0){ // if previous bullet attacked it, and it wasnt deleted yet
        return;
    }

    m_unique.m_hp -= l_bullet->GetDamage();
    if(m_unique.m_hp <= 0){
        m_enemyManager->RemoveEnemy(this);

        Statistics* statistics = m_enemyManager->GetContext()->m_statistics;
        statistics->AddEnemiesWithTypeKilled(m_proporties->m_enemy);
        statistics->AddEnemiesKilled();
        statistics->AddMoneyEarned(m_proporties->m_money);
        m_enemyManager->GetContext()->m_level->AddMoney(m_proporties->m_money);
    }
    m_enemyManager->GetContext()->m_statistics->AddDmgDealt(l_bullet->GetDamage());
}

void EnemyBase::SetDestination(const sf::Vector2f &l_pos)
{
    if(l_pos.x == -1 && l_pos.y == 1){
        return;
    }
    ++m_unique.m_waypoint;
    m_unique.m_destination = l_pos;
    if(l_pos.x == m_unique.m_position.x){
        if(l_pos.y > m_unique.m_position.y){
            m_unique.m_direction = MoveDirection::Down;
        } else m_unique.m_direction = MoveDirection::Up;
    } else{
        if(l_pos.x > m_unique.m_position.x){
            m_unique.m_direction = MoveDirection::Right;
        } else m_unique.m_direction = MoveDirection::Left;
    }
}

sf::Vector2f EnemyBase::GetPositionAfter(const float &l_time, const bool& l_recurency)
{
    sf::Vector2f pos;
    bool recurrency = false;
    float subtract = 0.f;
    switch (m_unique.m_direction) {
    case MoveDirection::Up:
        pos = sf::Vector2f(m_unique.m_position.x, m_unique.m_position.y - m_unique.m_speed * l_time);
        if(pos.y < m_unique.m_destination.y){
            pos.y = m_unique.m_destination.y;
            subtract = (abs(m_unique.m_destination.y - m_unique.m_position.y)) / m_unique.m_speed;
            recurrency = true;
        }
        break;
    case MoveDirection::Down:
        pos = sf::Vector2f(m_unique.m_position.x, m_unique.m_position.y + m_unique.m_speed * l_time);
        if(pos.y > m_unique.m_destination.y){
            pos.y = m_unique.m_destination.y;
            subtract = (abs(m_unique.m_destination.y - m_unique.m_position.y)) / m_unique.m_speed;
            recurrency = true;
        }
        break;
    case MoveDirection::Right:
        pos = sf::Vector2f(m_unique.m_position.x + m_unique.m_speed * l_time, m_unique.m_position.y);
        if(pos.x > m_unique.m_destination.x){
            pos.x = m_unique.m_destination.x;
            subtract = (abs(m_unique.m_destination.x - m_unique.m_position.x)) / m_unique.m_speed;
            recurrency = true;
        }
        break;
    case MoveDirection::Left:
        pos = sf::Vector2f(m_unique.m_position.x - m_unique.m_speed * l_time, m_unique.m_position.y);
        if(pos.x < m_unique.m_destination.x){
            pos.x = m_unique.m_destination.x;
            subtract = (abs(m_unique.m_destination.x - m_unique.m_position.x)) / m_unique.m_speed;
            recurrency = true;
        }
        break;
    }


    if(recurrency){
        int copyway = m_unique.m_waypoint;
        MoveDirection copy = m_unique.m_direction;
        sf::Vector2f copyPos = m_unique.m_position;
        sf::Vector2f copyDes = m_unique.m_destination;

        m_unique.m_position = pos;
        sf::Vector2f l_pos = m_enemyManager->GiveNextWaypoint(this);
        SetDestination(l_pos);
        pos = GetPositionAfter(l_time - subtract, true);

        if(!l_recurency){
            m_unique.m_position = copyPos;
            m_unique.m_waypoint = copyway;
            m_unique.m_direction = copy;
            m_unique.m_destination = copyDes;
        }
    }
    return pos;
}
