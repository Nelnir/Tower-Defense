#ifndef GUI_TEXTFIELD_H
#define GUI_TEXTFIELD_H

#include "gui_element.h"

class GUI_Textfield : public GUI_Element
{
public:
    GUI_Textfield(const std::string& l_name, GUI_Interface* l_owner);
    ~GUI_Textfield() {}

    void ReadIn(std::stringstream &l_stream);
    void Draw(sf::RenderTarget *l_target);

    void OnClick(const sf::Vector2f &l_mousePos) {}
    void OnHover(const sf::Vector2f &l_mousePos) {}
    void OnRelease() {}
    void OnLeave() {}
    void Update(const float &l_dT) {}
};

#endif // GUI_TEXTFIELD_H
