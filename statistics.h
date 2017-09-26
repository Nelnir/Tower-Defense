#ifndef STATISTICS_H
#define STATISTICS_H

#include <unordered_map>
#include "abstractenemy.h"
#include "gui_interface.h"

class Statistics
{
public:
    Statistics() : m_totalMoneyEarned(0), m_totalMonstersKilled(0), m_totalTowersPlaced(0), m_timePlayed(0), m_totalDmgDealt(0){
        m_enemiesTypeKilled.emplace(Enemy::Plane, 0);
        m_enemiesTypeKilled.emplace(Enemy::Soldier, 0);
        m_enemiesTypeKilled.emplace(Enemy::Tank, 0);
    }

    void AddMoneyEarned(const unsigned int& l_money) { m_totalMoneyEarned += l_money; }
    void AddEnemiesKilled(const unsigned int& l_monsters = 1) { m_totalMonstersKilled += l_monsters; }
    void AddTowersPlaced(const unsigned int& l_towers = 1) { m_totalTowersPlaced += l_towers; }
    void AddDmgDealt(const unsigned int& l_dmg) { m_totalDmgDealt += l_dmg; }
    void AddTimePlayed(const float& l_time) { m_timePlayed += l_time; }
    void AddEnemiesWithTypeKilled(const Enemy& l_type, const unsigned int& l_enemies = 1){
        auto itr = m_enemiesTypeKilled.emplace(l_type, 0).first;
        itr->second += l_enemies;
    }
    void Reset(){
        m_totalMoneyEarned = m_totalMonstersKilled = m_totalTowersPlaced = 0;
        m_timePlayed = 0;
    }
    const unsigned int& GetMoneyEarned() { return m_totalMoneyEarned; }
    const unsigned int& GetMonstersKilled() { return m_totalMonstersKilled; }
    const unsigned int& GetTowersPlaced() { return m_totalTowersPlaced; }
    const unsigned int& GetTotalDmg() { return m_totalDmgDealt; }
    const float& GetTimePlayed() { return m_timePlayed; }
    const unsigned int& GetEnemiesWithTypeKilled(const Enemy& l_type) { return m_enemiesTypeKilled.find(l_type)->second; }
    void SetupInterface(GUI_Interface* l_interface){
        l_interface->GetElement("MoneyEarned")->SetText("Money earned: " + std::to_string(m_totalMoneyEarned));
        l_interface->GetElement("TowersPlaced")->SetText("Towers placed: " + std::to_string(m_totalTowersPlaced));
        l_interface->GetElement("DamageDealt")->SetText("Damage dealt: " + std::to_string(m_totalDmgDealt));
        l_interface->GetElement("TimePlayed")->SetText("Time played: " + std::to_string(m_timePlayed));
        l_interface->GetElement("MonstersKilled")->SetText("Monsters killed: " + std::to_string(m_totalMonstersKilled));
        l_interface->GetElement("SoldiersKilled")->SetText("Soldiers killed: " + std::to_string(m_enemiesTypeKilled.find(Enemy::Soldier)->second));
        l_interface->GetElement("TanksKilled")->SetText("Tanks killed: " + std::to_string(m_enemiesTypeKilled.find(Enemy::Tank)->second));
        l_interface->GetElement("PlanesKilled")->SetText("Planes killed: " + std::to_string(m_enemiesTypeKilled.find(Enemy::Plane)->second));
    }

private:
    unsigned int m_totalMoneyEarned;
    unsigned int m_totalMonstersKilled;
    unsigned int m_totalTowersPlaced;
    unsigned int m_totalDmgDealt;
    float m_timePlayed; /// in seconds
    std::unordered_map<Enemy, unsigned int> m_enemiesTypeKilled;
};

#endif // STATISTICS_H
