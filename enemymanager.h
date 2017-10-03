#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "sharedcontext.h"
#include "EnemyBase.h"
#include "statistics.h"
#include <unordered_map>
#include <functional>
#include <memory>

using Enemies = std::vector<std::shared_ptr<EnemyBase>>;
using EnemyFactory = std::unordered_map<Enemy, std::function<EnemyBase*(EnemyProporties*)>>;
using EnemiesProporties = std::unordered_map<Enemy, std::unordered_map<EnemyId, EnemyProporties*>>;
using EnemiesToRemove = std::vector<std::shared_ptr<EnemyBase>>;

class TowerBase;
class EnemyManager
{
public:
    EnemyManager(SharedContext* l_shared, Statistics* l_statistics);
    ~EnemyManager();

    void SpawnEnemy(const Enemy& l_enemy, const EnemyId& l_id, const sf::Vector2f& l_pos, const sf::Vector2f& l_destination);
    SharedContext* GetContext() { return m_context; }

    void Draw();
    void Update(const float& l_dT);

    sf::Vector2f GiveNextWaypoint(const std::shared_ptr<EnemyBase>& l_enemy);
    sf::Vector2f GiveNextWaypoint(EnemyBase* l_enemy);
    void ProcessRequests();
    void Restart();
    std::shared_ptr<EnemyBase> GetEnemyFor(const std::shared_ptr<TowerBase>& l_tower);
    std::shared_ptr<EnemyBase> GetFirstEnemy() { if(!m_sorted.empty()) return m_sorted.front(); else return nullptr; }
    void RemoveEnemy(const std::shared_ptr<EnemyBase>& l_enemy);

private:
    void Sort();
    void Purge();
    void LoadProporties(const Enemy& l_enemy, const EnemyId& l_id, const std::string& l_file);
    void LoadConfigFile(const std::string& l_file);
    template<class T>
    void RegisterEnemy(const Enemy& l_type){
        m_factory[l_type] = [this](EnemyProporties* l_proporties) -> EnemyBase* {
            return new T(l_proporties, this);
        };
    }

    SharedContext* m_context;
    EnemyFactory m_factory;
    Enemies m_enemies;
    Enemies m_sorted; /// getting enemies for towers is faster
    EnemiesProporties m_enemyProporties;
    std::unordered_map<Enemy, std::unordered_map<EnemyId, std::string>> m_pathProporties;
    EnemiesToRemove m_toRemove;

    Statistics* m_statistics;
};

#endif // ENEMYMANAGER_H
