#ifndef ABSTRACTTOWER_H
#define ABSTRACTTOWER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "bullet.h"

struct UpgradeProporties{
    int m_damage;
    float m_firingRate; /// shoots per second
    float m_radius; /// range
    unsigned int m_shoots; /// multiple shots maybe?
    unsigned int m_cost; /// cost to upgrade to this lvl
    float m_bulletSpeed; /// seconds to hit the enemy
};

using Upgrades = std::vector<UpgradeProporties>;

enum class AttackStrategy { First, Last };
enum class Tower { Basic };
enum class TowerType { Land, Air };

struct TowerProporties{ /// proporties which are the same to all towers X
    TowerProporties() : m_cost(0), m_tower(Tower::Basic) { }

    int m_cost;
    Upgrades m_upgrades;
    std::string m_texture;
    sf::Sprite m_sprite;
    Tower m_tower;
    TowerType m_type;
    float m_radiusCollision;
    bool m_towerRotation;
    BulletType m_bulletType;
};

class TowerManager;
class EnemyBase;
class AbstractTower
{
    friend class TowerManager;
public:
    AbstractTower(TowerProporties* l_proporties, TowerManager* l_towerManager);

    virtual void Draw(sf::RenderWindow* l_wind) = 0;
    virtual void Update(const float& l_dT) = 0;

    UpgradeProporties& GetUpgradeProporties() { return m_proporties->m_upgrades[m_currentUpgrade]; }
    TowerProporties* GetProporties() { return m_proporties; }
    AttackStrategy GetStrategy() { return m_strategy; }
    sf::Vector2f GetPosition() { return m_position; }

    void SetPosition(const sf::Vector2f& l_pos) { m_position = l_pos; }
    void SetEnemy(const std::shared_ptr<EnemyBase>& l_enemy){ if(m_lookinAt != l_enemy) m_lookinAt = l_enemy; }
protected:
    virtual void Shot(const std::shared_ptr<EnemyBase>& l_enemy) = 0;
    void RotateToEnemy(const std::shared_ptr<EnemyBase>& l_enemy);
    TowerProporties* m_proporties;
    TowerManager* m_towerManager;

    std::shared_ptr<EnemyBase> m_lookinAt;
    unsigned int m_currentUpgrade;
    AttackStrategy m_strategy;
    sf::Vector2f m_position;
    float m_angle;
private:
    float m_shootElapsed;
};

#endif // ABSTRACTTOWER_H
