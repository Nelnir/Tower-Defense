#include "bulletmanager.h"
#include "window.h"
#include "EnemyBase.h"
#include "texturemanager.h"
#include <TowerBase.h>
#include "missile.h"
BulletManager::BulletManager(SharedContext* l_context) : m_context(l_context)
{
    RegisterProporties(BulletType::Normal, CreateProportiesFor(BulletType::Normal));
    RegisterProporties(BulletType::Missile, CreateProportiesFor(BulletType::Missile));
    RegisterBullet<Bullet>(BulletType::Normal);
    RegisterBullet<Missile>(BulletType::Missile);
}

BulletManager::~BulletManager()
{
    for(auto& itr : m_proporties){
        m_context->m_textureManager->ReleaseResource(itr.second->m_texture);
        delete itr.second;
    }
    m_proporties.clear();
    Purge();
}

void BulletManager::Purge()
{
    for(auto& itr : m_bullets){
        delete itr;
    }
    m_bullets.clear();
}

void BulletManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_bullets){
        itr->Draw(window);
    }
}

void BulletManager::Update(const float &l_dT)
{
    for(auto& itr : m_bullets){
        itr->Update(l_dT);
    }
}

void BulletManager::SpawnBullet(TowerBase *l_tower, const std::shared_ptr<EnemyBase>& l_enemy)
{
    auto itr = m_factory.find(l_tower->GetProporties()->m_bulletType);
    if(itr == m_factory.end()){
        return;
    }
    BulletProporties* proporties = GetProportiesFor(l_tower->GetProporties()->m_bulletType);
    if(!proporties){
        return;
    }
    m_bullets.emplace_back(itr->second(proporties, l_enemy));
    m_bullets.back()->Initialize(l_tower);
}

void BulletManager::EnemyRemoved(const std::shared_ptr<EnemyBase> &l_enemy)
{
    for(auto& itr : m_bullets){
        if(itr->GetEnemy() == l_enemy){
            itr->OnEnemyDeath();
        }
    }
}

BulletProporties* BulletManager::GetProportiesFor(const BulletType &l_type)
{
    auto itr = m_proporties.find(l_type);
    if(itr == m_proporties.end()){
        return nullptr;
    }
    return itr->second;
}

void BulletManager::RegisterProporties(const BulletType &l_type, BulletProporties *l_proporties)
{
    m_proporties[l_type] = l_proporties;
}

BulletProporties* BulletManager::CreateProportiesFor(const BulletType &l_type)
{
    BulletProporties* proporties = new BulletProporties;

    switch(l_type){
    case BulletType::Normal:
        proporties->m_type = BulletType::Normal;
        SetTextureForProporties("Tileset", sf::IntRect(1216, 768, 64, 64), proporties);
        return proporties;
        break;
    case BulletType::Missile:
        proporties->m_type == BulletType::Missile;
        SetTextureForProporties("Tileset", sf::IntRect(1410, 640, 64, 64), proporties);
        return proporties;
        break;
    }
    delete proporties;
    return nullptr;
}

void BulletManager::RemoveBullet(Bullet* l_bullet)
{
    if(std::find(m_toRemove.begin(), m_toRemove.end(), l_bullet) != m_toRemove.end()){
        return;
    }
    m_toRemove.push_back(l_bullet);
}

void BulletManager::ProcessRequests()
{
    while(!m_toRemove.empty()){
        m_bullets.erase(std::find(m_bullets.begin(), m_bullets.end(), m_toRemove.back()));
        delete m_toRemove.back();
        m_toRemove.pop_back();
    }
}

void BulletManager::SetTextureForProporties(const std::string &l_texture, const sf::IntRect &l_rect, BulletProporties *l_proporties)
{
    TextureManager* mgr = m_context->m_textureManager;
    mgr->RequireResource(l_texture);
    l_proporties->m_texture = l_texture;
    l_proporties->m_sprite.setTexture(*mgr->GetResource(l_texture));
    l_proporties->m_sprite.setTextureRect(l_rect);
    l_proporties->m_sprite.setOrigin(sf::Vector2f(l_rect.width / 2.f, l_rect.height / 2.f));
}
