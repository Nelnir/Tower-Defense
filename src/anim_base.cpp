#include "anim_base.h"
#include "spritesheet.h"

Anim_Base::Anim_Base() :
    m_frameCurrent(0),
    m_frameStart(0),
    m_frameEnd(0),
    m_frameRow(0),
    m_frameTime(0.f),
    m_elapsedTime(0.f),
    m_frameActionStart(-1),
    m_frameActionEnd(-1),
    m_loop(false),
    m_playing(false){}
Anim_Base::~Anim_Base(){}

void Anim_Base::SetFrame(const unsigned int& l_frame)
{
    if((l_frame >= m_frameStart && l_frame <= m_frameEnd) || (l_frame >= m_frameEnd && l_frame <= m_frameStart)){
        m_frameCurrent = l_frame;
    }
}

bool Anim_Base::IsInAction()
{
    if(m_frameActionStart == -1 || m_frameActionEnd == -1){
        return true;
    }
    return (m_frameCurrent >= m_frameActionStart && m_frameCurrent <= m_frameActionEnd);
}

void Anim_Base::Reset()
{
    m_frameCurrent = m_frameStart;
    m_elapsedTime = 0.0f;
    CropSprite();
}

void Anim_Base::Update(const float& l_dT)
{
    if (!m_playing){ return; }
    m_elapsedTime += l_dT;
    if (m_elapsedTime < m_frameTime){ return; }
    FrameStep();
    CropSprite();
    m_elapsedTime = 0;
}
