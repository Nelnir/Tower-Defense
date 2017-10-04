#include "tank_enemy.h"
#include "enemymanager.h"
#include "sharedcontext.h"
#include "texturemanager.h"
Tank_Enemy::Tank_Enemy(const std::shared_ptr<EnemyProporties>& l_proporties, EnemyManager *l_manager) : EnemyBase(l_proporties, l_manager),
    m_tankProporties(std::dynamic_pointer_cast<TankProporties>(l_proporties))
{

}

Tank_Enemy::~Tank_Enemy()
{

}

void Tank_Enemy::Draw(sf::RenderWindow *l_window)
{
    EnemyBase::Draw(l_window);
    m_tankProporties->m_tankSprite.setPosition(m_unique.m_position);
    m_tankProporties->m_tankSprite.setRotation(static_cast<int>(m_unique.m_direction) * 90);
    l_window->draw(m_tankProporties->m_tankSprite);
}
