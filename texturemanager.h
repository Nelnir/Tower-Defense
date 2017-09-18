#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "resourcemanager.h"
#include <SFML/Graphics.hpp>

#include <iostream>

class TextureManager :
        public ResourceManager<TextureManager, sf::Texture>
{
public:
    TextureManager() : ResourceManager("textures\\textures.cfg") {}

    sf::Texture * Load(const std::string &l_path){
        sf::Texture * texture = new sf::Texture;
        if(!texture->loadFromFile(Utils::GetTexturesDirectory() + '\\' + l_path)){
            delete texture;
            texture = nullptr;
            std::cerr << "Failed to load texture: " << l_path << std::endl;
        }
        return texture;
    }
};


#endif // TEXTUREMANAGER_H
