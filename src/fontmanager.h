#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "resourcemanager.h"
#include <SFML/Graphics.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font>
{
public:
    FontManager() : ResourceManager("fonts.cfg") {}
    sf::Font* Load(const std::string& l_path){
        sf::Font* font = new sf::Font;
        if (!font->loadFromFile(Utils::GetDataDirectory() + '\\' + l_path)){
            delete font;
            font = nullptr;
            std::cerr << "! Failed to load font: " << l_path << std::endl;
        }
        return font;
    }
};

#endif // FONTMANAGER_H
