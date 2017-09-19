#ifndef LEVEL_H
#define LEVEL_H

class Level
{
    int m_currentLevel;
    const int m_levels = 4;
public:
    Level();
    int GetCurrentLevel() { return m_currentLevel; }
    int GetNumberOfLevels() { return m_levels; }
    void SetCurrentLevel(const int& l_level) { m_currentLevel = l_level; }
};

#endif // LEVEL_H
