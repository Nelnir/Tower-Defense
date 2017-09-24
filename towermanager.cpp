#include "towermanager.h"
#include "texturemanager.h"
#include "eventmanager.h"
#include "window.h"
#include "level.h"
#include "utils.h"

#include "normal_tower.h"

TowerManager::TowerManager(SharedContext* l_context, const float& l_zoom) : m_context(l_context), m_countId(0), m_placingTower(nullptr), m_colliding(false),
    m_zoom(l_zoom)
{
    RegisterProporties(TowerType::Neutral, CreateTowerProporties(TowerType::Neutral));
    RegisterTower<Normal_Tower>(TowerType::Neutral);
}

TowerManager::~TowerManager() { Purge(); }

TowerProporties* TowerManager::GetProporties(const TowerType &l_type)
{
    auto itr = m_towerProporties.find(l_type);
    return (itr == m_towerProporties.end() ? nullptr : itr->second);
}

TowerProporties* TowerManager::CreateTowerProporties(const TowerType &l_type)
{
    TowerProporties * proporties = new TowerProporties;
    switch(l_type)
    {
    case TowerType::Neutral:
        proporties->m_cost = 100;
        proporties->m_type = TowerType::Neutral;
        proporties->m_radiusCollision = 30.f;
        SetTextureForProporties("Tileset", sf::IntRect(1216, 640, 64, 64), proporties);
        UpgradeProporties upp;
        upp.m_cost = 0;
        upp.m_damage = 10;
        upp.m_firingRate = 1;
        upp.m_radius = 125;
        upp.m_shoots = 1;
        proporties->m_upgrades.emplace_back(upp);
        break;
    default:
        delete proporties;
        proporties = nullptr;
        break;
    }
    return proporties;
}

void TowerManager::SetTextureForProporties(const std::string &l_texture, const sf::IntRect &l_rect, TowerProporties *l_proporties)
{
    TextureManager* mgr = m_context->m_textureManager;
    mgr->RequireResource(l_texture);
    l_proporties->m_texture = l_texture;
    l_proporties->m_sprite.setTexture(*mgr->GetResource(l_texture));
    l_proporties->m_sprite.setTextureRect(l_rect);
    const sf::IntRect& size = l_proporties->m_sprite.getTextureRect();
    l_proporties->m_sprite.setOrigin(sf::Vector2f(size.width / 2.f, size.height / 2.f));
}

void TowerManager::Purge()
{
    TextureManager* mgr = m_context->m_textureManager;
    for(auto& itr : m_towerProporties){
        mgr->ReleaseResource(itr.second->m_texture);
        delete itr.second;
        itr.second = nullptr;
    }
    m_towerProporties.clear();
}

void TowerManager::Pressed(TowerProporties *l_proporties)
{
    m_placingTower = l_proporties;
}

void TowerManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_towers){
        itr.second->Draw(window);
    }

    if(m_placingTower){
        const sf::Vector2i mousePos = m_context->m_eventManager->GetMousePos(m_context->m_wind->getRenderWindow());
        const float& radiusC = m_placingTower->m_radiusCollision;
        const sf::Vector2f currPlacingTowerPos = sf::Vector2f(mousePos.x * m_zoom - radiusC, mousePos.y * m_zoom - radiusC);
        sf::CircleShape range;

        /// Check collison with other towers and draw thei collision circle
        bool collidingWithTowers = false;
        range.setFillColor(sf::Color(0, 0, 0, 128));
        for(auto& itr : m_towers){
            const float& radius = itr.second->GetProporties()->m_radiusCollision;
            range.setRadius(radius);
            sf::Vector2f pos = itr.second->GetPosition();
            range.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius));
            if(Utils::CirclesColliding(radius, range.getPosition(), radiusC, currPlacingTowerPos)){
                range.setFillColor(sf::Color(255, 0, 0, 128));
                collidingWithTowers = m_colliding = true;
            } else{
                range.setFillColor(sf::Color(0, 0, 0, 128));
            }
            window->draw(range);
        }

        const float& AttackRadius = m_placingTower->m_upgrades.front().m_radius;
        /// draw current placing tower attak circle
        range.setFillColor(sf::Color(0, 0, 0, 128));
        range.setRadius(AttackRadius);
        range.setPosition(sf::Vector2f(mousePos.x * m_zoom - AttackRadius, mousePos.y * m_zoom - AttackRadius));
        window->draw(range);

        /// draw current placing tower collision circle
        range.setRadius(radiusC);
        range.setPosition(currPlacingTowerPos);
        if(m_context->m_level->CollideWithPath(range)){ m_colliding = true; }
        else if (!collidingWithTowers){ m_colliding = false; }

        if(m_colliding){
            range.setFillColor(sf::Color(255, 0, 0, 128));
        } else {
            range.setFillColor(sf::Color(0, 255, 0, 128));
        }
        window->draw(range);

        /// draw current placing tower sprite
        m_placingTower->m_sprite.setPosition(sf::Vector2f(mousePos.x * m_zoom, mousePos.y * m_zoom));
        window->draw(m_placingTower->m_sprite);
    }
}

void TowerManager::Update(const float& l_dT)
{
    for(auto& itr : m_towers){
        itr.second->Update(l_dT, nullptr);
    }
}

void TowerManager::HandleRelease(EventDetails *l_details)
{
    if(l_details->m_name == "Mouse_Right_Release"){
        m_placingTower = nullptr;
    }
    if(!m_placingTower){
        return;
    }
    if(m_colliding){
        m_placingTower = nullptr;
        return;
    }
    m_context->m_level->SubtractMoney(m_placingTower->m_cost);
    AddTower(m_placingTower, m_placingTower->m_sprite.getPosition());
    m_placingTower = nullptr;
}

void TowerManager::HandleKey(EventDetails* l_details)
{
    if(l_details->m_name == "Key_ESC"){
        m_placingTower = nullptr;
    }
}

void TowerManager::AddTower(TowerProporties *l_proporties, const sf::Vector2f& l_pos)
{
    auto itr = m_towerFactory.find(l_proporties->m_type);
    if(itr == m_towerFactory.end()){
        return;
    }
    AbstractTower* tower = m_towers.emplace(m_countId++, itr->second(l_proporties)).first->second;
    tower->SetPosition(l_pos);
}
