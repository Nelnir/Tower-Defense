#include "gui_textfield.h"
#include "utils.h"

GUI_Textfield::GUI_Textfield(const std::string &l_name, GUI_Interface* l_owner) : GUI_Element(l_name, GUI_ElementType::Textfield, l_owner)
{

}

void GUI_Textfield::ReadIn(std::stringstream &l_stream)
{
    std::string content;
    Utils::ReadQuotedString(l_stream, content);
    m_visual.m_text.setString(content);
}

void GUI_Textfield::Draw(sf::RenderTarget* l_target)
{
     l_target->draw(m_visual.m_backgroundSolid);
     if (!m_style[m_state].m_glyph.empty()){
        l_target->draw(m_visual.m_glyph);
     }
     l_target->draw(m_visual.m_text);
}
