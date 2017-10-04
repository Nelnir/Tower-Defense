#include "tower_missile.h"

Tower_Missile::Tower_Missile(const std::shared_ptr<TowerProporties> &l_prop, TowerManager *l_towerManager) : TowerBase(l_prop, l_towerManager),
  m_missileProporties(std::dynamic_pointer_cast<TowerMissileProporties>(l_prop)), m_justTargeted(false)
{

}

void Tower_Missile::Draw(sf::RenderWindow *l_wind)
{
    if(m_lookinAt && !m_justTargeted){
        RotateToEnemy(m_lookinAt);
        m_missileProporties->m_missileSprite.setPosition(m_position);
        m_missileProporties->m_missileSprite.setRotation(m_angle);
        l_wind->draw(m_missileProporties->m_missileSprite);
    } else{
        TowerBase::Draw(l_wind);
    }
}

void Tower_Missile::Update(const float &l_dT)
{

}

void Tower_Missile::Shot(const std::shared_ptr<EnemyBase> &l_enemy)
{
    m_justTargeted = false;
    TowerBase::Shot(l_enemy);
}

void Tower_Missile::SetEnemy(const std::shared_ptr<EnemyBase> &l_enemy)
{
    if(!m_lookinAt){
        m_justTargeted = true;
    }
    TowerBase::SetEnemy(l_enemy);
}
