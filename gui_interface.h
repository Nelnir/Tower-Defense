#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "gui_element.h"

using Elements = std::unordered_map<std::string, GUI_Element*>;

class GUI_Manager;
class GUI_Interface : public GUI_Element
{
    friend class GUI_Manager;
    friend class GUI_Element;
public:
    GUI_Interface(const std::string& l_name, GUI_Manager* l_guiManager);
    ~GUI_Interface();
    GUI_Manager* GetManager() { return m_guiManager; }
    sf::Vector2f GetPadding() { return m_elementPadding; }

    void SetPosition(const sf::Vector2f &l_position);
    bool AddElement(const GUI_ElementType& l_type, const std::string& l_name);
    GUI_Element* GetElement(const std::string& l_name) const;
    bool RemoveElement(const std::string& l_name);
    bool IsInside(const sf::Vector2f &l_point) const;
    bool IsBeingMoved() const { return m_beingMoved; }
    void Focus() { m_focused = true; }
    bool IsFocused() { return m_focused; }
    bool NeedsContentRedraw() { return m_contentRedraw; }
    bool NeedsControlRedraw() { return m_controlRedraw; }
    void SetContentSize(const sf::Vector2f& l_size) { m_contentSize = l_size; }
    void SetContentRedraw(const bool& l_redraw) { m_contentRedraw = l_redraw; }
    sf::Vector2f GetContentSize() { return m_contentSize; }
    void UpdateScrollHorizontal(const unsigned int& l_percent);
    void UpdateScrollVertical(const unsigned int& l_percent);


    void ReadIn(std::stringstream &l_stream);
    void OnClick(const sf::Vector2f &l_mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f &l_mousePos);
    void OnLeave();
    void Update(const float &l_dT);
    void Draw(sf::RenderTarget *l_target);

    void OnTextEntered(const char& l_char);
    sf::Vector2f GetGlobalPosition() const;
    void ApplyStyle();

private:
    void BeginMoving();
    void StopMoving() { m_beingMoved = false; }
    void DefocusTextfields();

    void Redraw();
    void RedrawContent();
    void RedrawControls();

    Elements m_elements;
    sf::Vector2f m_elementPadding;
    GUI_Interface* m_parent;
    GUI_Manager* m_guiManager;
    sf::RenderTexture* m_backdropTexture;
    sf::Sprite m_backdrop;
    // Movement.
    sf::RectangleShape m_titleBar;
    sf::Vector2f m_moveMouseLast;
    bool m_showTitleBar;
    bool m_movable;
    bool m_beingMoved;
    bool m_focused;
    // Variable size.
    void AdjustContentSize(const GUI_Element* l_reference = nullptr);
    sf::RenderTexture* m_contentTexture;
    sf::Sprite m_content;
    sf::Vector2f m_contentSize;
    int m_scrollHorizontal;
    int m_scrollVertical;
    bool m_contentRedraw;
    // Control layer.
    sf::RenderTexture* m_controlTexture;
    sf::Sprite m_control;
    bool m_controlRedraw;
   };

#endif // GUI_INTERFACE_H
