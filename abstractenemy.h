#ifndef ABSTRACTENEMY_H
#define ABSTRACTENEMY_H

#include <SFML/Graphics.hpp>

enum class Enemy { Soldier = 0};
enum class EnemyType { Normal = 0};

struct twoFloats{
    float x, y;
};

struct EnemyProporties{
    Enemy m_enemy;
    EnemyType m_type;
    float m_baseSpeed;
    int m_baseHp;
    std::string m_texture;
    sf::Sprite m_sprite;
    int m_lifeTakes; /// how much life it takes when it pass to the end
    int m_money; /// how much money this monster give when killed
    union{
        twoFloats m_size;
        float m_radius;
    };
};

enum class MoveDirection { Right = 0, Down, Left, Up };

struct Proporties{
    Proporties() : m_waypoint(0) {}
    float m_speed;
    int m_hp;
    sf::Vector2f m_position;
    sf::Vector2f m_destionation;
    MoveDirection m_direction;
    int m_waypoint;
    int m_id;
};

class EnemyManager;
class AbstractEnemy
{
    friend class EnemyManager;
public:
    AbstractEnemy(EnemyProporties* l_proporties, EnemyManager* l_mgr) : m_enemyManager(l_mgr){
        m_proporties = l_proporties;
        m_unique.m_hp = l_proporties->m_baseHp;
        m_unique.m_speed = l_proporties->m_baseSpeed;
    }
    virtual ~AbstractEnemy() {}

    virtual void Update(const float& l_dT);

    virtual void Draw(sf::RenderWindow* l_window);

    Proporties* GetProporties() { return &m_unique; }

    void SetPosition(const sf::Vector2f& l_pos) { m_unique.m_position = l_pos; }
    void SetDestination(const sf::Vector2f& l_pos);

protected:
    EnemyManager* m_enemyManager;
    EnemyProporties* m_proporties;
    Proporties m_unique;
};

#endif // ABSTRACTENEMY_H
