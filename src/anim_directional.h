#ifndef ANIM_DIRECTIONAL_H
#define ANIM_DIRECTIONAL_H

#include "anim_base.h"

class Anim_Directional : public Anim_Base
{
protected:
    void FrameStep();
    void CropSprite();
    void ReadIn(std::stringstream& l_stream);
};

#endif // ANIM_DIRECTIONAL_H
