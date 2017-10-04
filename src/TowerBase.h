#ifndef ABSTRACTTOWER_H
#define ABSTRACTTOWER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "bullet.h"
#include <bitset>
#include "EnemyBase.h"
#include <iostream>

struct UpgradeProporties{
    int m_damage;
    float m_firingRate; /// shoots per second
    float m_radius; /// range
    int m_cost; /// cost to upgrade to this lvl
    float m_bulletSpeed; /// seconds to hit the enemy
};

using Upgrades = std::vector<UpgradeProporties>;

enum class AttackStrategy { First, Last };
enum class Tower { Basic = 0, BasicPRO = 2, Missile = 4, MissilePRO = 6}; // if %2 is equal to 0, its circle collision otherwise aabb

struct TowerProporties{ /// proporties which are the same to all towers X
    TowerProporties() : m_cost(0), m_tower(Tower::Basic) { }
    virtual ~TowerProporties() {}
    virtual void ReadIn(std::stringstream& keystream, TextureManager* mgr) {}
    int m_cost;
    Upgrades m_upgrades;
    std::string m_texture;
    sf::Sprite m_sprite;
    Tower m_tower;
    std::bitset<static_cast<int>(EnemyType::Count)> m_attacking;
    bool m_towerRotation;
    float m_towerRotationOffset;
    BulletType m_bulletType;
    union{
        twoFloats m_size;
        float m_radiusCollision;
    };
};

class TowerManager;
class EnemyBase;
class TowerBase
{
    friend class TowerManager;
public:
    TowerBase(const std::shared_ptr<TowerProporties>& l_proporties, TowerManager* l_towerManager);

    virtual void Draw(sf::RenderWindow* l_wind);
    virtual void Update(const float& l_dT);

    const UpgradeProporties& GetUpgradeProporties() { return m_proporties->m_upgrades[m_currentUpgrade]; }
    const std::shared_ptr<TowerProporties>& GetProporties() { return m_proporties; }
    UpgradeProporties* GetNextUpgradeProporties();
    AttackStrategy GetStrategy() { return m_strategy; }
    sf::Vector2f GetPosition() { return m_position; }
    unsigned int GetCurrentUpgrade() { return m_currentUpgrade; }
    unsigned int GetNumberOfUpgrades() { return m_proporties->m_upgrades.size(); }

    void SetStrategy(const AttackStrategy& l_strategy) { m_strategy = l_strategy; }
    void SetPosition(const sf::Vector2f& l_pos) { m_position = l_pos; }
    virtual void SetEnemy(const std::shared_ptr<EnemyBase>& l_enemy){ if(m_lookinAt != l_enemy) m_lookinAt = l_enemy; }
    void Upgrade() { ++m_currentUpgrade; }
    float GetRotation() { return m_angle; }
protected:
    virtual void Shot(const std::shared_ptr<EnemyBase>& l_enemy);
    void RotateToEnemy(const std::shared_ptr<EnemyBase>& l_enemy);
    std::shared_ptr<TowerProporties> m_proporties;
    TowerManager* m_towerManager;

    std::shared_ptr<EnemyBase> m_lookinAt;
    unsigned int m_currentUpgrade;
    AttackStrategy m_strategy;
    sf::Vector2f m_position;
    float m_angle;
    float m_shootElapsed;
};

#endif // ABSTRACTTOWER_H
