#ifndef GUI_SCROLLBAR_H
#define GUI_SCROLLBAR_H

#include "gui_element.h"

enum class SliderType { Horizontal, Vertical };

class GUI_Scrollbar : public GUI_Element
{
public:
    GUI_Scrollbar(const std::string& l_name, GUI_Interface* l_owner);
    ~GUI_Scrollbar();

    void ReadIn(std::stringstream& l_stream);
    void OnClick(const sf::Vector2f& l_mousePos) ;
    void OnRelease();
    void OnHover(const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Update(const float& l_dT);
    void Draw(sf::RenderTarget* l_target) ;

    void SetPosition(const sf::Vector2f &l_pos);
    void ApplyStyle();
    void UpdateStyle(const GUI_ElementState &l_state, const GUI_Style &l_style);
private:
    SliderType m_sliderType;
    sf::RectangleShape m_slider;
    sf::Vector2f m_moveMouseLast;
    int m_percentage;
};

#endif // GUI_SCROLLBAR_H
