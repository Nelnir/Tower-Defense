#ifndef STATISTICS_H
#define STATISTICS_H


class Statistics
{
public:
    Statistics() : m_totalMoneyEarned(0), m_totalMonstersKilled(0), m_totalTowersPlaced(0), m_timePlayed(0) {}

    void AddMoneyEarned(const int& l_money) { m_totalMoneyEarned += l_money; }
    void AddMonstersKilled(const int& l_monsters = 1) { m_totalMonstersKilled += l_monsters; }
    void AddTowersPlaced(const int& l_towers = 1) { m_totalTowersPlaced += l_towers; }
    void AddTimePlayed(const float& l_time) { m_timePlayed += l_time; }
    void Reset(){
        m_totalMoneyEarned = m_totalMonstersKilled = m_totalTowersPlaced = 0;
        m_timePlayed = 0;
    }
    const int& GetMoneyEarned() { return m_totalMoneyEarned; }
    const int& GetMonstersKilled() { return m_totalMonstersKilled; }
    const int& GetTowersPlaced() { return m_totalTowersPlaced; }
    const float& GetTimePlayed() { return m_timePlayed; }
private:
    int m_totalMoneyEarned;
    int m_totalMonstersKilled;
    int m_totalTowersPlaced;
    float m_timePlayed; /// in seconds
};

#endif // STATISTICS_H
