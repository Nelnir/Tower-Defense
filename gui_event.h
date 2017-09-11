#ifndef GUI_EVENT_H
#define GUI_EVENT_H

enum class GUI_EventType { None, Click, Release, Hover, Leave,
                         ReleaseInside};

struct ClickCoordinates{
    float x, y;
};

struct GUI_Event{
    GUI_EventType m_type;
    const char* m_element;
    const char* m_interface;
    union{
        ClickCoordinates m_clickCoords;
    };
};

#endif // GUI_EVENT_H
