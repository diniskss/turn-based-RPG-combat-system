#include "fighter.hpp"
#include <iostream>

namespace combat
{

Fighter::Fighter(std::int32_t id, std::int32_t health, std::int32_t mana, std::int32_t actionPoints,
                 std::string_view spellName, std::int32_t spellCost)
    : m_id(id)
    , m_health(health)
    , m_maxHealth(health)
    , m_mana(mana)
    , m_maxMana(mana)
    , m_actionPoints(actionPoints)
    , m_spell(spellName, spellCost)   // Spell создаётся здесь, ДО тела конструктора
{
    std::cout << "[Fighter] Created fighter ID: " << m_id << "\n";
}

Fighter::~Fighter()
{
    // m_spell уничтожится автоматически ПОСЛЕ выполнения этого тела
    std::cout << "[Fighter] Destroyed fighter ID: " << m_id << "\n";
}

std::int32_t Fighter::GetId() const { return m_id; }
std::int32_t Fighter::GetHealth() const { return m_health; }
std::int32_t Fighter::GetMana() const { return m_mana; }
std::int32_t Fighter::GetActionPoints() const { return m_actionPoints; }
const Spell& Fighter::GetSpell() const { return m_spell; }

void Fighter::TakeDamage(std::int32_t amount)
{
    const auto oldHealth = m_health;
    m_health -= amount;

    // Правило: здоровье не может быть меньше 0
    if (m_health < 0)
    {
        m_health = 0;
    }
    std::cout << "[Fighter] ID: " << m_id << " took " << amount
              << " damage. HP: " << oldHealth << " -> " << m_health << "\n";
}

bool Fighter::CastSpell()
{
    const auto cost = m_spell.GetManaCost();

    // Правило: стоимость заклинания не может превышать текущий запас маны
    if (m_mana < cost)
    {
        std::cout << "[Fighter Error] ID: " << m_id
                  << " cannot cast spell '" << m_spell.GetName() << "': required mana " << cost
                  << ", available mana " << m_mana << " (Action rejected)\n";
        return false;
    }

    m_mana -= cost;
    m_spell.Use();
    std::cout << "[Fighter] ID: " << m_id << " successfully cast spell. Remaining mana: " << m_mana << "\n";
    return true;
}

} // namespace combat