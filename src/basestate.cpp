#include "basestate.h"

BaseState::BaseState(StateManager *l_stateManager) :
    m_stateMgr(l_stateManager),
    m_transparent(false),
    m_transcendent(false)
{

}
