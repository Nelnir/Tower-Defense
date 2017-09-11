#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "gui_interface.h"
#include "basestate.h"
#include "gui_event.h"
#include "eventmanager.h"
#include <functional>

using GUI_Interfaces = std::unordered_map<std::string, GUI_Interface*>;
using GUI_Container = std::unordered_map<StateType, GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType, std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType, std::function<GUI_Element*(GUI_Interface*)>>;
using GUI_ElemTypes = std::unordered_map<std::string, GUI_ElementType>;


struct SharedContext; // Forward declaration.
class GUI_Manager{
    friend class GUI_Interface;
public:
    GUI_Manager(EventManager* l_evMgr, SharedContext* l_context);
    ~GUI_Manager();

    SharedContext* GetContext() { return m_context; }
    void SetCurrentState(const StateType& l_state);

    bool AddInterface(const StateType& l_state, const std::string& l_name);
    GUI_Interface* GetInterface(const StateType& l_state, const std::string& l_name);
    bool RemoveInterface(const StateType& l_state, const std::string& l_name);
    void AddEvent(const GUI_Event& l_event) { m_events[m_currentState].push_back(l_event); }
    bool PollEvent(GUI_Event& l_event);
    void Update(const float& l_dT);
    void Render(sf::RenderWindow* l_wind);

    void HandleClick(EventDetails* l_details);
    void HandleRelease(EventDetails* l_details);
    void HandleTextEntered(EventDetails* l_details);

    template<class T>
    void RegisterElement(const GUI_ElementType& l_id){
        m_factory[l_id] = [](GUI_Interface* l_owner) -> GUI_Element*
        { return new T("", l_owner); };
    }

    bool LoadInterface(const StateType& l_state, const std::string& l_interface, const std::string& l_name);
private:
    GUI_Element* CreateElement(const GUI_ElementType& l_id, GUI_Interface* l_owner);
    GUI_ElementType StringToType(const std::string& l_string);
    bool LoadStyle(const std::string& l_file, GUI_Element* l_element);
    GUI_Container m_interfaces;
    GUI_Events m_events;
    EventManager * m_eventMgr;
    SharedContext* m_context;
    StateType m_currentState;
    GUI_Factory m_factory;
    GUI_ElemTypes m_elemTypes;
};

#endif // GUI_MANAGER_H
