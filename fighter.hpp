#pragma once

#include <cstdint>
#include <string_view>
#include "spell.hpp"

namespace combat
{

class Fighter
{
private:
    static constexpr auto TAG = "fighter";

    std::int32_t m_id;
    std::int32_t m_health;
    std::int32_t m_maxHealth;
    std::int32_t m_mana;
    std::int32_t m_maxMana;
    std::int32_t m_actionPoints;
    std::int32_t m_attack;
    std::int32_t m_defense;

    // Композиция: заклинание принадлежит бойцу и живёт ровно столько же
    Spell m_spell;

public:
    Fighter(std::int32_t id, std::int32_t health, std::int32_t mana, std::int32_t actionPoints,
            std::string_view spellName, std::int32_t spellCost,
            std::int32_t attack, std::int32_t defense);

    Fighter() = delete;
    Fighter(const Fighter&) = delete;
    Fighter(Fighter&&) = delete;
    Fighter& operator=(const Fighter&) = delete;
    Fighter& operator=(Fighter&&) = delete;

    ~Fighter();

    [[nodiscard]] std::int32_t GetId() const;
    [[nodiscard]] std::int32_t GetHealth() const;
    [[nodiscard]] std::int32_t GetMana() const;
    [[nodiscard]] std::int32_t GetActionPoints() const;
    [[nodiscard]] std::int32_t GetAttack() const;
    [[nodiscard]] std::int32_t GetDefense() const;
    [[nodiscard]] const Spell& GetSpell() const;

    // Содержательный метод 1: получение урона с проверкой (здоровье >= 0)
    void TakeDamage(std::int32_t amount);

    // Содержательный метод 2: применение своего заклинания с проверкой (хватает ли маны)
    [[nodiscard]] bool CastSpell();

    // Содержательный метод 3: восстановление ресурсов предметами.
    // Правила: здоровье и мана не бывают отрицательными и не превышают максимумы.
    void RestoreHealth(std::int32_t amount);
    void RestoreMana(std::int32_t amount);

    // Содержательный метод 4: применение к бойцу оружия и брони.
    // Правило: предмет повышает параметр бойца, а не снижает.
    void IncreaseAttack(std::int32_t bonus);
    void IncreaseDefense(std::int32_t bonus);
};

} // namespace combat