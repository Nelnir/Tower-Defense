#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <memory>

enum class BulletType { Normal = 0, Missile };

struct BulletProporties{
    std::string m_texture;
    sf::Sprite m_sprite;
    BulletType m_type;
};

class EnemyBase;
class TowerBase;

struct Unique{
    Unique(std::shared_ptr<EnemyBase> l_enemy) : m_angle(0), m_damage(0), m_enemy(l_enemy) {}
    sf::Vector2f m_position;
    float m_angle;
    int m_damage;
    std::shared_ptr<EnemyBase> m_enemy;
    sf::Vector2f m_tick;
    float m_timeToHitTarget;
};

class BulletManager;
class Bullet
{
    friend class BulletManager;
public:
    Bullet(BulletManager* l_manager, BulletProporties* m_proporties, const std::shared_ptr<EnemyBase>& m_enemy);
    virtual void Draw(sf::RenderWindow* l_window);
    virtual void Update(const float& l_dT);
    virtual void OnEnemyDeath();
    const std::shared_ptr<EnemyBase>& GetEnemy() { return m_unique.m_enemy; }
    const int& GetDamage() { return m_unique.m_damage; }
protected:
    virtual void Initialize(TowerBase* l_tower = nullptr);
    BulletManager* m_manager;
    BulletProporties* m_proporties;
    Unique m_unique;
};

#endif // BULLET_H
