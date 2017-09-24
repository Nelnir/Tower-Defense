#include "gui_manager.h"
#include "eventmanager.h"
#include "window.h"
#include <fstream>
#include "utils.h"
#include <iostream>
#include "gui_label.h"
#include "gui_textfield.h"
#include "gui_scrollbar.h"
#include "sharedcontext.h"

GUI_Manager::GUI_Manager(EventManager* l_evMgr, SharedContext* l_shared) :
    m_eventMgr(l_evMgr),
    m_context(l_shared),
    m_currentState(StateType(0)),
    m_isPressed(false)
{
    RegisterElement<GUI_Label>(GUI_ElementType::Label);
    RegisterElement<GUI_Scrollbar>(GUI_ElementType::Scrollbar);
    RegisterElement<GUI_Textfield>(GUI_ElementType::Textfield);

    m_elemTypes.emplace("Label", GUI_ElementType::Label);
    m_elemTypes.emplace("Button", GUI_ElementType::Button);
    m_elemTypes.emplace("Scrollbar", GUI_ElementType::Scrollbar);
    m_elemTypes.emplace("TextField", GUI_ElementType::Textfield);
    m_elemTypes.emplace("Interface", GUI_ElementType::Window);

    m_eventMgr->AddCallback(StateType(0), "Mouse_Left", &GUI_Manager::HandleClick, this);
    m_eventMgr->AddCallback(StateType(0), "Mouse_Left_Release", &GUI_Manager::HandleRelease, this);
    m_eventMgr->AddCallback(StateType(0), "Text_Entered", &GUI_Manager::HandleTextEntered, this);
}

GUI_Manager::~GUI_Manager()
{
    m_eventMgr->RemoveCallback(StateType(0), "Mouse_Left");
    m_eventMgr->RemoveCallback(StateType(0), "Mouse_Left_Release");
    m_eventMgr->RemoveCallback(StateType(0), "Text_Entered");
    for (auto &itr : m_interfaces){
        for (auto &itr2 : itr.second){
            delete itr2.second;
        }
        itr.second.clear();
    }
    m_interfaces.clear();
}

bool GUI_Manager::AddInterface(const StateType& l_state, const std::string& l_name)
{
    auto& itr = m_interfaces.emplace(l_state, GUI_Interfaces()).first;

    GUI_Interface* temp = new GUI_Interface(l_name, this);
    if (itr->second.emplace(l_name, temp).second){
        return true;
    }

    delete temp;
    return false;
}
GUI_Interface* GUI_Manager::GetInterface(const StateType &l_state, const std::string &l_name)
{
    auto& itr = m_interfaces.find(l_state);
    if(itr == m_interfaces.end()){
        return nullptr;
    }
    auto& itr2 = itr->second.find(l_name);
    return (itr2 == itr->second.end() ? nullptr : itr2->second);
}

bool GUI_Manager::RemoveInterface(const StateType& l_state, const std::string& l_name)
{
    auto s = m_interfaces.find(l_state);
    if (s == m_interfaces.end()){
        return false;
    }
    auto i = s->second.find(l_name);
    if (i == s->second.end()){
        return false;
    }
    delete i->second;
    return s->second.erase(l_name);
}

void GUI_Manager::SetCurrentState(const StateType &l_state)
{
    if(m_currentState == l_state){
        return;
    }
    HandleRelease(nullptr);
    m_currentState = l_state;
}

void GUI_Manager::HandleClick(EventDetails* l_details)
{
    m_isPressed = true;

    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){
        return;
    }
    sf::Vector2i mousePos = m_eventMgr->GetMousePos(m_context->m_wind->getRenderWindow());
    for (auto itr = --state->second.end(); itr != --state->second.begin(); --itr) ///rend? rbegin?
    {
        if (!itr->second->IsInside(sf::Vector2f(mousePos))){
            continue;
        }
        if (!itr->second->IsActive()){
            return;
        }
        itr->second->OnClick(sf::Vector2f(mousePos));
        itr->second->Focus();
        return;
    }
}

void GUI_Manager::HandleRelease(EventDetails* l_details)
{
    m_isPressed = false;
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){
        return;
    }
    for (auto &itr : state->second){
        GUI_Interface* i = itr.second;
        if (!i->IsActive()){
            continue;
        }
        if (i->GetState() == GUI_ElementState::Clicked){
                i->OnRelease();
        }
        if (i->IsBeingMoved()){
            i->StopMoving();
        }
    }
}

void GUI_Manager::HandleTextEntered(EventDetails* l_details)
{
     auto& state = m_interfaces.find(m_currentState);
     if (state == m_interfaces.end()){
         return;
     }
     for (auto &itr : state->second){
         if (!itr.second->IsActive()){
             continue;
         }
         if (!itr.second->IsFocused()){
             continue;
         }
         itr.second->OnTextEntered(l_details->m_textEntered);
         return;
     }
}

bool GUI_Manager::PollEvent(GUI_Event& l_event)
{
    if(m_events[m_currentState].empty()){
        return false;
    }
    l_event = m_events[m_currentState].back();
    m_events[m_currentState].pop_back();
    return true;
}

void GUI_Manager::Update(const float& l_dT)
{
    sf::Vector2i mousePos = m_eventMgr->GetMousePos(m_context->m_wind->getRenderWindow());
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){
        return;
    }
    for (auto itr = --state->second.end(); itr != --state->second.begin(); --itr) /// rbegin? rend?
    {
        GUI_Interface* i = itr->second;
        if (!i->IsActive()){
            continue;
        }

        i->Update(l_dT);

        if (i->IsBeingMoved()){
            continue;
        }
        if (i->IsInside(sf::Vector2f(mousePos))){
            if (i->GetState() == GUI_ElementState::Neutral && !m_isPressed){
                i->OnHover(sf::Vector2f(mousePos));
            }
            return;
        } else if (i->GetState() == GUI_ElementState::Focused){
            i->OnLeave();
        }
    }
}

void GUI_Manager::Render(sf::RenderWindow* l_wind)
{
    auto state = m_interfaces.find(m_currentState);
    if (state == m_interfaces.end()){
        return;
    }
    for (auto &itr : state->second){
        GUI_Interface* i = itr.second;
        if (!i->IsActive()){ continue; }
        if (i->NeedsRedraw()){ i->Redraw(); }
        if (i->NeedsContentRedraw()){ i->RedrawContent(); }
        if (i->NeedsControlRedraw()){ i->RedrawControls(); }
        i->Draw(l_wind);
    }
}

GUI_Element* GUI_Manager::CreateElement(const GUI_ElementType &l_id, GUI_Interface *l_owner)
{
    if (l_id == GUI_ElementType::Window){
        return new GUI_Interface("", this);
    }
    auto f = m_factory.find(l_id);
    return (f != m_factory.end() ? f->second(l_owner) : nullptr);
}

bool GUI_Manager::LoadInterface(const StateType &l_state, const std::string &l_interface, const std::string &l_name)
{
    std::ifstream file;
    file.open(Utils::GetInterfacesDirectory() + '\\' + l_interface);
    if(!file.is_open()){
        std::cerr << "Error when loading interface: " << l_interface << std::endl;
        return false;
    }

    std::string InterfaceName;
    std::string line;

    while(std::getline(file, line) && !file.eof()){
        std::stringstream keystream(line);
        std::string key;
        keystream >> key;
        if(key == "Interface"){
            std::string style;
            keystream >> InterfaceName >> style;
            if (!AddInterface(l_state, l_name)){
                std::cout << "Failed adding interface: " << l_name << std::endl;
                return false;
            }
            GUI_Interface* i = GetInterface(l_state, l_name);
            keystream >> *i;
            if (!LoadStyle(style, i)){
                std::cerr << "Failed loading style file: " << style << " for interface " << l_name << std::endl;
            }
            i->SetContentSize(i->GetSize());
        }
        else if (key == "Element"){
            if (InterfaceName.empty()){
                std::cerr << "Error: 'Element' outside or before declaration of 'Interface'!" << std::endl;
                continue;
            }
            std::string type;
            std::string name;
            sf::Vector2f position;
            std::string style;
            keystream >> type >> name >> position.x >> position.y >> style;
            GUI_ElementType eType = StringToType(type);
            if (eType == GUI_ElementType::None){
                std::cout << "Unknown element('" << name<< "') type: '" << type << "'" << std::endl;
                continue;
            }
            GUI_Interface* i = GetInterface(l_state, l_name);
            if (!i){
                continue;
            }
            if (!i->AddElement(eType, name)){
                std::cerr << "Error when adding element: " << name << " to: " << InterfaceName << " :interface " << std::endl;
                continue;
            }
            GUI_Element* e = i->GetElement(name);
            keystream >> *e;
            e->SetPosition(position);
            if (!LoadStyle(style, e)){
                std::cout << "Failed loading style file: " << style << " for element " << name << std::endl;
                continue;
            }
        }
    }
    file.close();
}

GUI_ElementType GUI_Manager::StringToType(const std::string &l_string)
{
    auto itr = m_elemTypes.find(l_string);
    if(itr == m_elemTypes.end()){
        return GUI_ElementType::None;
    }
    return itr->second;
}

bool GUI_Manager::LoadStyle(const std::string &l_file, GUI_Element *l_element)
{
    std::ifstream file;
    file.open(Utils::GetStylesDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when opening file: " << l_file << std::endl;
        return false;
    }

    std::string currentState;
    GUI_Style ParentStyle;
    GUI_Style TemporaryStyle;

    std::string line;
    while(std::getline(file, line)){
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;

        if(type == "State"){
            if(!currentState.empty()){
                std::cerr << "Error: 'State' keyword found inside another state!" << std::endl;
                continue;
            }
            keystream >> currentState;
        } else if(type == "/State"){
            if(currentState.empty()){
                std::cerr << "Error: '/State' keyword found prior to 'State'!" << std::endl;
                continue;
            }
            GUI_ElementState state = GUI_ElementState::Neutral;
            if(currentState == "Hover") { state = GUI_ElementState::Focused; }
            else if(currentState == "Clicked") { state = GUI_ElementState::Clicked; }
            else if(currentState == "Locked") { state = GUI_ElementState::Locked; }

            if(state == GUI_ElementState::Neutral){
                ParentStyle = TemporaryStyle;
                for(int i = static_cast<int>(GUI_ElementState::Neutral); i < static_cast<int>(GUI_ElementState::Count); ++i){
                    l_element->UpdateStyle(static_cast<GUI_ElementState>(i), TemporaryStyle);
                }
            } else {
                l_element->UpdateStyle(state, TemporaryStyle);
            }
            TemporaryStyle = ParentStyle;
            currentState.clear();
        } else{ /// style informations
            if (currentState.empty()){
                std::cerr << "Error: '" << type << "' keyword found outside of a state!" << std::endl;
                continue;
            }

            if (type == "Size"){
                keystream >> TemporaryStyle.m_size.x >>TemporaryStyle.m_size.y;
                TemporaryStyle.m_imageSize = TemporaryStyle.m_size;
            } else if (type == "BgColor"){
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle.m_backgroundColor = sf::Color(r, g, b, a);
            } else if (type == "BgImage"){
                keystream >> TemporaryStyle.m_backgroundImage;
            } else if (type == "BgImageColor"){
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle.m_backgroundImageColor = sf::Color(r, g, b, a);
            } else if (type == "TextColor"){
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle.m_textColor = sf::Color(r, g, b, a);
            } else if (type == "TextSize"){
                keystream >> TemporaryStyle.m_textSize;
            } else if (type == "TextOriginCenter"){
                TemporaryStyle.m_textCenterOrigin = true;
            } else if (type == "Font"){
                keystream >> TemporaryStyle.m_textFont;
            } else if (type == "TextPadding"){
                keystream >> TemporaryStyle.m_textPadding.x >> TemporaryStyle.m_textPadding.y;
            } else if (type == "ElementColor"){
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle.m_elementColor = sf::Color(r, g, b, a);
            } else if (type == "Glyph"){
                keystream >> TemporaryStyle.m_glyph;
            } else if (type == "GlyphPadding"){
                keystream >> TemporaryStyle.m_glyphPadding.x >> TemporaryStyle.m_glyphPadding.y;
            } else if(type == "ImageSize"){
                keystream >> TemporaryStyle.m_imageSize.x >> TemporaryStyle.m_imageSize.y;
            } else if(type == "ImageRect"){
                keystream >> TemporaryStyle.m_imageRect.left >> TemporaryStyle.m_imageRect.top
                          >> TemporaryStyle.m_imageRect.width >> TemporaryStyle.m_imageRect.height;
            } else {
                std::cerr << "Error: style tag '" << type << "' is unknown!" << std::endl;
            }
        }
    }
}
