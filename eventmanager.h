#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include "gui_event.h"

enum class EventType{
    KeyDown = sf::Event::KeyPressed,
    KeyUp = sf::Event::KeyReleased,
    MButtonDown = sf::Event::MouseButtonPressed,
    MButtonUp = sf::Event::MouseButtonReleased,
    MouseWheel = sf::Event::MouseWheelMoved,
    WindowResized = sf::Event::Resized,
    GainedFocus = sf::Event::GainedFocus,
    LostFocus = sf::Event::LostFocus,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    Closed = sf::Event::Closed,
    TextEntered = sf::Event::TextEntered,
    Keyboard = sf::Event::Count + 1, Mouse, Joystick,

    GUI_Click, GUI_Release, GUI_Hover, GUI_Leave,
    GUI_ReleaseInside /* my own */
};

struct EventInfo{
    EventInfo() { m_code = 0; }
    EventInfo(int l_event) { m_code = l_event; }
    EventInfo(GUI_Event l_guiEvent) { m_guiEvent = l_guiEvent; }

    union{
        int m_code; /// ex. f5 key code
        GUI_Event m_guiEvent;
    };
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails{
    EventDetails(const std::string& l_bindName) : m_name(l_bindName){
        Clear();
    }
    std::string m_name;
    sf::Vector2i m_size;
    sf::Uint32 m_textEntered;

    sf::Vector2i m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode; // Single key code.

    std::string m_guiInterface; // GUI interface name.
    std::string m_guiElement; // GUI element name.
    GUI_EventType m_guiEvent; // GUI event type.

    void Clear(){
        m_size = sf::Vector2i(0, 0);
        m_textEntered = 0;
        m_mouse = sf::Vector2i(0, 0);
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
        m_guiElement.clear();
        m_guiInterface.clear();
        m_guiEvent = GUI_EventType::None;
    }
};

struct Binding{
    Binding(const std::string& l_name) : m_name(l_name), m_details(l_name), c(0){}
    ~Binding() {
        for (auto itr = m_events.begin(); itr != m_events.end(); ++itr){
            if (itr->first == EventType::GUI_Click ||
            itr->first == EventType::GUI_Release ||
            itr->first == EventType::GUI_Hover ||
            itr->first == EventType::GUI_Leave){
                delete [] itr->second.m_guiEvent.m_interface;
                delete [] itr->second.m_guiEvent.m_element;
            }
        }
    }

    void BindEvent(EventType l_type, EventInfo l_info = EventInfo()){
        m_events.emplace_back(l_type, l_info);
    }


    Events m_events;  //std::vector<std::pair<EventType, EventInfo>>;

    std::string m_name;
    int c; // Count of events that are "happening".

    EventDetails m_details;
};

using Bindings = std::unordered_map<std::string, Binding*>;

using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;


class EventManager
{
public:
    EventManager();
    ~EventManager();

    bool AddBinding(Binding *l_binding);
    bool RemoveBinding(std::string l_name);

    void SetFocus(const bool& l_focus) { m_hasFocus = l_focus; }

    // Needs to be defined in the header!
    template<class T>
    bool AddCallback(StateType l_state, const std::string& l_name, void(T::*l_func)(EventDetails*), T* l_instance){
        auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;
        auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
        return itr->second.emplace(l_name, temp).second;
    }

    bool RemoveCallback(StateType l_state, const std::string& l_name){
        auto itr = m_callbacks.find(l_state);
        if (itr == m_callbacks.end()){ return false; }
        auto itr2 = itr->second.find(l_name);
        if (itr2 == itr->second.end()){ return false; }
        itr->second.erase(l_name);
        return true;
    }

    void HandleEvent(sf::Event& l_event);
    void HandleEvent(GUI_Event& l_event);
    void Update();
    sf::Vector2i GetMousePos(sf::RenderWindow* l_wind = nullptr){
        return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
    }
    inline void SetCurrentState(StateType l_state) { m_currentState = l_state; }
private:
    void LoadBindings();
    Bindings m_bindings; /// from file
    Callbacks m_callbacks; /// from code
    bool m_hasFocus;
    StateType m_currentState;
};

#endif // EVENTMANAGER_H
