#ifndef SHAREDCONTEXT_H
#define SHAREDCONTEXT_H

class StateManager;
class Window;
class EventManager;
class TextureManager;
class FontManager;
class GUI_Manager;
class Settings;
class Level;
class TowerManager;
class EnemyManager;
class StateManager;
class Statistics;

struct SharedContext{
    SharedContext():m_wind(nullptr),m_eventManager(nullptr), m_textureManager(nullptr), m_fontManager(nullptr), m_guiManager(nullptr), m_level(nullptr),
    m_towerManager(nullptr), m_enemyManager(nullptr), m_stateMgr(nullptr), m_statistics(nullptr) {}
    Window* m_wind;
    EventManager* m_eventManager;
    TextureManager* m_textureManager;
    FontManager* m_fontManager;
    GUI_Manager* m_guiManager;
    Settings* m_settings;
    Level* m_level;
    TowerManager* m_towerManager;
    EnemyManager* m_enemyManager;
    StateManager* m_stateMgr;
    Statistics* m_statistics;
};

#endif // SHAREDCONTEXT_H
