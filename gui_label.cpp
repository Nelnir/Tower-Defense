#include "gui_label.h"
#include "utils.h"
#include "gui_interface.h"
#include "gui_manager.h"
#include "sharedcontext.h"
#include <iostream>
GUI_Label::GUI_Label(const std::string &l_name, GUI_Interface *m_owner) : GUI_Element(l_name, GUI_ElementType::Label, m_owner) {  }

GUI_Label::~GUI_Label() { }

void GUI_Label::ReadIn(std::stringstream &l_stream)
{
    std::string content;
    Utils::ReadQuotedString(l_stream, content);
    if(std::count(content.begin(), content.end(), '%') >= 2)
        Utils::ReplaceText(content, m_owner->GetManager()->GetContext()->m_settings);
    size_t start = -1;
    while(true){
        start = content.find("\\n", start + 1);
        if(start == std::string::npos){
            break;
        }
        content.replace(start, 2, "\n");
    }
    m_visual.m_text.setString(content);
}

void GUI_Label::OnClick(const sf::Vector2f& l_mousePos)
{
    SetState(GUI_ElementState::Clicked);
}

void GUI_Label::OnRelease()
{
    SetState(GUI_ElementState::Neutral);
}

void GUI_Label::OnHover(const sf::Vector2f& l_mousePos)
{
    SetState(GUI_ElementState::Focused);
}

void GUI_Label::OnLeave()
{
    SetState(GUI_ElementState::Neutral);
}

void GUI_Label::Draw(sf::RenderTarget* l_target)
{
    l_target->draw(m_visual.m_backgroundSolid);
    if(!m_style[m_state].m_backgroundImage.empty()){
        l_target->draw(m_visual.m_backgroundImage);
    }
    if (!m_style[m_state].m_glyph.empty()){
        l_target->draw(m_visual.m_glyph);
    }
    l_target->draw(m_visual.m_text);
}
