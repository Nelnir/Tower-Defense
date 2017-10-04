#ifndef TANK_ENEMY_H
#define TANK_ENEMY_H

#include "EnemyBase.h"
#include "texturemanager.h"

struct TankProporties : public EnemyProporties{
    ~TankProporties() {}
    void ReadIn(std::stringstream &keystream, TextureManager* mgr)
    {
        sf::IntRect rect;
        keystream >> m_tankTexture >> rect.left >> rect.top >> rect.width >> rect.height;
        mgr->RequireResource(m_tankTexture);
        m_tankSprite.setTexture(*mgr->GetResource(m_tankTexture));
        m_tankSprite.setTextureRect(rect);
        m_tankSprite.setColor(m_sprite.getColor());
        m_tankSprite.setOrigin(sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
    }

    std::string m_tankTexture;
    sf::Sprite m_tankSprite;
};

class Tank_Enemy : public EnemyBase
{
public:
    Tank_Enemy(const std::shared_ptr<EnemyProporties>& l_proporties, EnemyManager* l_manager);
    ~Tank_Enemy();
    void Draw(sf::RenderWindow *l_window);
    std::shared_ptr<TankProporties> m_tankProporties;
};

#endif // TANK_ENEMY_H
