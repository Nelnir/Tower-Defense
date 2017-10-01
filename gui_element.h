#ifndef GUI_STYLE_H
#define GUI_STYLE_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

enum class GUI_ElementState { Neutral, Focused, Clicked, Locked, Count};

struct GUI_Style{
    GUI_Style() : m_textSize(12), m_textCenterOrigin(false), m_backgroundImageColor(255, 255, 255, 255), m_imageRect(0, 0, 0, 0)
    {
        sf::Color none = {0, 0, 0, 0};
        m_backgroundColor = m_elementColor = m_textColor = none;
    }

    sf::Vector2f m_size; // Element size.
    sf::Vector2f m_imageSize;
    // Background properties.
    sf::Color m_backgroundColor;
    sf::Color m_elementColor;
    std::string m_backgroundImage;
    sf::Color m_backgroundImageColor;
    // Text properties.
    sf::Color m_textColor;
    std::string m_textFont;
    sf::Vector2f m_textPadding;
    unsigned int m_textSize;
    bool m_textCenterOrigin;
    // Glyph properties.
    std::string m_glyph;
    sf::Vector2f m_glyphPadding;
    sf::IntRect m_imageRect;
};

struct GUI_Visual{
    sf::RectangleShape m_backgroundSolid;
    sf::Sprite m_backgroundImage;
    sf::Sprite m_glyph;
    sf::Text m_text;
};

enum class GUI_ElementType { Window, Label, Button, Scrollbar, Textfield, None};

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;
class GUI_Element{
    friend class GUI_Interface;
public:
    GUI_Element(const std::string& l_name, const GUI_ElementType& l_type, GUI_Interface* l_owner);
    virtual ~GUI_Element();
    // Event methods.
    virtual void ReadIn(std::stringstream& l_stream) = 0;
    virtual void OnClick(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnRelease() = 0;
    virtual void OnHover(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnLeave() = 0;
    virtual void Update(const float& l_dT) = 0;
    virtual void Draw(sf::RenderTarget* l_target) = 0;

    virtual void UpdateStyle(const GUI_ElementState& l_state, const GUI_Style& l_style);
    virtual void ApplyStyle();
    virtual void UpdateBgStyle(const GUI_ElementState& l_state, const GUI_Style& l_style);

    void SetState(const GUI_ElementState& l_state);
    void SetPosition(const sf::Vector2f& l_position);
    void SetName(const std::string& l_name) { m_name = l_name; }
    void SetOwner(GUI_Interface* l_owner) { m_owner = l_owner; }
    bool IsInside(const sf::Vector2f & l_point) const;
    inline sf::Vector2f GetPosition() const { return m_position; }
    sf::Vector2f GetGlobalPosition() const;
    const sf::Vector2f& GetSize() const;
    sf::Vector2f GetTextSize() const;
    void SetActive(const bool& l_active);
    std::string GetText() const;
    void SetText(const std::string& l_text);
    void SetRedraw(const bool& l_redraw) { m_needsRedraw = l_redraw; }
    bool IsControl() const { return m_isControl; }
    bool IsActive() const { return m_active; }
    bool NeedsRedraw() const { return m_needsRedraw; }
    GUI_ElementState GetState() { return m_state; }
    GUI_ElementType GetType() { return m_type; }
    GUI_Style GetStyle(const GUI_ElementState& l_state) { return m_style[l_state]; }
    std::string GetName() { return m_name; }

    friend std::stringstream& operator >>(std::stringstream& l_stream, GUI_Element& b){
        b.ReadIn(l_stream);
        return l_stream;
    }
protected:
    void ApplyTextStyle();
    void ApplyBgStyle();
    void ApplyGlyphStyle();
    void RequireTexture(const std::string& l_name);
    void RequireFont(const std::string& l_name);
    void ReleaseTexture(const std::string& l_name);
    void ReleaseFont(const std::string& l_name);
    void ReleaseResources();
    std::string m_name;
    sf::Vector2f m_position;
    ElementStyles m_style; // Style of drawables.
    GUI_Visual m_visual; // Drawable bits.
    GUI_ElementType m_type;
    GUI_ElementState m_state;
    GUI_Interface* m_owner;
    bool m_needsRedraw;
    bool m_active;
    bool m_isControl;
};

#endif // GUI_STYLE_H
