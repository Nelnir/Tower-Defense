#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H

#include "TowerBase.h"
#include "sharedcontext.h"
#include "statistics.h"
#include <unordered_map>
#include <functional>

using TowerFactory = std::unordered_map<Tower, std::function<std::shared_ptr<TowerBase>(const std::shared_ptr<TowerProporties>&)>>;
using TowersProporties = std::unordered_map<Tower, std::shared_ptr<TowerProporties>>;
using TowerID = unsigned int;
using Towers = std::vector<std::shared_ptr<TowerBase>>;

struct EventDetails;
class TowerManager
{
public:
    TowerManager(SharedContext* l_context, Statistics* l_statistics);
    ~TowerManager();
    std::shared_ptr<TowerProporties> GetProporties(const Tower& l_type);
    void Pressed(const std::shared_ptr<TowerProporties>& l_proporties);
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
    void DeleteTower(EventDetails* l_details);
    void RefreshInterface();
private:
    void LoadConfigFile(const std::string& l_file);
    void LoadProportiesFor(const Tower& l_type, const std::string& l_file);
    void UpdateAttackStrategyGUI();
    void UpdateUpgradeGUI(UpgradeProporties* l_proporties);
    void ShowUpgradeInterfaceFor(const std::shared_ptr<TowerBase>& l_tower);
    template<class T>
    void RegisterTower(const Tower& l_type){
        m_towerFactory[l_type] = [this] (const std::shared_ptr<TowerProporties>& l_prop) -> std::shared_ptr<TowerBase> {
            return std::move(std::make_shared<T>(l_prop, this));
        };
    }
    void RegisterProporties(const Tower& l_type, const std::shared_ptr<TowerProporties>& l_proporties){
        m_towerProporties[l_type] = l_proporties;
    }
    void Purge();
    void SetTextureForProporties(const std::string& l_texture, const sf::IntRect& l_rect, const std::shared_ptr<TowerProporties>& l_proporties);
    void AddTower(const std::shared_ptr<TowerProporties>& l_proporties, const sf::Vector2f& l_pos);


    SharedContext* m_context;
    TowerFactory m_towerFactory;
    TowersProporties m_towerProporties;
    Towers m_towers;

    std::shared_ptr<TowerProporties> m_placingTower;
    bool m_colliding;
    float m_zoom;
    Statistics* m_statistics;
    std::shared_ptr<TowerBase> m_pressed;
    GUI_Interface* m_interface;
};

#endif // TOWERMANAGER_H
