#include "soldierenemy.h"
#include <iostream>

SoldierEnemy::SoldierEnemy(EnemyProporties *l_prop, EnemyManager *l_mgr) : AbstractEnemy(l_prop, l_mgr)
{

}

SoldierEnemy::~SoldierEnemy() {}

void SoldierEnemy::Draw(sf::RenderWindow *l_window)
{
    AbstractEnemy::Draw(l_window);
}

void SoldierEnemy::Update(const float &l_dT)
{
    AbstractEnemy::Update(l_dT);
}
