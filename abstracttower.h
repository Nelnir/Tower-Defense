#ifndef ABSTRACTTOWER_H
#define ABSTRACTTOWER_H

#include <vector>
#include <SFML/Graphics.hpp>

struct UpgradeProporties{
    int m_damage;
    float m_firingRate; /// shoots per second
    float m_radius; /// range
    unsigned int m_shoots; /// multiple shots maybe?
    unsigned int m_cost; /// cost to upgrade to this lvl
};

using Upgrades = std::vector<UpgradeProporties>;

enum class AttackStrategy { First, Last };
enum class TowerType { Neutral };

struct TowerProporties{ /// proporties which are the same to all towers of type X
    TowerProporties() : m_cost(0), m_type(TowerType::Neutral) { }

    int m_cost;
    Upgrades m_upgrades;
    std::string m_texture;
    sf::Sprite m_sprite;
    TowerType m_type;
    float m_radiusCollision;
};

class TowerManager;
class AbstractEnemy;
class AbstractTower
{
    friend class TowerManager;
public:
    AbstractTower(TowerProporties* l_proporties, TowerManager* l_towerManager) : m_proporties(l_proporties), m_towerManager(l_towerManager) {}

    virtual void Draw(sf::RenderWindow* l_wind) = 0;
    virtual void Update(const float& l_dT, AbstractEnemy* l_lookinAt) = 0;

    TowerProporties* GetProporties() { return m_proporties; }
    AttackStrategy GetStrategy() { return m_strategy; }
    sf::Vector2f GetPosition() { return m_position; }

    void SetPosition(const sf::Vector2f& l_pos) { m_position = l_pos; }
protected:
    virtual void Shot(AbstractEnemy* l_enemy) = 0;

    unsigned int m_currentUpgrade;
    AttackStrategy m_strategy;
    TowerProporties* m_proporties;
    sf::Vector2f m_position;

    TowerManager* m_towerManager;
};

#endif // ABSTRACTTOWER_H
