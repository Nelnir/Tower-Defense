#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include "sharedcontext.h"
#include "texturemanager.h"
#include "abstractenemy.h"

enum Sheet{Tile_Size = 64, Sheet_Width = 1472, Sheet_Height = 832, Num_Layers = 2};
using TileID = unsigned int;

struct TileInfo{
    TileInfo(SharedContext* l_context, const std::string& l_texture = "", TileID l_id = 0)
    : m_context(l_context), m_id(0), m_ornament(true)
    {
        TextureManager* tmgr = l_context->m_textureManager;
        if (l_texture.empty()){ m_id = l_id; return; }
        if (!tmgr->RequireResource(l_texture)){ return; }
        m_texture = l_texture;
        m_id = l_id;
        m_sprite.setTexture(*tmgr->GetResource(m_texture));
        sf::IntRect tileBoundaries(m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
        m_id / (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
        Sheet::Tile_Size, Sheet::Tile_Size);
        m_sprite.setTextureRect(tileBoundaries);
    }
    ~TileInfo(){
        if (m_texture.empty()){ return; }
        m_context->m_textureManager->ReleaseResource(m_texture);
    }

    sf::Sprite m_sprite;
    TileID m_id;
    std::string m_name;
    bool m_ornament;
    SharedContext* m_context;
    std::string m_texture;
};

struct Tile{
    TileInfo* m_proporties;
    bool m_towerPlaceable;
    // others flag unique to each tile
};


struct Wave{
    std::vector<Enemy> m_enemies;
    float m_interval;
};

using Waves = std::vector<Wave*>;
using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;
using Waypoints = std::vector<sf::Vector2f>;

enum class Direction { Left = 0, Right, Up, Down, Middle };

class Level
{
public:
    Level(SharedContext* l_context);
    ~Level();

    void StartGame() { m_playing = true; }
    void NextWave() { ++m_currentWave; m_spawnedEnemies = 0; }

    void Draw(const unsigned int& l_layer);

    void LoadLevel(const std::string& l_file);
    void Update(const float& l_dT);
    Tile* GetTile(const unsigned int& l_x, const unsigned int& l_y, const unsigned int& l_layer);
    bool CollideWithPath(const sf::CircleShape& l_circle);
    sf::Vector2f GetWaypointAfter(int l_waypoint);

    void RemoveLifes(const int& l_lifes);
private:
    void TransformPosition(sf::Vector2f& l_pos, const Direction& l_direction);
    void Purge();
    void LoadTiles(const std::string& l_file);
    void AddWaypoint(const sf::Vector2i& l_pos);
    unsigned int ConvertCoords(const unsigned int& l_x, const unsigned int& l_y, const unsigned int& l_layer);
    TileMap m_tileMap;
    TileSet m_tileSet;

    SharedContext* m_context;
    sf::Vector2u m_maxMapSize;
    sf::Vector2f m_monsterStartSpawn;
    int m_currentWave;
    int m_spawnedEnemies;
    Waves m_waves;
    bool m_playing;
    float m_elapsed;
    Waypoints m_waypoints;
    int m_lives;
};

#endif // LEVEL_H
