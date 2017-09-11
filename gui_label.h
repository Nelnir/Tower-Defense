#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "gui_element.h"

class GUI_Label : public GUI_Element
{
public:
    GUI_Label(const std::string& l_name, GUI_Interface* m_owner);
    ~GUI_Label();

    void ReadIn(std::stringstream &l_stream);
    void OnClick(const sf::Vector2f &l_mousePos);
    void OnHover(const sf::Vector2f &l_mousePos);
    void OnRelease();
    void OnLeave();
    void Update(const float &l_dT) {}
    void Draw(sf::RenderTarget *l_target);
};

#endif // GUI_LABEL_H
