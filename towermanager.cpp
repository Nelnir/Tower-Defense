#include "towermanager.h"
#include "texturemanager.h"
#include "eventmanager.h"
#include "window.h"
#include "level.h"
#include "utils.h"
#include "enemymanager.h"
#include "abstracttower.h"
#include "gui_manager.h"

TowerManager::TowerManager(SharedContext* l_context, Statistics* l_statistics) : m_context(l_context),
    m_countId(0), m_placingTower(nullptr), m_colliding(false), m_zoom(0.f),
    m_statistics(l_statistics), m_pressed(nullptr)
{
    RegisterProporties(Tower::Basic, CreateTowerProporties(Tower::Basic));
    RegisterTower<AbstractTower>(Tower::Basic);

    GUI_Manager* guiMgr = m_context->m_guiManager;
    guiMgr->LoadInterface(StateType::Game, "TowerUpgrade.interface", "TowerUpgrade");
    m_interface = guiMgr->GetInterface(StateType::Game, "TowerUpgrade");
    m_interface->SetActive(false);

    m_context->m_eventManager->AddCallback(StateType::Game, "CloseUpgrades", &TowerManager::CloseUpgrades, this);
    m_context->m_eventManager->AddCallback(StateType::Game, "UpgradeTower", &TowerManager::UpgradeTower, this);
    m_context->m_eventManager->AddCallback(StateType::Game, "ChangeStrategy", &TowerManager::ChangeStrategy, this);
}

TowerManager::~TowerManager() { Purge(); }

void TowerManager::Purge()
{
    TextureManager* mgr = m_context->m_textureManager;
    for(auto& itr : m_towerProporties){
        mgr->ReleaseResource(itr.second->m_texture);
        delete itr.second;
        itr.second = nullptr;
    }
    m_towerProporties.clear();
    for(auto& itr : m_towers){
        delete itr.second;
    }
    m_towers.clear();
    m_context->m_eventManager->RemoveCallback(StateType::Game, "CloseUpgrades");
    m_context->m_eventManager->RemoveCallback(StateType::Game, "UpgradeTower");
    m_context->m_eventManager->RemoveCallback(StateType::Game, "ChangeStrategy");
    m_context->m_guiManager->RemoveInterface(StateType::Game, "TowerUpgrade");
}

TowerProporties* TowerManager::GetProporties(const Tower &l_type)
{
    auto itr = m_towerProporties.find(l_type);
    return (itr == m_towerProporties.end() ? nullptr : itr->second);
}

TowerProporties* TowerManager::CreateTowerProporties(const Tower &l_type)
{
    TowerProporties * proporties = new TowerProporties;
    switch(l_type)
    {
    case Tower::Basic:
        proporties->m_cost = 100;
        proporties->m_tower = Tower::Basic;
        proporties->m_attacking.set(static_cast<int>(EnemyType::Land));
        proporties->m_bulletType = BulletType::Normal;
        proporties->m_radiusCollision = 30.f;
        proporties->m_towerRotation = true;
        SetTextureForProporties("Tileset", sf::IntRect(1216, 640, 64, 64), proporties);
        UpgradeProporties upp;
        upp.m_cost = 0;
        upp.m_damage = 25;
        upp.m_firingRate = 2.f;
        upp.m_radius = 0.f;
        upp.m_bulletSpeed = 1000.f;
        proporties->m_upgrades.emplace_back(upp);
        upp.m_cost = 150;
        upp.m_damage = 1;
        upp.m_radius = 0.f;
        upp.m_firingRate = 250;
        upp.m_bulletSpeed = 1250.f;
        proporties->m_upgrades.emplace_back(upp);
        break;
    default:
        delete proporties;
        proporties = nullptr;
        break;
    }
    return proporties;
}

void TowerManager::SetTextureForProporties(const std::string &l_texture, const sf::IntRect &l_rect, TowerProporties *l_proporties)
{
    TextureManager* mgr = m_context->m_textureManager;
    mgr->RequireResource(l_texture);
    l_proporties->m_texture = l_texture;
    l_proporties->m_sprite.setTexture(*mgr->GetResource(l_texture));
    l_proporties->m_sprite.setTextureRect(l_rect);
    l_proporties->m_sprite.setOrigin(sf::Vector2f(l_rect.width / 2.f, l_rect.height / 2.f));
}

void TowerManager::UpgradeTower(EventDetails *l_details)
{
    m_pressed->Upgrade();
    ShowUpgradeInterfaceFor(m_pressed);
    m_context->m_level->AddMoney(-m_pressed->GetUpgradeProporties().m_cost);
}

void TowerManager::RefreshInterface()
{
    if(!m_pressed){
        return;
    }
    UpdateUpgradeGUI(m_pressed->GetNextUpgradeProporties());
}

void TowerManager::ChangeStrategy(EventDetails *l_details)
{
    switch(m_pressed->GetStrategy())
    {
    case AttackStrategy::First:
        m_pressed->SetStrategy(AttackStrategy::Last);
        break;
    case AttackStrategy::Last:
        m_pressed->SetStrategy(AttackStrategy::First);
        break;
    }
    UpdateAttackStrategyGUI();
}

void TowerManager::Pressed(TowerProporties *l_proporties)
{
    m_placingTower = l_proporties;
}

void TowerManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_towers){
        AbstractTower* tower = itr.second;
        tower->GetProporties()->m_sprite.setPosition(tower->GetPosition());
        tower->Draw(window);
    }

    if(m_placingTower){
        const sf::Vector2i mousePos = m_context->m_eventManager->GetMousePos(m_context->m_wind->getRenderWindow());
        const float& radiusC = m_placingTower->m_radiusCollision;
        const sf::Vector2f currPlacingTowerPos = sf::Vector2f(mousePos.x * m_zoom - radiusC, mousePos.y * m_zoom - radiusC);
        sf::CircleShape range;

        /// Check collison with other towers and draw thei collision circle
        bool collidingWithTowers = false;
        range.setFillColor(sf::Color(0, 0, 0, 128));
        for(auto& itr : m_towers){
            const float& radius = itr.second->GetProporties()->m_radiusCollision;
            range.setRadius(radius);
            sf::Vector2f pos = itr.second->GetPosition();
            range.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius));
            if(Utils::CirclesColliding(radius, range.getPosition(), radiusC, currPlacingTowerPos)){
                range.setFillColor(sf::Color(255, 0, 0, 128));
                collidingWithTowers = m_colliding = true;
            } else{
                range.setFillColor(sf::Color(0, 0, 0, 128));
            }
            window->draw(range);
        }

        const float& AttackRadius = m_placingTower->m_upgrades.front().m_radius;
        /// draw current placing tower attak circle
        range.setFillColor(sf::Color(0, 0, 0, 128));
        range.setRadius(AttackRadius);
        range.setPosition(sf::Vector2f(mousePos.x * m_zoom - AttackRadius, mousePos.y * m_zoom - AttackRadius));
        window->draw(range);

        /// draw current placing tower collision circle
        range.setRadius(radiusC);
        range.setPosition(currPlacingTowerPos);
        if(m_context->m_level->IsOutsideMap(range) || m_context->m_level->CollideWithPath(range)){ m_colliding = true; }
        else if (!collidingWithTowers){ m_colliding = false; }

        if(m_colliding){
            range.setFillColor(sf::Color(255, 0, 0, 128));
        } else {
            range.setFillColor(sf::Color(0, 255, 0, 128));
        }
        window->draw(range);

        /// draw current placing tower sprite
        m_placingTower->m_sprite.setPosition(sf::Vector2f(mousePos.x * m_zoom, mousePos.y * m_zoom));
        m_placingTower->m_sprite.setRotation(0);
        window->draw(m_placingTower->m_sprite);
    }
    if(m_pressed){
        sf::CircleShape range;
        range.setRadius(m_pressed->GetUpgradeProporties().m_radius);
        range.setPosition(m_pressed->GetPosition().x - range.getRadius(), m_pressed->GetPosition().y - range.getRadius());
        range.setFillColor(sf::Color(0, 0, 0, 128));
        window->draw(range);
    }
}

void TowerManager::Update(const float& l_dT)
{
    for(auto& itr : m_towers){
        AbstractTower* tower = itr.second;
        tower->Update(l_dT);
        tower->SetEnemy(m_context->m_enemyManager->GetEnemyFor(itr.second));
        tower->m_shootElapsed += l_dT;
        if(tower->m_lookinAt && tower->m_shootElapsed >= 1.f / tower->GetUpgradeProporties().m_firingRate){
            tower->m_shootElapsed = 0.f;
            tower->Shot(tower->m_lookinAt);
        }
    }
}

void TowerManager::HandleRelease(EventDetails *l_details)
{
    if(l_details->m_name == "Mouse_Right_Release"){
        m_placingTower = nullptr;
        m_pressed = nullptr;
        m_interface->SetActive(false);
        return;
    }
    if(m_placingTower){
        if(m_colliding){
            m_placingTower = nullptr;
            return;
        }
        m_context->m_level->AddMoney(-m_placingTower->m_cost);
        AddTower(m_placingTower, m_placingTower->m_sprite.getPosition());
        m_placingTower =
                nullptr;
    } else {
        sf::Vector2i mousePos = sf::Vector2i(l_details->m_mouse.x * m_zoom, l_details->m_mouse.y * m_zoom);
        for(auto& itr : m_towers){
            AbstractTower* tower = itr.second;
            if(Utils::PointInsideCircle(mousePos, tower->GetPosition(), tower->GetProporties()->m_radiusCollision)){
                m_pressed = tower;
                ShowUpgradeInterfaceFor(m_pressed);
                return;
            }
        }
    }
}

void TowerManager::UpdateAttackStrategyGUI()
{
    m_interface->GetElement("%AttackStrategy%")->SetText((m_pressed->GetStrategy() == AttackStrategy::First ? "First" : "Last"));
}

void TowerManager::UpdateUpgradeGUI(UpgradeProporties *nextUpr)
{
    if(nextUpr){
        if(nextUpr->m_cost > m_context->m_level->GetMoney()){
            m_interface->GetElement("Upgrade")->SetState(GUI_ElementState::Locked);
        } else{
            m_interface->GetElement("Upgrade")->SetState(GUI_ElementState::Neutral);
        }
    } else{
        m_interface->GetElement("Upgrade")->SetState(GUI_ElementState::Locked);
    }
}

void TowerManager::ShowUpgradeInterfaceFor(AbstractTower *l_tower)
{
    m_interface->SetActive(true);
    const float& radius = l_tower->GetProporties()->m_radiusCollision;
    const sf::Vector2f pos = sf::Vector2f((l_tower->GetPosition().x + radius) / m_zoom, (l_tower->GetPosition().y + radius) / m_zoom);
    m_interface->SetPosition(pos);

    const UpgradeProporties& upr = l_tower->GetUpgradeProporties();
    m_interface->GetElement("Upgrades")->SetText("Upgrade: " + std::to_string(l_tower->GetCurrentUpgrade()) +
                                                " / " + std::to_string(l_tower->GetNumberOfUpgrades() - 1));
    m_interface->GetElement("Damage")->SetText("Damage: " + std::to_string(upr.m_damage));
    std::string string = std::to_string(upr.m_firingRate);
    string.erase(string.length() - 4, 4);
    m_interface->GetElement("FiringRate")->SetText("Firing rate: " + string);
    string = std::to_string(upr.m_radius);
    string.erase(string.length() - 4, 4);
    m_interface->GetElement("Radius")->SetText("Range: " + string);
    string = std::to_string(upr.m_bulletSpeed);
    string.erase(string.length() - 7, 7);
    m_interface->GetElement("BulletSpeed")->SetText("Bullet speed: " + string);

    UpdateAttackStrategyGUI();
    UpgradeProporties* nextUpr = l_tower->GetNextUpgradeProporties();
    UpdateUpgradeGUI(nextUpr);
    if(nextUpr){
        m_interface->GetElement("Cost")->SetText("Cost: " + std::to_string(nextUpr->m_cost) + m_context->m_settings->GetCurrency());

        GUI_Element* ele = m_interface->GetElement("Damage");
        GUI_Element* ele2 = m_interface->GetElement("Damage+");
        ele2->SetText("-> " + std::to_string(nextUpr->m_damage));
        ele2->SetPosition(sf::Vector2f(ele->GetPosition().x + ele->GetTextSize().x + 5, ele2->GetPosition().y));
        ele = m_interface->GetElement("FiringRate");
        ele2 = m_interface->GetElement("FiringRate+");
        std::string string = std::to_string(nextUpr->m_firingRate);
        string.erase(string.length() - 4, 4);
        ele2->SetText("-> " + string);
        ele2->SetPosition(sf::Vector2f(ele->GetPosition().x + ele->GetTextSize().x + 5, ele2->GetPosition().y));
        ele = m_interface->GetElement("Radius");
        ele2 = m_interface->GetElement("Radius+");
        string = std::to_string(nextUpr->m_radius);
        string.erase(string.length() - 4, 4);
        ele2->SetText("-> " + string);
        ele2->SetPosition(sf::Vector2f(ele->GetPosition().x + ele->GetTextSize().x + 5, ele2->GetPosition().y));
        ele = m_interface->GetElement("BulletSpeed");
        ele2 = m_interface->GetElement("BulletSpeed+");
        string = std::to_string(nextUpr->m_bulletSpeed);
        string.erase(string.length() - 7, 7);
        ele2->SetText("-> " + string);
        ele2->SetPosition(sf::Vector2f(ele->GetPosition().x + ele->GetTextSize().x + 5, ele2->GetPosition().y));

    } else{
        m_interface->GetElement("Cost")->SetText("Cost: ---");
        m_interface->GetElement("BulletSpeed+")->SetText("");
        m_interface->GetElement("Radius+")->SetText("");
        m_interface->GetElement("FiringRate+")->SetText("");
        m_interface->GetElement("Damage+")->SetText("");
    }
}

void TowerManager::HandleKey(EventDetails* l_details)
{
    if(l_details->m_name == "Key_ESC"){
        m_placingTower = nullptr;
    }
}

void TowerManager::AddTower(TowerProporties *l_proporties, const sf::Vector2f& l_pos)
{
    auto itr = m_towerFactory.find(l_proporties->m_tower);
    if(itr == m_towerFactory.end()){
        return;
    }
    AbstractTower* tower = m_towers.emplace(m_countId++, itr->second(l_proporties)).first->second;
    tower->SetPosition(l_pos);
    m_statistics->AddTowersPlaced();
}

void TowerManager::Restart()
{
    for(auto& itr : m_towers){
        delete itr.second;
    }
    m_towers.clear();
    m_pressed = nullptr;
    m_placingTower = nullptr;
    m_interface->SetActive(false);
}
