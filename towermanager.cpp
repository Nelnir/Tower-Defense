#include "towermanager.h"
#include "texturemanager.h"
#include "eventmanager.h"
#include "window.h"
#include "level.h"
#include "utils.h"
#include "enemymanager.h"
#include "TowerBase.h"
#include "gui_manager.h"
#include "tower_missile.h"

TowerManager::TowerManager(SharedContext* l_context, Statistics* l_statistics) : m_context(l_context),
    m_placingTower(nullptr), m_colliding(false),
    m_statistics(l_statistics), m_pressed(nullptr)
{
    RegisterTower<TowerBase>(Tower::Basic);
    RegisterTower<TowerBase>(Tower::BasicPRO);
    RegisterTower<Tower_Missile>(Tower::Missile);
    LoadConfigFile("towers.cfg");

    GUI_Manager* guiMgr = m_context->m_guiManager;
    guiMgr->LoadInterface(StateType::Game, "TowerUpgrade.interface", "TowerUpgrade");
    m_interface = guiMgr->GetInterface(StateType::Game, "TowerUpgrade");
    m_interface->SetActive(false);

    m_context->m_eventManager->AddCallback(StateType::Game, "CloseUpgrades", &TowerManager::CloseUpgrades, this);
    m_context->m_eventManager->AddCallback(StateType::Game, "UpgradeTower", &TowerManager::UpgradeTower, this);
    m_context->m_eventManager->AddCallback(StateType::Game, "ChangeStrategy", &TowerManager::ChangeStrategy, this);
    m_context->m_eventManager->AddCallback(StateType::Game, "DeleteTower", &TowerManager::DeleteTower, this);
}

TowerManager::~TowerManager() { Purge(); }

void TowerManager::Purge()
{
    TextureManager* mgr = m_context->m_textureManager;
    for(auto& itr : m_towerProporties){
        mgr->ReleaseResource(itr.second->m_texture);
        if(itr.second->m_tower == Tower::Missile || itr.second->m_tower == Tower::MissilePRO){
            auto missile = std::dynamic_pointer_cast<TowerMissileProporties>(itr.second);
            mgr->ReleaseResource(missile->m_missileTexture);
        }
    }
    m_towerProporties.clear();
    m_towers.clear();
    m_context->m_eventManager->RemoveCallback(StateType::Game, "CloseUpgrades");
    m_context->m_eventManager->RemoveCallback(StateType::Game, "UpgradeTower");
    m_context->m_eventManager->RemoveCallback(StateType::Game, "ChangeStrategy");
    m_context->m_eventManager->RemoveCallback(StateType::Game, "DeleteTower");
    m_context->m_guiManager->RemoveInterface(StateType::Game, "TowerUpgrade");
}

void TowerManager::LoadConfigFile(const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetTowersDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when opening towers config file: " << l_file << std::endl;
    }

    std::string line;
    while(std::getline(file, line)){
        std::stringstream keystream(line);
        int towerType;
        std::string path;
        keystream >> towerType >> path;
        LoadProportiesFor(static_cast<Tower>(towerType), path);
    }
    file.close();
}

void TowerManager::LoadProportiesFor(const Tower &l_type, const std::string &l_file)
{
    std::ifstream file;
    file.open(Utils::GetTowersDirectory() + '\\' + l_file);
    if(!file.is_open()){
        std::cerr << "Error when opening tower file: " << l_file << std::endl;
    }

    std::shared_ptr<TowerProporties> proporties;
    if(l_type == Tower::Basic || l_type == Tower::BasicPRO){
       proporties = std::make_shared<TowerProporties>();
    } else if(l_type == Tower::Missile || l_type == Tower::MissilePRO){
        proporties = std::make_shared<TowerMissileProporties>();
    }
    proporties->m_towerRotation = false;
    std::string line;
    while(std::getline(file, line)){
        if(line[0] == '|'){
            continue;
        }
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;

        if(type == "COST"){
            keystream >> proporties->m_cost;
        } else if(type == "TYPE"){
            int type;
            keystream >> type;
            proporties->m_tower = static_cast<Tower>(type);
        } else if(type == "ATTACK"){
            int bit;
            keystream >> bit;
            proporties->m_attacking.set(bit);
        } else if(type == "BULLET"){
            int bulletType;
            keystream >> bulletType;
            proporties->m_bulletType = static_cast<BulletType>(bulletType);
        } else if(type == "COLLISION"){
            if(!(static_cast<int>(proporties->m_tower) % 2)){
                keystream >> proporties->m_radiusCollision;
            } else{
                keystream >> proporties->m_size.x >> proporties->m_size.y;
            }
        } else if(type == "ROTATION"){
            proporties->m_towerRotation = true;
            keystream >> proporties->m_towerRotationOffset;
        } else if(type == "TEXTURE"){
            sf::IntRect rect;
            std::string texture;
            keystream >> texture >> rect.left >> rect.top >> rect.width >> rect.height;
            SetTextureForProporties(texture, rect, proporties);
        } else if(type == "UPGRADE"){
            UpgradeProporties upp;
            keystream >> upp.m_cost >> upp.m_damage >> upp.m_firingRate >> upp.m_radius >> upp.m_bulletSpeed;
            proporties->m_upgrades.emplace_back(upp);
        } else if(l_type == Tower::Missile || l_type == Tower::MissilePRO){
            if(type == "MISSILE"){
                auto missile = std::dynamic_pointer_cast<TowerMissileProporties>(proporties);
                sf::IntRect rect;
                keystream >> missile->m_missileTexture >> rect.left >> rect.top >> rect.width >> rect.height;
                m_context->m_textureManager->RequireResource(missile->m_missileTexture);
                missile->m_missileSprite.setTexture(*m_context->m_textureManager->GetResource(missile->m_missileTexture));
                missile->m_missileSprite.setTextureRect(rect);
                missile->m_missileSprite.setOrigin(sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
            }
        }
    }
    file.close();
    RegisterProporties(l_type, proporties);
}

std::shared_ptr<TowerProporties> TowerManager::GetProporties(const Tower &l_type)
{
    auto itr = m_towerProporties.find(l_type);
    return (itr == m_towerProporties.end() ? nullptr : itr->second);
}

void TowerManager::SetTextureForProporties(const std::string &l_texture, const sf::IntRect &l_rect, const std::shared_ptr<TowerProporties>& l_proporties)
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
    const sf::Vector2f& pos = m_interface->GetPosition();
    m_pressed->Upgrade();
    ShowUpgradeInterfaceFor(m_pressed);
    m_context->m_level->AddMoney(-m_pressed->GetUpgradeProporties().m_cost);
    m_interface->SetPosition(pos);
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

void TowerManager::DeleteTower(EventDetails *l_details)
{
    int money = m_pressed->GetProporties()->m_cost;
    for(unsigned int i = 0; i <= m_pressed->GetCurrentUpgrade(); ++i){
        money += m_pressed->GetProporties()->m_upgrades[i].m_cost;
    }
    m_context->m_level->AddMoney(money / 1.25);
    m_towers.erase(std::find(m_towers.begin(), m_towers.end(), m_pressed));
    m_interface->SetActive(false);
    m_pressed.reset();
}

void TowerManager::Pressed(const std::shared_ptr<TowerProporties> &l_proporties)
{
    m_placingTower = l_proporties;
}

void TowerManager::Draw()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    for(auto& itr : m_towers){
        itr->Draw(window);
    }
    if(m_pressed){
        sf::CircleShape range;
        range.setRadius(m_pressed->GetUpgradeProporties().m_radius);
        range.setPosition(m_pressed->GetPosition().x - range.getRadius(), m_pressed->GetPosition().y - range.getRadius());
        range.setFillColor(sf::Color(0, 0, 0, 128));
        window->draw(range);
    }
}

void TowerManager::DrawPlacingTower()
{
    sf::RenderWindow* window = m_context->m_wind->getRenderWindow();
    if(m_placingTower){
        const sf::Vector2i mousePos = m_context->m_eventManager->GetMousePos(m_context->m_wind->getRenderWindow());
        const float& radiusC = m_placingTower->m_radiusCollision;
        const sf::Vector2f currPlacingTowerPos = m_context->m_wind->getRenderWindow()->mapPixelToCoords(mousePos);
        sf::CircleShape range;

        /// Check collison with other towers and draw their collision circle
        bool collidingWithTowers = false;
        range.setFillColor(sf::Color(0, 0, 0, 128));
        for(auto& itr : m_towers){
            const float& radius = itr->GetProporties()->m_radiusCollision;
            range.setRadius(radius);
            range.setOrigin(radius, radius);
            range.setPosition(itr->GetPosition());
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
        range.setOrigin(AttackRadius, AttackRadius);
        range.setPosition(currPlacingTowerPos);
        window->draw(range);

        /// draw current placing tower collision circle
        range.setRadius(radiusC);
        range.setPosition(currPlacingTowerPos);
        range.setOrigin(radiusC, radiusC);
        if(m_context->m_level->IsOutsideMap(range) || m_context->m_level->CollideWithPath(range)){ m_colliding = true; }
        else if (!collidingWithTowers){ m_colliding = false; }

        if(m_colliding){
            range.setFillColor(sf::Color(255, 0, 0, 128));
        } else {
            range.setFillColor(sf::Color(0, 255, 0, 128));
        }
        window->draw(range);

        /// draw current placing tower sprite
        m_placingTower->m_sprite.setPosition(currPlacingTowerPos);
        m_placingTower->m_sprite.setRotation(0);
        window->draw(m_placingTower->m_sprite);
    }
}

void TowerManager::Update(const float& l_dT)
{
    for(auto& itr : m_towers){
        itr->Update(l_dT);
        itr->SetEnemy(m_context->m_enemyManager->GetEnemyFor(itr));
        itr->m_shootElapsed += l_dT;
        if(itr->m_lookinAt && itr->m_shootElapsed >= 1.f / itr->GetUpgradeProporties().m_firingRate){
            itr->m_shootElapsed = 0.f;
            itr->Shot(itr->m_lookinAt);
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
        m_placingTower = nullptr;
    } else {
        sf::Vector2i mousePos(m_context->m_wind->getRenderWindow()->mapPixelToCoords(l_details->m_mouse));
        for(auto& itr : m_towers){
            if(Utils::PointInsideCircle(mousePos, itr->GetPosition(), itr->GetProporties()->m_radiusCollision)){
                m_pressed = itr;
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

void TowerManager::ShowUpgradeInterfaceFor(const std::shared_ptr<TowerBase>& l_tower)
{
    m_interface->SetActive(true);
    const float& radius = l_tower->GetProporties()->m_radiusCollision;
    const sf::Vector2f pos(m_context->m_wind->getRenderWindow()->mapCoordsToPixel(
                               sf::Vector2f((l_tower->GetPosition().x + radius), (l_tower->GetPosition().y + radius))));
    const sf::Vector2u windowSize = m_context->m_wind->GetWindowSize();
    /// 170 is size of game interface
    const sf::Vector2f diff = sf::Vector2f(windowSize.x - pos.x - 170 - m_interface->GetSize().x, windowSize.y - pos.y - m_interface->GetSize().y);
    m_interface->SetPosition(sf::Vector2f(pos.x + (diff.x < 0 ? diff.x : 0), pos.y + (diff.y < 0 ? diff.y : 0)));

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

void TowerManager::AddTower(const std::shared_ptr<TowerProporties>& l_proporties, const sf::Vector2f& l_pos)
{
    auto itr = m_towerFactory.find(l_proporties->m_tower);
    if(itr == m_towerFactory.end()){
        return;
    }
    m_towers.emplace_back(itr->second(l_proporties));
    m_towers.back()->SetPosition(l_pos);
    m_statistics->AddTowersPlaced();
}

void TowerManager::Restart()
{
    m_towers.clear();
    m_pressed.reset();
    m_placingTower = nullptr;
    m_interface->SetActive(false);
}
