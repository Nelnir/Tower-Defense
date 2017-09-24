#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "sharedcontext.h"
#include "abstractenemy.h"
#include <unordered_map>
#include <functional>

using EnemyId = unsigned int;
using Enemies = std::unordered_map<EnemyId, AbstractEnemy*>;
using EnemyFactory = std::unordered_map<Enemy, std::function<AbstractEnemy*(EnemyProporties*)>>;
using EnemiesProporties = std::unordered_map<Enemy, EnemyProporties*>;
using EnemiesToRemove = std::list<EnemyId>;

class EnemyManager
{
public:
    EnemyManager(SharedContext* l_shared);
    ~EnemyManager();

    void SpawnEnemy(const Enemy& l_enemy, const sf::Vector2f& l_pos, const sf::Vector2f& l_destination);
    SharedContext* GetContext() { return m_context; }

    void Draw();
    void Update(const float& l_dT);

    void GiveNextWaypoint(AbstractEnemy* l_enemy);
    void ProcessRequests();
private:
    void Purge();
    void LoadProporties(const Enemy& l_enemy, const std::string& l_file);
    void LoadConfigFile(const std::string& l_file);
    template<class T>
    void RegisterEnemy(const Enemy& l_type){
        m_factory[l_type] = [this](EnemyProporties* l_proporties) -> AbstractEnemy* {
            return new T(l_proporties, this);
        };
    }

    SharedContext* m_context;
    EnemyFactory m_factory;
    Enemies m_enemies;
    EnemiesProporties m_enemyProporties;
    EnemyId m_enemyCount;
    std::unordered_map<Enemy, std::string> m_pathProporties;
    EnemiesToRemove m_toRemove;
};

#endif // ENEMYMANAGER_H
