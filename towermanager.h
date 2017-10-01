#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H

#include "abstracttower.h"
#include "sharedcontext.h"
#include "statistics.h"
#include <unordered_map>
#include <functional>

using TowerFactory = std::unordered_map<Tower, std::function<AbstractTower*(TowerProporties*)>>;
using TowersProporties = std::unordered_map<Tower, TowerProporties*>;
using TowerID = unsigned int;
using Towers = std::unordered_map<TowerID, AbstractTower*>;

struct EventDetails;

class TowerManager
{
public:
    TowerManager(SharedContext* l_context, Statistics* l_statistics);
    ~TowerManager();
    TowerProporties* GetProporties(const Tower& l_type);
    void Pressed(TowerProporties* l_proporties);
    void Draw();
    void Update(const float& l_dT);

    void HandleRelease(EventDetails* l_details);
    void HandleKey(EventDetails* l_details);
    SharedContext* GetContext() { return m_context; }
    void Restart();
    void SetZoom(const float& l_zoom) { m_zoom = l_zoom; }
    void CloseUpgrades(EventDetails* l_details) { m_interface->SetActive(false); m_pressed = nullptr; }
    void UpgradeTower(EventDetails* l_details);
    void ChangeStrategy(EventDetails* l_details);
    void RefreshInterface();
private:
    void UpdateAttackStrategyGUI();
    void UpdateUpgradeGUI(UpgradeProporties* l_proporties);
    void ShowUpgradeInterfaceFor(AbstractTower* l_tower);
    template<class T>
    void RegisterTower(const Tower& l_type){
        m_towerFactory[l_type] = [this] (TowerProporties* l_prop) -> AbstractTower* { return new T(l_prop, this); };
    }
    void RegisterProporties(const Tower& l_type, TowerProporties* l_proporties){
        m_towerProporties[l_type] = l_proporties;
    }
    void Purge();
    TowerProporties* CreateTowerProporties(const Tower& l_type);
    void SetTextureForProporties(const std::string& l_texture, const sf::IntRect& l_rect, TowerProporties* l_proporties);
    void AddTower(TowerProporties* l_proporties, const sf::Vector2f& l_pos);


    SharedContext* m_context;
    TowerFactory m_towerFactory;
    TowersProporties m_towerProporties;
    TowerID m_countId;
    Towers m_towers;

    TowerProporties* m_placingTower;
    bool m_colliding;
    float m_zoom;
    Statistics* m_statistics;
    AbstractTower* m_pressed;
    GUI_Interface* m_interface;
};

#endif // TOWERMANAGER_H
