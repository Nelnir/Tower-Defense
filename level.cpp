#include "level.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include "window.h"

Level::Level(SharedContext* l_context) : m_context(l_context), m_maxMapSize(0, 0)
{
    LoadTiles("tiles.cfg");
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
            keystream >> tileCoords.x >> tileCoords.y >> tileLayer >> towerPlaceable;
            if(tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y || tileLayer >= Sheet::Num_Layers){
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
        } else if(type == "SIZE"){
            keystream >> m_maxMapSize.x >> m_maxMapSize.y;
        }
    }
    file.close();
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

}
