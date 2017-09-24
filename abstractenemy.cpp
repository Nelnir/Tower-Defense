#include "abstractenemy.h"
#include "enemymanager.h"
#include <iostream>
void AbstractEnemy::Update(const float &l_dT)
{
    switch(m_unique.m_direction){
    case MoveDirection::Left:
        m_unique.m_position.x -= m_unique.m_speed * l_dT;
        if(m_unique.m_position.x < m_unique.m_destionation.x){
            m_unique.m_position.x = m_unique.m_destionation.x;
            m_enemyManager->GiveNextWaypoint(this);
        }
        break;
    case MoveDirection::Right:
        m_unique.m_position.x += m_unique.m_speed * l_dT;
        if(m_unique.m_position.x > m_unique.m_destionation.x){
            m_unique.m_position.x = m_unique.m_destionation.x;
            m_enemyManager->GiveNextWaypoint(this);
        }
        break;
    case MoveDirection::Up:
        m_unique.m_position.y -= m_unique.m_speed * l_dT;
        if(m_unique.m_position.y < m_unique.m_destionation.y){
            m_unique.m_position.y = m_unique.m_destionation.y;
            m_enemyManager->GiveNextWaypoint(this);
        }
        break;
    case MoveDirection::Down:
        m_unique.m_position.y += m_unique.m_speed * l_dT;
        if(m_unique.m_position.y > m_unique.m_destionation.y){
            m_unique.m_position.y = m_unique.m_destionation.y;
            m_enemyManager->GiveNextWaypoint(this);
        }
        break;
    }
}

void AbstractEnemy::Draw(sf::RenderWindow *l_window)
{
    m_proporties->m_sprite.setPosition(m_unique.m_position);
    m_proporties->m_sprite.setRotation(static_cast<int>(m_unique.m_direction) * 90);
    l_window->draw(m_proporties->m_sprite);
}

void AbstractEnemy::SetDestination(const sf::Vector2f &l_pos)
{
    ++m_unique.m_waypoint;
    m_unique.m_destionation = l_pos;
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
