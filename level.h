#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include "sharedcontext.h"
#include "texturemanager.h"

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


using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;


class Level
{
public:
    Level(SharedContext* l_context);

    void Draw(const unsigned int& l_layer);

    void LoadLevel(const std::string& l_file);
    void Update(const float& l_dT);
    Tile* GetTile(const unsigned int& l_x, const unsigned int& l_y, const unsigned int& l_layer);
private:
    void LoadTiles(const std::string& l_file);
    unsigned int ConvertCoords(const unsigned int& l_x, const unsigned int& l_y, const unsigned int& l_layer);
    TileMap m_tileMap;
    TileSet m_tileSet;

    SharedContext* m_context;
    sf::Vector2u m_maxMapSize;
};

#endif // LEVEL_H
