#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SFML/Graphics.hpp>
#include "directions.h"
#include <unordered_map>

class TextureManager;
class Anim_Base;

class SpriteSheet
{
    using Animations = std::unordered_map<std::string, Anim_Base*>;

    std::string m_texture;
    sf::Sprite m_sprite;
    sf::Vector2i m_spriteSize;
    sf::Vector2f m_spriteScale;
    Direction m_direction;
    std::string m_animType;
    Animations m_animations;
    Anim_Base* m_animationCurrent;
    TextureManager* m_textureManager;
public:
    SpriteSheet(TextureManager* l_textMgr);
    ~SpriteSheet();

    void CropSprite(const sf::IntRect& l_rect);

    void SetSpriteSize(const sf::Vector2i& l_size);
    void SetSpritePosition(const sf::Vector2f& l_pos);
    void SetDirection(const Direction& l_dir);

    sf::Vector2i GetSpriteSize() const { return m_spriteSize; }
    sf::Vector2f GetSpritePosition() const { return m_sprite.getPosition(); }
    Direction GetDirection() const { return m_direction; }

    bool LoadSheet(const std::string& l_file);
    void ReleaseSheet();
    Anim_Base* GetCurrentAnim();
    bool SetAnimation(const std::string& l_name, const bool& l_play = false, const bool& l_loop = false);
    void Update(const float& l_dT);
    void Draw(sf::RenderWindow* l_wnd);
};

#endif // SPRITESHEET_H
