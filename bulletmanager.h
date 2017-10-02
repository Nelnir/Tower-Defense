#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <unordered_map>
#include "sharedcontext.h"
#include "bullet.h"
#include <SFML/Graphics.hpp>
#include <functional>
class EnemyBase;

using Bullets = std::vector<Bullet*>;
using BulletsToRemove = std::vector<Bullet*>;
using BulletsProporties = std::unordered_map<BulletType, BulletProporties*>;
using BulletFactory = std::unordered_map<BulletType, std::function<Bullet*(BulletProporties*, std::shared_ptr<EnemyBase>)>>;

class TowerBase;
class BulletManager
{
public:
    BulletManager(SharedContext* l_context);
    ~BulletManager();
    void Purge();
    void Update(const float& l_dT);
    void Draw();
    void ProcessRequests();
    void RemoveBullet(Bullet* l_bullet);
    void SpawnBullet(TowerBase* l_tower, const std::shared_ptr<EnemyBase>& l_enemy);
    BulletProporties* GetProportiesFor(const BulletType& l_type);
private:
    BulletProporties* CreateProportiesFor(const BulletType& l_type);
    template <class T>
    void RegisterBullet(const BulletType& l_type){
        m_factory[l_type] = [this](BulletProporties* l_proporties, std::shared_ptr<EnemyBase> l_enemy) -> Bullet*
        {
            return new T(this, l_proporties, l_enemy);
        };
    }
    void RegisterProporties(const BulletType& l_type, BulletProporties* l_proporties);
    void SetTextureForProporties(const std::string &l_texture, const sf::IntRect &l_rect, BulletProporties *l_proporties);

    SharedContext* m_context;
    Bullets m_bullets;
    BulletFactory m_factory;
    BulletsProporties m_proporties;
    BulletsToRemove m_toRemove;
};

#endif // BULLETMANAGER_H
