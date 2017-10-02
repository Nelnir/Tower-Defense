#include "enemymanager.h"
#include <iostream>
#include <fstream>
#include "utils.h"
#include "window.h"
#include "level.h"
#include "TowerBase.h"

#include "texturemanager.h"
#include "tank_enemy.h"

EnemyManager::EnemyManager(SharedContext* l_context, Statistics* l_statistics) : m_context(l_context),
    m_statistics(l_statistics)
{
    LoadConfigFile("enemies.cfg");
    RegisterEnemy<EnemyBase>(Enemy::Soldier);
    RegisterEnemy<Tank_Enemy>(Enemy::Tank);
}

EnemyManager::~EnemyManager() { Purge(); }

void EnemyManager::Purge()
{
    for(auto& itr : m_enemyProporties){
        for(auto& itr2 : itr.second){
            m_context->m_textureManager->ReleaseResource(itr2.second->m_texture);
            if(itr2.second->m_enemy == Enemy::Tank){
                m_context->m_textureManager->ReleaseResource(dynamic_cast<TankProporties*>(itr2.second)->m_tankTexture);
            }
            delete itr2.second;
        }
    }
    m_enemyProporties.clear();
    m_enemies.clear();
}

void EnemyManager::SpawnEnemy(const Enemy &l_enemy, const EnemyId& l_id, const sf::Vector2f &l_pos, const sf::Vector2f &l_destination)
{
    auto itr = m_factory.find(l_enemy);
    if(itr == m_factory.end()){
        return;
    }
    auto itr2 = m_enemyProporties.find(l_enemy);
    if(itr2 == m_enemyProporties.end() || itr2->second.find(l_id) == itr2->second.end()){
        auto itr3 = m_pathProporties.find(l_enemy);
        if(itr3 == m_pathProporties.end()){
            return;
        }
        auto itr4 = itr3->second.find(l_id);
        if(itr4 == itr3->second.end()){
            return;
        }
        LoadProporties(l_enemy, l_id, itr4->second);
        itr2 = m_enemyProporties.find(l_enemy);
    }
    auto itr5 = itr2->second.find(l_id);
    m_enemies.emplace_back(itr->second(itr5->second));
    m_enemies.back()->SetPosition(l_pos);
    m_enemies.back()->SetDestination(l_destination);
    m_sorted.emplace_back(m_enemies.back());
}

void EnemyManager::LoadConfigFile(const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetEnemiesDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when loading enemy config file: " << l_file << std::endl;
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        std::stringstream keystream(line);
        int key = 0;
        EnemyId id = 0;
        std::string path;
        keystream >> key >> id >> path;
        auto itr = m_pathProporties.emplace(static_cast<Enemy>(key), std::unordered_map<EnemyId, std::string>()).first;
        itr->second.emplace(id, path);
    }
    file.close();
}

void EnemyManager::LoadProporties(const Enemy& l_enemy, const EnemyId& l_id, const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetEnemiesDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when loading enemy config file: " << l_file << std::endl;
        return;
    }

    EnemyProporties* proporties;
    if(l_enemy == Enemy::Soldier){
        proporties = new EnemyProporties;
    } else if(l_enemy == Enemy::Tank){
        proporties = new TankProporties;
    }
    proporties->m_enemy = l_enemy;

    std::string line;
    while(std::getline(file, line)){
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;
        if(type == "SPEED"){
            keystream >> proporties->m_baseSpeed;
        } else if(type == "HP"){
            keystream >> proporties->m_baseHp;
        } else if(type == "SIZE"){
            keystream >> proporties->m_size.x >> proporties->m_size.y;
        } else if(type == "RADIUS"){
            keystream >> proporties->m_radius;
        } else if(type == "TYPE"){
            int type;
            keystream >> type;
            proporties->m_type = static_cast<EnemyType>(type);
        } else if(type == "TEXTURE"){
            keystream >> proporties->m_texture;
            m_context->m_textureManager->RequireResource(proporties->m_texture);
            proporties->m_sprite.setTexture(*m_context->m_textureManager->GetResource(proporties->m_texture));
        } else if(type == "TEXTURE_RECT"){
            sf::IntRect rect;
            keystream >> rect.left >> rect.top >> rect.width >> rect.height;
            proporties->m_sprite.setTextureRect(rect);
        } else if(type == "LIFE_TAKES"){
            keystream >> proporties->m_lifeTakes;
        } else if(type == "MONEY"){
            keystream >> proporties->m_money;
        } else if(l_enemy == Enemy::Tank){
            if(type == "TANK"){
                sf::IntRect rect;
                TankProporties* prop = dynamic_cast<TankProporties*>(proporties);
                keystream >> prop->m_tankTexture >> rect.left >> rect.top >> rect.width >> rect.height;
                m_context->m_textureManager->RequireResource(prop->m_tankTexture);
                prop->m_tankSprite.setTexture(*m_context->m_textureManager->GetResource(prop->m_tankTexture));
                prop->m_tankSprite.setTextureRect(rect);
                prop->m_tankSprite.setOrigin(sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
            }
        }
    }
    sf::IntRect size = proporties->m_sprite.getTextureRect();
    proporties->m_sprite.setOrigin(sf::Vector2f(size.width / 2.f, size.height / 2.f));
    auto itr = m_enemyProporties.emplace(l_enemy, std::unordered_map<EnemyId, EnemyProporties*>()).first;
    itr->second.emplace(l_id, proporties);
    file.close();
}


void EnemyManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_enemies){
        itr->Draw(window);
    }
}

void EnemyManager::Update(const float &l_dT)
{
    for(auto& itr : m_enemies){
        itr->Update(l_dT);
    }
    Sort();
}

sf::Vector2f EnemyManager::GiveNextWaypoint(EnemyBase *l_enemy)
{
    sf::Vector2f dest = m_context->m_level->GetWaypointAfter(l_enemy->m_unique.m_waypoint);
    if(dest.x == -1 && dest.y == -1){
        m_context->m_level->AddLifes(-l_enemy->m_proporties->m_lifeTakes);
        RemoveEnemy(l_enemy);
    }
    return dest;
}

void EnemyManager::ProcessRequests()
{
    if(!m_toRemove.empty()){
        while(!m_toRemove.empty()){

            m_enemies.erase(std::find(m_enemies.begin(), m_enemies.end(), m_toRemove.back()));
            m_sorted.erase(std::find(m_sorted.begin(), m_sorted.end(), m_toRemove.back()));

            m_toRemove.pop_back();
        }
        if(m_context->m_level->Finished() && m_enemies.empty()){
            m_context->m_level->Win();
        }
    }
}

void EnemyManager::Restart()
{
    m_enemies.clear();
}

void EnemyManager::Sort()
{
    std::sort(m_sorted.begin(), m_sorted.end(), [](std::shared_ptr<EnemyBase>& a, std::shared_ptr<EnemyBase>& b) -> bool{
        if(a->m_unique.m_waypoint != b->m_unique.m_waypoint){
            return a->m_unique.m_waypoint > b->m_unique.m_waypoint;
        }
        const sf::Vector2f& posA = a->GetProporties()->m_position;
        const sf::Vector2f& desA = a->GetProporties()->m_destination;
        const sf::Vector2f& posB = b->GetProporties()->m_position;
        const sf::Vector2f& desB = b->GetProporties()->m_destination;
        const sf::Vector2f diffA(abs(desA.x - posA.x), abs(desA.y - posA.y));
        const sf::Vector2f diffB(abs(desB.x - posB.x), abs(desB.y - posB.y));
        if(diffA.x != diffB.x){
            return diffA.x < diffB.x;
        } else return diffA.y < diffB.y;
    });
}

std::shared_ptr<EnemyBase> EnemyManager::GetEnemyFor(const std::shared_ptr<TowerBase>& l_tower)
{
    std::shared_ptr<EnemyBase> previous;
    for(auto& itr : m_sorted){
        bool aabb = (static_cast<int>(itr->GetEnemyProporties()->m_enemy) % 2) == 0;
        sf::Vector2f enemyPos = itr->GetProporties()->m_position;
        if(aabb){ /// aabb circle collision
            const twoFloats& size = itr->GetEnemyProporties()->m_size;
            sf::FloatRect rect(enemyPos.x - size.x / 2.f, enemyPos.y - size.y / 2.f, size.x, size.y);
            if(!l_tower->GetUpgradeProporties().m_radius || Utils::CircleAABBColliding(l_tower->GetPosition(), l_tower->GetUpgradeProporties().m_radius, rect)){
                if(l_tower->GetProporties()->m_attacking[static_cast<int>(itr->GetEnemyProporties()->m_type)]){
                    if(l_tower->GetStrategy() == AttackStrategy::First && !previous){
                        return itr;
                    }
                    previous = itr;
                }
            } else if(previous){
                return previous;
            }
        } else { /// circle circle collision

        }
    }

    return previous;
}
