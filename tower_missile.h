#ifndef TOWER_MISSILE_H
#define TOWER_MISSILE_H

#include "TowerBase.h"
#include "texturemanager.h"
#include <sstream>

struct TowerMissileProporties : public TowerProporties{
    ~TowerMissileProporties(){}
    void ReadIn(std::stringstream &keystream, TextureManager *mgr)
    {
        sf::IntRect rect;
        keystream >> m_missileTexture >> rect.left >> rect.top >> rect.width >> rect.height;
        mgr->RequireResource(m_missileTexture);
        m_missileSprite.setTexture(*mgr->GetResource(m_missileTexture));
        m_missileSprite.setTextureRect(rect);
        m_missileSprite.setOrigin(sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
    }

    std::string m_missileTexture;
    sf::Sprite m_missileSprite;
};

class Tower_Missile : public TowerBase
{
public:
    Tower_Missile(const std::shared_ptr<TowerProporties>& l_prop, TowerManager* l_towerManager);
    void Draw(sf::RenderWindow *l_wind);
    void Update(const float &l_dT);
    void Shot(const std::shared_ptr<EnemyBase> &l_enemy);
    void SetEnemy(const std::shared_ptr<EnemyBase> &l_enemy);
private:
    std::shared_ptr<TowerMissileProporties> m_missileProporties;
    bool m_justTargeted;
};

#endif // TOWER_MISSILE_H
