#include "fighter.hpp"
#include <iostream>
#include <algorithm>

namespace combat
{

Fighter::Fighter(std::int32_t id, std::int32_t health, std::int32_t mana, std::int32_t actionPoints)
    : m_id(id)
    , m_health(health)
    , m_maxHealth(health)
    , m_mana(mana)
    , m_maxMana(mana)
    , m_actionPoints(actionPoints)
{
    std::cout << "[Fighter] Created fighter ID: " << m_id << "\n";
}

Fighter::~Fighter()
{
    std::cout << "[Fighter] Destroyed fighter ID: " << m_id << "\n";
}

std::int32_t Fighter::GetId() const
{
    return m_id;
}

std::int32_t Fighter::GetHealth() const
{
    return m_health;
}

std::int32_t Fighter::GetMana() const
{
    return m_mana;
}

std::int32_t Fighter::GetActionPoints() const
{
    return m_actionPoints;
}

void Fighter::TakeDamage(std::int32_t amount)
{
    const auto oldHealth = m_health;
    m_health -= amount;

    // Проверка правила: здоровье не может быть меньше 0
    if (m_health < 0)
    {
        m_health = 0;
    }
    std::cout << "[Fighter] ID: " << m_id << " took " << amount 
              << " damage. HP: " << oldHealth << " -> " << m_health << "\n";
}

bool Fighter::CastSpell(std::int32_t manaCost)
{
    // Проверка правила: стоимость заклинания не может превышать текущий запас маны
    if (m_mana < manaCost)
    {
        std::cout << "[Fighter Error] ID: " << m_id 
                  << " cannot cast spell: required mana " << manaCost 
                  << ", available mana " << m_mana << " (Action rejected)\n";
        return false;
    }

    m_mana -= manaCost;
    std::cout << "[Fighter] ID: " << m_id << " successfully cast spell. Remaining mana: " << m_mana << "\n";
    return true;
}

} // namespace combat