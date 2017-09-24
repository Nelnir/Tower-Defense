#include "enemymanager.h"
#include <iostream>
#include <fstream>
#include "utils.h"
#include "window.h"
#include "level.h"

#include "soldierenemy.h"
#include "texturemanager.h"


EnemyManager::EnemyManager(SharedContext* l_context) : m_context(l_context), m_enemyCount(0)
{
    LoadConfigFile("enemies.cfg");
    RegisterEnemy<SoldierEnemy>(Enemy::Soldier);
}

EnemyManager::~EnemyManager() { Purge(); }

void EnemyManager::Purge()
{
    for(auto& itr : m_enemyProporties){
        m_context->m_textureManager->ReleaseResource(itr.second->m_texture);
        delete itr.second;
    }
    m_enemyProporties.clear();
    for(auto& itr: m_enemies){
        delete itr.second;
    }
    m_enemies.clear();
}

void EnemyManager::SpawnEnemy(const Enemy &l_enemy, const sf::Vector2f &l_pos, const sf::Vector2f &l_destination)
{
    auto itr = m_factory.find(l_enemy);
    if(itr == m_factory.end()){
        return;
    }
    auto itr2 = m_enemyProporties.find(l_enemy);
    if(itr2 == m_enemyProporties.end()){
        auto itr3 = m_pathProporties.find(l_enemy);
        if(itr3 == m_pathProporties.end()){
            return;
        }
        LoadProporties(l_enemy, itr3->second);
        itr2 = m_enemyProporties.find(l_enemy);
    }

    AbstractEnemy* enemy = itr->second(itr2->second);
    enemy->SetPosition(l_pos);
    enemy->SetDestination(l_destination);
    enemy->GetProporties()->m_id = m_enemyCount;
    m_enemies.emplace(m_enemyCount++, enemy);
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
        int key;
        std::string path;
        keystream >> key >> path;
        m_pathProporties.emplace(static_cast<Enemy>(key), path);
    }
    file.close();
}

void EnemyManager::LoadProporties(const Enemy& l_enemy, const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetEnemiesDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when loading enemy config file: " << l_file << std::endl;
        return;
    }

    EnemyProporties* proporties = new EnemyProporties;
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
        }
    }
    sf::IntRect size = proporties->m_sprite.getTextureRect();
    proporties->m_sprite.setOrigin(sf::Vector2f(size.width / 2.f, size.height / 2.f));
    m_enemyProporties.emplace(l_enemy, proporties);
    file.close();
}


void EnemyManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_enemies){
        itr.second->Draw(window);
    }
}

void EnemyManager::Update(const float &l_dT)
{
    for(auto& itr : m_enemies){
        itr.second->Update(l_dT);
    }
}

void EnemyManager::GiveNextWaypoint(AbstractEnemy *l_enemy)
{
    sf::Vector2f dest = m_context->m_level->GetWaypointAfter(l_enemy->m_unique.m_waypoint);
    if(dest.x != -1 && dest.y != -1){
        l_enemy->SetDestination(dest);
    } else{
        m_context->m_level->RemoveLifes(l_enemy->m_proporties->m_lifeTakes);
        m_toRemove.emplace_back(l_enemy->m_unique.m_id);
    }
}

void EnemyManager::ProcessRequests()
{
    while(!m_toRemove.empty()){
        auto itr = m_enemies.find(m_toRemove.front());
        delete itr->second;
        m_enemies.erase(itr);
        m_toRemove.erase(m_toRemove.begin());
    }
}
