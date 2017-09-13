#ifndef BASESTATE_H
#define BASESTATE_H

#include <SFML/Graphics.hpp>

enum class StateType {Intro = 1, MainMenu, Game, Paused, GameOver, Credits, About, Settings};

class StateManager;
class BaseState
{
    friend class StateManager;
public:
    BaseState(StateManager* l_stateManager);
    virtual ~BaseState(){}
    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void Update(const sf::Time& l_time) = 0;
    virtual void Draw() = 0;
    inline void SetTransparent(const bool& l_transparent){ m_transparent = l_transparent; }
    inline bool IsTransparent()const{ return m_transparent; }
    inline void SetTranscendent(const bool& l_transcendence){ m_transcendent = l_transcendence; }
    inline bool IsTranscendent()const{ return m_transcendent; }
    inline StateManager* GetStateManager(){ return m_stateMgr; }
    inline sf::View& GetView() { return m_view; }
protected:
    StateManager* m_stateMgr;
    bool m_transparent;
    bool m_transcendent;
    sf::View m_view;
};

#endif // BASESTATE_H
