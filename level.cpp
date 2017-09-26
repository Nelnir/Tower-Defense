#include "level.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "window.h"
#include "enemymanager.h"
#include "gui_interface.h"
#include "abstracttower.h"
#include "statemanager.h"

Level::Level(SharedContext* l_context, GUI_Interface* l_interface, Connections* l_conn, Statistics* l_statistics) : m_context(l_context),
    m_maxMapSize(0, 0), m_currentWave(0), m_spawnedEnemies(0), m_elapsed(0), m_lifes(0), m_money(0),
    m_interface(l_interface), m_connections(l_conn), m_previousTime(0), m_statistics(l_statistics) {}

Level::~Level()
{
    Purge();
    for(auto& itr : m_tileSet){
        delete itr.second;
    }
    m_tileSet.clear();
}

void Level::Purge()
{
    for(auto& itr : m_waves){
        delete itr;
    }
    for(auto& itr : m_tileMap){
        delete itr.second;
    }
    m_tileMap.clear();
    m_waves.clear();
    m_elapsed = false;
    m_currentWave = 0;
    m_spawnedEnemies = 0;
}

void Level::Draw(const unsigned int& l_layer)
{
    if (l_layer >= Sheet::Num_Layers){
        return;
    }
    sf::RenderWindow* l_wind = m_context->m_wind->getRenderWindow();
    sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
    sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::Tile_Size), floor(viewSpace.top / Sheet::Tile_Size));
    sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size), ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));
    unsigned int count = 0;

    for(int x = tileBegin.x; x <= tileEnd.x; ++x){
        for(int y = tileBegin.y; y <= tileEnd.y; ++y){
            Tile* tile = GetTile(x, y, l_layer);
            if (!tile){
                continue;
            }
            sf::Sprite& sprite = tile->m_proporties->m_sprite;
            sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
            l_wind->draw(sprite);
            ++count;
        }
    }
}

Tile* Level::GetTile(const unsigned int &l_x, const unsigned int &l_y, const unsigned int &l_layer)
{
    if(l_x < 0 || l_y < 0 || l_x >= m_maxMapSize.x || l_y >= m_maxMapSize.y || l_layer < 0 || l_layer >= Sheet::Num_Layers){
        return nullptr;
    }
    auto & itr = m_tileMap.find(ConvertCoords(l_x, l_y, l_layer));
    return (itr != m_tileMap.end() ? itr->second : nullptr);
}

void Level::LoadLevel(const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetLevelsDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when loading level file: " << l_file << std::endl;
        return;
    }

    struct Points{
        unsigned int x, y;
    };
    Points previous, next;
    bool goingX = false;
    bool change = false;
    next.x = next.y = previous.x = previous.y = 0;

    std::string line;
    while(std::getline(file, line)){
       if(line[0] == '|'){
            continue;
        }
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;

        if(type == "TILE")
        {
            int tileId = 0;
            keystream >> tileId;
            if(tileId < 0){
                std::cerr << "Wrong tile id: " << tileId << std::endl;
                continue;
            }
            auto itr = m_tileSet.find(tileId);
            if(itr == m_tileSet.end()){
                std::cerr << "Tile id(" << tileId << ") was not found in tileshett" << std::endl;
                continue;
            }

            sf::Vector2i tileCoords;
            unsigned int tileLayer = 0;
            bool towerPlaceable = false;

            previous = next;
            keystream >> tileCoords.x >> tileCoords.y >> tileLayer >> towerPlaceable;
            if(!towerPlaceable){
                next.x = tileCoords.x;
                next.y = tileCoords.y;
            }

            if(next.y == previous.y){
                if(!goingX){
                  change = true;
                }
                goingX = true;
            } else{
                if(goingX){
                    change = true;
                }
                goingX = false;
            }

            if(tileCoords.x >= m_maxMapSize.x || tileCoords.y >= m_maxMapSize.y || tileLayer >= Sheet::Num_Layers){
                std::cerr << "Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
                continue;
            }

            Tile* tile = new Tile;
            tile->m_proporties = itr->second;
            tile->m_towerPlaceable = towerPlaceable;

            if(!m_tileMap.emplace(ConvertCoords(tileCoords.x, tileCoords.y, tileLayer), tile).second){
                std::cerr << "Duplicate tile: " << tileCoords.x << " " << tileCoords.y << std::endl;
                delete tile;
                tile = nullptr;
                continue;
            }

            if(change && !towerPlaceable){
                AddWaypoint(sf::Vector2i(previous.x, previous.y));
            }
            change = false;

        } else if(type == "SIZE"){
            keystream >> m_maxMapSize.x >> m_maxMapSize.y;
        } else if(type == "START"){
            int direction;
            keystream >> direction;
            TransformPosition(m_waypoints.front(), static_cast<Direction>(direction));
        } else if(type == "END"){
            int direction;
            sf::Vector2i l_pos;
            keystream >> l_pos.x >> l_pos.y >> direction;
            sf::Vector2f pos(l_pos.x * Sheet::Tile_Size + Sheet::Tile_Size / 2.f, l_pos.y * Sheet::Tile_Size + Sheet::Tile_Size / 2.f);
            TransformPosition(pos, static_cast<Direction>(direction));
            m_waypoints.emplace_back(pos);
        } else if(type == "BACKGROUND"){
            TileID tileID;
            bool towerPlaceable = false;
            keystream >> tileID >> towerPlaceable;
            auto itr = m_tileSet.find(tileID);
            if(itr == m_tileSet.end()){
                std::cerr << "Tile id(" << tileID << ") not found in tileset - BACKGROUND" << std::endl;
                continue;
            }
            for(int i = 0;i<m_maxMapSize.x;++i){
                for(int j = 0;j<m_maxMapSize.y;++j){
                    unsigned int Coords = ConvertCoords(i, j, 0);
                    if(m_tileMap.find(Coords) == m_tileMap.end()){
                        Tile* tile = new Tile;
                        tile->m_proporties = itr->second;
                        tile->m_towerPlaceable = towerPlaceable;
                        m_tileMap.emplace(Coords, tile);
                    }
                }
            }
        } else if(type == "WAVE"){
            float interval, time;
            int reward;
            keystream >> interval >> reward >> time;
            if(time < 5){ time = 5; }
            Wave * wave = new Wave(interval, reward, time);
            while(!keystream.eof()){
                int m_enemy;
                EnemyId id;
                keystream >> m_enemy >> id;
                wave->m_enemies.emplace_back(std::make_pair(static_cast<Enemy>(m_enemy), id));
            }
            if(wave->m_enemies.empty()){
                delete wave;
            } else {
                m_waves.emplace_back(wave);
            }
        } else if(type == "LIFES"){
            keystream >> m_lifes;
            m_baseLifes = m_lifes;
        } else if(type == "MONEY"){
            keystream >> m_money;
            m_baseMoney = m_money;
            m_statistics->AddMoneyEarned(m_money);
        }
    };
    Initialize();
    file.close();
}

void Level::TransformPosition(sf::Vector2f &l_pos, const Direction &l_direction)
{
    switch(l_direction){
    case Direction::Down: l_pos.y += Sheet::Tile_Size;
        break;
    case Direction::Left: l_pos.x -= Sheet::Tile_Size;
        break;
    case Direction::Middle: /// nothing to do
        break;
    case Direction::Right: l_pos.x += Sheet::Tile_Size;
        break;
    case Direction::Up: l_pos.y -= Sheet::Tile_Size;
        break;
    }
}

void Level::AddWaypoint(const sf::Vector2i& l_pos)
{
    sf::Vector2f pos(l_pos.x * Sheet::Tile_Size + Sheet::Tile_Size / 2.f, l_pos.y * Sheet::Tile_Size + Sheet::Tile_Size / 2.f);
    m_waypoints.emplace_back(pos);
}

void Level::LoadTiles(const std::string &l_path)
{
    std::ifstream file;
    file.open(Utils::GetDataDirectory() + '\\' + l_path);
    if(!file.is_open()){
        std::cerr << "Error when loading tile set file: " << l_path << std::endl;
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        if(line[0] == '|'){
            continue;
        }
        std::stringstream keystream(line);
        int tileId;
        keystream >> tileId;
        if(tileId < 0){
            continue;
        }
        TileInfo * tile = new TileInfo(m_context, "Tileset", tileId);
        keystream >> tile->m_name >> tile->m_ornament;
        if(!m_tileSet.emplace(tileId, tile).second){
            std::cerr << "Duplicate tile type detected: " << tile->m_name << " " << tile->m_id << std::endl;
            delete tile;
        }
    }

    file.close();
}

unsigned int Level::ConvertCoords(const unsigned int &l_x, const unsigned int &l_y, const unsigned int &l_layer)
{
    return ((l_layer * m_maxMapSize.y + l_y) * m_maxMapSize.x + l_x);
}

void Level::Update(const float& l_dT)
{
    m_elapsed += l_dT;
    Wave* current = m_waves[m_currentWave];
    m_elapsedWave -= l_dT;
    if(m_spawnedEnemies < current->m_enemies.size() && m_elapsed >= current->m_interval){
        m_elapsed -= current->m_interval;
        auto& enemy = current->m_enemies[m_spawnedEnemies++];
        m_context->m_enemyManager->SpawnEnemy(enemy.first, enemy.second, m_waypoints[0], m_waypoints[1]);
        UpdateMonstersToSpawnGUI();
    }
    if(m_currentWave < m_waves.size()){
        UpdateWaveTimeGUI();
        if(m_elapsedWave <= 0 && m_currentWave + 1 < m_waves.size()){
            NextWave();
        }
    }
}

template<class T>
const T& clamp( const T& v, const T& lo, const T& hi )
{
    if(v < lo) return lo;
    if(hi < v) return hi;
    return v;
}

bool Level::CollideWithPath(const sf::CircleShape &l_circle)
{
    sf::Vector2f pos = l_circle.getPosition();
    float radius = l_circle.getRadius();
    pos.x += radius;
    pos.y += radius;
    for(unsigned layer = 0; layer < static_cast<unsigned int>(Sheet::Num_Layers); ++layer){
        for(unsigned int x = 0; x < m_maxMapSize.x; ++x){
            for(unsigned int y = 0; y < m_maxMapSize.y; ++y){
                Tile* tile = GetTile(x, y, layer);
                if(!tile) { continue; }
                if(tile->m_towerPlaceable && !tile->m_proporties->m_ornament) { continue; }
               /* if(l_circle.getGlobalBounds().intersects(sf::FloatRect(x * Sheet::Tile_Size, y * Sheet::Tile_Size, Sheet::Tile_Size, Sheet::Tile_Size))){
                    return true;

                }*/
                const float closestX = clamp(pos.x, float(x * Sheet::Tile_Size), float((x + 1) * Sheet::Tile_Size));
                const float closestY = clamp(pos.y, float(y * Sheet::Tile_Size), float((y + 1) * Sheet::Tile_Size));

                // Calculate the distance between the circle's center and this closest point
                const float distanceX = pos.x - closestX;
                const float distanceY = pos.y - closestY;

                // If the distance is less than the circle's radius, an intersection occurs
                const float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
                if(distanceSquared < (radius * radius))
                    return true;
            }
        }
    }
    return false;
}

sf::Vector2f Level::GetWaypointAfter(int l_waypoint)
{
    if(l_waypoint + 1 == m_waypoints.size()){
        return sf::Vector2f(-1, -1);
    }
    return m_waypoints[l_waypoint + 1];
}

void Level::AddLifes(const int &l_lifes)
{
    m_lifes += l_lifes;
    UpdateLifesGUI();
    if(m_lifes <= 0){
        Lose();
    }
}

void Level::AddMoney(const int &l_money)
{
    m_money += l_money;
    UpdateMoneyGUI();
}

void Level::UpdateLifesGUI()
{
    m_interface->GetElement("Lives")->SetText("Lifes: " + std::to_string(m_lifes));
}

void Level::UpdateMoneyGUI()
{
    m_interface->GetElement("Money")->SetText("Money: " + std::to_string(m_money) + m_context->m_settings->GetCurrency());
    for(auto& itr : *m_connections){
        TowerProporties* prop = itr.second;
        if(prop->m_cost > m_money){
            m_interface->GetElement(itr.first)->SetState(GUI_ElementState::Locked);
        } else{
            m_interface->GetElement(itr.first)->SetState(GUI_ElementState::Neutral);
        }
    }
}

void Level::UpdateWaveGUI()
{
    m_interface->GetElement("Wave")->SetText("Wave: " + std::to_string(m_currentWave + 1) + " / " + std::to_string(m_waves.size()));
}

void Level::UpdateMonstersToSpawnGUI()
{
    int toSpawn = m_waves[m_currentWave]->m_enemies.size() - m_spawnedEnemies;
    m_interface->GetElement("MonstersSpawn")->SetText("To spawn: " + (toSpawn ? std::to_string(toSpawn) : "-"));
}

void Level::UpdateWaveTimeGUI()
{
    if(m_currentWave + 1 < m_waves.size()){
        int timeToSet = static_cast<int>(m_elapsedWave);
        if(timeToSet != m_previousTime){
            m_previousTime = timeToSet;
            if(timeToSet >= 0){
                m_interface->GetElement("WaveTime")->SetText("Next in: " + std::to_string(timeToSet));
            }
        }
    } else {
        m_interface->GetElement("WaveTime")->SetText("Next in: ---");
    }
}

void Level::NextWave()
{
    Wave* wave = m_waves[m_currentWave];
    m_money += wave->m_reward;

    m_statistics->AddMoneyEarned(wave->m_reward);

    UpdateMoneyGUI();
    ++m_currentWave;
    wave = m_waves[m_currentWave];
    m_spawnedEnemies = 0;
    m_elapsed = wave->m_interval;
    m_elapsedWave = wave->m_time;
    m_elapsedWave += wave->m_interval * wave->m_enemies.size();
    UpdateWaveGUI();
    UpdateMonstersToSpawnGUI();
}

void Level::Win()
{
    m_context->m_settings->SetWin(true);
    m_context->m_stateMgr->SwitchTo(StateType::GameOver);
    m_interface->SetContentRedraw(true);
}

void Level::Lose()
{
    m_context->m_stateMgr->SwitchTo(StateType::GameOver);
    m_interface->SetContentRedraw(true); /// well in State_GameOver we are drawning this interface manually, but it wont change it content if it wont be able
    /// to call it's update method, so we set manually flag for redrawning to show updated lifes
}

bool Level::Finished()
{
    return (m_currentWave + 1 == m_waves.size() && m_spawnedEnemies == m_waves.back()->m_enemies.size());
}

void Level::Restart()
{
    Initialize();
    m_interface->GetElement("Start")->SetText("Start");
}

void Level::Initialize()
{
    m_elapsed = m_waves.front()->m_interval;
    m_currentWave = 0;
    m_spawnedEnemies = 0;
    m_money = m_baseMoney;
    m_lifes = m_baseLifes;
    m_elapsedWave = m_waves.front()->m_time + m_waves.front()->m_interval * m_waves.front()->m_enemies.size();
    UpdateMoneyGUI();
    UpdateLifesGUI();
    UpdateMonstersToSpawnGUI();
    UpdateWaveGUI();
    UpdateWaveTimeGUI();
}
