#ifndef ANIM_BASE_H
#define ANIM_BASE_H

#include <sstream>

class SpriteSheet;
using Frame = unsigned int;

class Anim_Base
{
    friend class SpriteSheet;
public:
    Anim_Base();
    virtual ~Anim_Base();

    inline void SetSpriteSheet(SpriteSheet* l_sheet) { m_spriteSheet = l_sheet; }
    inline void SetName(const std::string& l_name) { m_name = l_name; }
    inline void SetLooping(const bool& l_looping) { m_loop = l_looping; }
    inline std::string GetName() const { return m_name; }
    inline bool IsPlaying() const { return m_playing; }
    bool IsInAction();

    inline void Play() { m_playing = true; }
    inline void Pause() { m_playing = false; }
    inline void Stop() { m_playing = false; Reset();}
    void Reset();
    virtual void Update(const float& l_dT);
    friend std::stringstream& operator >>(std::stringstream& l_stream, Anim_Base& a)
    {
        a.ReadIn(l_stream);
        return l_stream;
    }
protected:
    virtual void FrameStep() = 0;
    virtual void CropSprite() = 0;
    virtual void ReadIn(std::stringstream& l_stream) = 0;

    void SetFrame(const unsigned int& l_frame);

    Frame m_frameCurrent;
    Frame m_frameStart;
    Frame m_frameEnd;
    Frame m_frameRow;
    int m_frameActionStart; // Frame when a specific "action" begins
    int m_frameActionEnd; // Frame when a specific "action" ends
    float m_frameTime;
    float m_elapsedTime;
    bool m_loop;
    bool m_playing;
    std::string m_name;
    SpriteSheet* m_spriteSheet;
};

#endif // ANIM_BASE_H
