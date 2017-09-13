#include "gui_interface.h"
#include "utils.h"
#include "gui_event.h"
#include "sharedcontext.h"
#include "gui_manager.h"
#include "eventmanager.h"
#include "window.h"
#include <iostream>

GUI_Interface::GUI_Interface(const std::string& l_name, GUI_Manager* l_guiManager)
 : GUI_Element(l_name, GUI_ElementType::Window, this),
 m_parent(nullptr), m_guiManager(l_guiManager), m_movable(false),
 m_beingMoved(false), m_showTitleBar(false), m_focused(false),
 m_scrollHorizontal(0), m_scrollVertical(0),m_contentRedraw(true),
 m_controlRedraw(true)
{
    m_backdropTexture = new sf::RenderTexture;
    m_contentTexture = new sf::RenderTexture;
    m_controlTexture = new sf::RenderTexture;
}

GUI_Interface::~GUI_Interface()
{
    delete m_backdropTexture;
    delete m_contentTexture;
    delete m_controlTexture;
    for (auto &itr : m_elements){
        delete itr.second;
    }
    m_elements.clear();
}

void GUI_Interface::SetPosition(const sf::Vector2f& l_pos)
{
    GUI_Element::SetPosition(l_pos);
    m_backdrop.setPosition(l_pos);
    m_content.setPosition(l_pos);
    m_control.setPosition(l_pos);
    m_titleBar.setPosition(m_position.x, m_position.y - m_titleBar.getSize().y);
    m_visual.m_text.setPosition(m_titleBar.getPosition() + m_style[m_state].m_textPadding);
}

bool GUI_Interface::AddElement(const GUI_ElementType& l_type, const std::string& l_name)
{
    if(m_elements.find(l_name) != m_elements.end()){
        return false;
    }
    GUI_Element* element = nullptr;
    element = m_guiManager->CreateElement(l_type, this);
    if(!element){
        return false;
    }
    element->SetName(l_name);
    element->SetOwner(this);
    m_elements.emplace(l_name, element);
    m_contentRedraw = true;
    m_controlRedraw = true;
    return true;
}

GUI_Element* GUI_Interface::GetElement(const std::string& l_name) const
{
    auto itr = m_elements.find(l_name);
    return (itr != m_elements.end() ? itr->second : nullptr);
}

bool GUI_Interface::RemoveElement(const std::string& l_name)
{
    auto& itr = m_elements.find(l_name);
    if (itr == m_elements.end()){
        return false;
    }
    delete itr->second;
    m_elements.erase(itr);
    m_contentRedraw = true;
    m_controlRedraw = true;
    AdjustContentSize();
    return true;
}

bool GUI_Interface::IsInside(const sf::Vector2f& l_point) const
{
    if (GUI_Element::IsInside(l_point)){
        return true;
    }
    return m_titleBar.getGlobalBounds().contains(l_point);
}

void GUI_Interface::ReadIn(std::stringstream& l_stream)
{
    std::string movableState;
    std::string titleShow;
    std::string title;
    l_stream >> m_elementPadding.x >> m_elementPadding.y >> movableState >> titleShow;
    Utils::ReadQuotedString(l_stream, title);
    m_visual.m_text.setString(title);
    if (movableState == "Movable"){
        m_movable = true;
    }
    if (titleShow == "Title"){
        m_showTitleBar = true;
    }
}

void GUI_Interface::OnClick(const sf::Vector2f& l_mousePos)
{
    DefocusTextfields();
    if (m_titleBar.getGlobalBounds().contains(l_mousePos) && m_movable && m_showTitleBar){
        BeginMoving();
    } else {
        GUI_Event event;
        event.m_type = GUI_EventType::Click;
        event.m_interface = m_name.c_str();
        event.m_element = "";
        event.m_clickCoords.x = l_mousePos.x;
        event.m_clickCoords.y = l_mousePos.y;
        m_guiManager->AddEvent(event);
        for (auto &itr : m_elements){
            if (!itr.second->IsInside(l_mousePos)){
                continue;
            }
            itr.second->OnClick(l_mousePos);
            event.m_element = itr.second->m_name.c_str();
            m_guiManager->AddEvent(event);
        }
        SetState(GUI_ElementState::Clicked);
    }
}

void GUI_Interface::OnRelease()
{
    GUI_Event event;
    event.m_type = GUI_EventType::Release;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    m_guiManager->AddEvent(event);

    sf::Vector2i mousePos = m_guiManager->GetContext()->m_eventManager->GetMousePos(m_guiManager->GetContext()->m_wind->getRenderWindow());

    for (auto &itr : m_elements){
        if (itr.second->GetState() != GUI_ElementState::Clicked){
            continue;
        }
        itr.second->OnRelease();
        event.m_element = itr.second->m_name.c_str();
        if(itr.second->IsInside(sf::Vector2f(mousePos))){
            event.m_type = GUI_EventType::ReleaseInside;
        }
        m_guiManager->AddEvent(event);
    }
    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnHover(const sf::Vector2f& l_mousePos)
{
    GUI_Event event;
    event.m_type = GUI_EventType::Hover;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    event.m_clickCoords.x = l_mousePos.x;
    event.m_clickCoords.y = l_mousePos.y;
    m_guiManager->AddEvent(event);
    SetState(GUI_ElementState::Focused);
}
void GUI_Interface::OnLeave()
{
    GUI_Event event;
    event.m_type = GUI_EventType::Leave;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    m_guiManager->AddEvent(event);
    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnTextEntered(const char& l_char)
{
    for (auto &itr : m_elements){
        if (itr.second->GetType() != GUI_ElementType::Textfield){
            continue;
        }
        if (itr.second->GetState() != GUI_ElementState::Clicked){
            continue;
        }
        if (l_char == 8){
        // Backspace.
            const auto& text = itr.second->GetText();
            itr.second->SetText(text.substr(0, text.length() - 1));
            return;
        }
        if (l_char < 32 || l_char > 126){
            return;
        }
        std::string text = itr.second->GetText();
        text.push_back(l_char);
        itr.second->SetText(text);
        return;
    }
}

void GUI_Interface::DefocusTextfields()
{
    GUI_Event event;
    event.m_type = GUI_EventType::Release;
    event.m_interface = m_name.c_str();
    event.m_element = "";
    for (auto &itr : m_elements){
        if (itr.second->GetType() != GUI_ElementType::Textfield){
            continue;
        }
        itr.second->SetState(GUI_ElementState::Neutral);
        event.m_element = itr.second->m_name.c_str();
        m_guiManager->AddEvent(event);
    }
}

void GUI_Interface::Update(const float &l_dT)
{
    sf::Vector2f mousePos = sf::Vector2f(m_guiManager->GetContext()->m_eventManager->GetMousePos(m_guiManager->GetContext()->m_wind->getRenderWindow()));
    if (m_beingMoved && m_moveMouseLast != mousePos){
        sf::Vector2f difference = mousePos - m_moveMouseLast;
        m_moveMouseLast = mousePos;
        sf::Vector2f newPosition = m_position + difference;
        SetPosition(newPosition);
    }

    for (auto& itr : m_elements)
    {
        if (itr.second->NeedsRedraw()){
            if (itr.second->IsControl()){
                m_controlRedraw = true;
            } else {
                m_contentRedraw = true;
            }
        }
        if (!itr.second->IsActive()){
            continue;
        }

        itr.second->Update(l_dT);

        if (m_beingMoved){
            continue;
        }

        GUI_Event event;
        event.m_interface = m_name.c_str();
        event.m_element = itr.second->m_name.c_str();
        event.m_clickCoords.x = mousePos.x;
        event.m_clickCoords.y = mousePos.y;
        event.m_type = GUI_EventType::None;
        if (IsInside(mousePos) && itr.second->IsInside(mousePos) && !m_titleBar.getGlobalBounds().contains(mousePos)){
            if (itr.second->GetState() != GUI_ElementState::Neutral || m_guiManager->IsMousePressed()){
                continue;
            }
            itr.second->OnHover(mousePos);
            event.m_type = GUI_EventType::Hover;
        } else if (itr.second->GetState() == GUI_ElementState::Focused){
            itr.second->OnLeave();
            event.m_type = GUI_EventType::Leave;
        }
        if(event.m_type != GUI_EventType::None){
            m_guiManager->AddEvent(event);
        }
    }
}

void GUI_Interface::Draw(sf::RenderTarget* l_target)
{
    l_target->draw(m_backdrop);
    l_target->draw(m_content);
    l_target->draw(m_control);
    if (!m_showTitleBar){
        return;
    }
    l_target->draw(m_titleBar);
    l_target->draw(m_visual.m_text);
}

void GUI_Interface::BeginMoving()
{
    if (!m_showTitleBar || !m_movable){
        return;
    }
    m_beingMoved = true;
    SharedContext* context = m_guiManager->GetContext();
    m_moveMouseLast = sf::Vector2f(context->m_eventManager->GetMousePos(context->m_wind->getRenderWindow()));
}

sf::Vector2f GUI_Interface::GetGlobalPosition() const
{
    sf::Vector2f pos = m_position;
    GUI_Interface* i = m_parent;
    while(i){
        pos += i->GetPosition();
        i = i->m_parent;
    }
    return pos;
}

void GUI_Interface::ApplyStyle()
{
    GUI_Element::ApplyStyle(); // Call base method.
    m_visual.m_backgroundSolid.setPosition(0.f, 0.f);
    m_visual.m_backgroundImage.setPosition(0.f, 0.f);
    m_titleBar.setSize(sf::Vector2f(m_style[m_state].m_size.x, 16.f));
    m_titleBar.setPosition(m_position.x, m_position.y - m_titleBar.getSize().y);
    m_titleBar.setFillColor(m_style[m_state].m_elementColor);
    m_visual.m_text.setPosition(m_titleBar.getPosition() + m_style[m_state].m_textPadding);
    m_visual.m_glyph.setPosition(m_titleBar.getPosition() + m_style[m_state].m_glyphPadding);
}

void GUI_Interface::Redraw()
{
    if (m_backdropTexture->getSize().x != m_style[m_state].m_size.x || m_backdropTexture->getSize().y != m_style[m_state].m_size.y){
        m_backdropTexture->create(m_style[m_state].m_size.x, m_style[m_state].m_size.y);
    }
    m_backdropTexture->clear(sf::Color(0, 0, 0, 0));
    ApplyStyle();
    m_backdropTexture->draw(m_visual.m_backgroundSolid);
    if (!m_style[m_state].m_backgroundImage.empty()){
        m_backdropTexture->draw(m_visual.m_backgroundImage);
    }
    m_backdropTexture->display();
    m_backdrop.setTexture(m_backdropTexture->getTexture());
    m_backdrop.setTextureRect(sf::IntRect(0, 0, m_style[m_state].m_size.x, m_style[m_state].m_size.y));
    SetRedraw(false);
}

void GUI_Interface::RedrawContent()
{
    if (m_contentTexture->getSize().x != m_contentSize.x || m_contentTexture->getSize().y != m_contentSize.y){
        m_contentTexture->create(m_contentSize.x, m_contentSize.y);
    }
    m_contentTexture->clear(sf::Color(0, 0, 0, 0));
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()){
            continue;
        }
        element->ApplyStyle();
        element->Draw(m_contentTexture);
        element->SetRedraw(false);
    }
    m_contentTexture->display();
    m_content.setTexture(m_contentTexture->getTexture());
    m_content.setTextureRect(sf::IntRect(m_scrollHorizontal, m_scrollVertical, m_style[m_state].m_size.x, m_style[m_state].m_size.y));
    m_contentRedraw = false;
}

void GUI_Interface::RedrawControls()
{
    if (m_controlTexture->getSize().x != m_style[m_state].m_size.x || m_controlTexture->getSize().y != m_style[m_state].m_size.y){
        m_controlTexture->create(m_style[m_state].m_size.x, m_style[m_state].m_size.y);
    }
    m_controlTexture->clear(sf::Color(0, 0, 0, 0));
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || !element->IsControl()){
            continue;
        }
        element->ApplyStyle();
        element->Draw(m_controlTexture);
        element->SetRedraw(false);
    }
    m_controlTexture->display();
    m_control.setTexture(m_controlTexture->getTexture());
    m_control.setTextureRect(sf::IntRect(0, 0, m_style[m_state].m_size.x, m_style[m_state].m_size.y));
    m_controlRedraw = false;
}

void GUI_Interface::UpdateScrollHorizontal(const unsigned int& l_percent)
{
    if (l_percent > 100){
        return;
    }
    m_scrollHorizontal = ((m_contentSize.x - GetSize().x) / 100) * l_percent;
    sf::IntRect rect = m_content.getTextureRect();
    m_content.setTextureRect(sf::IntRect(m_scrollHorizontal, m_scrollVertical, rect.width, rect.height));
}

void GUI_Interface::UpdateScrollVertical(const unsigned int& l_percent)
{
    if (l_percent > 100){
        return;
    }
    m_scrollVertical = ((m_contentSize.y - GetSize().y) / 100) * l_percent;
    sf::IntRect rect = m_content.getTextureRect();
    m_content.setTextureRect(sf::IntRect(m_scrollHorizontal, m_scrollVertical, rect.width, rect.height));
}

void GUI_Interface::AdjustContentSize(const GUI_Element *l_reference)
{
    if (l_reference){
        sf::Vector2f bottomRight = l_reference->GetPosition() + l_reference->GetSize();
        if (bottomRight.x > m_contentSize.x){
            m_contentSize.x = bottomRight.x;
            m_controlRedraw = true;
        }
        if (bottomRight.y > m_contentSize.y){
            m_contentSize.y = bottomRight.y;
            m_controlRedraw = true;
        }
        return;
    }
    sf::Vector2f farthest = GetSize();
    for (auto &itr : m_elements){
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()){
            continue;
        }
        sf::Vector2f bottomRight = element->GetPosition() + element->GetSize();
        if (bottomRight.x > farthest.x){
            farthest.x = bottomRight.x;
            m_controlRedraw = true;
        }
        if (bottomRight.y > farthest.y){
            farthest.y = bottomRight.y;
            m_controlRedraw = true;
        }
    }
    SetContentSize(farthest);
}
