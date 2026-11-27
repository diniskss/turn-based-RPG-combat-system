#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "base_item.hpp"

namespace combat
{

// Броня: повышает защиту бойца.
class Armor : public BaseItem
{
private:
    std::int32_t m_defenseBonus;

public:
    // По умолчанию: тканевая рубаха новичка.
    Armor();

    Armor(std::string_view name, std::int32_t defenseBonus, std::shared_ptr<Effect> effect);

    // Правило пяти: все пять операций указаны явно.
    Armor(const Armor& other) = default;              // глубокая копия (через BaseItem)
    Armor(SharedEffectTag, const Armor& other);       // поверхностная копия: общий эффект
    Armor(Armor&& other) noexcept = default;
    Armor& operator=(const Armor& other) = default;
    Armor& operator=(Armor&& other) noexcept = default;
    ~Armor() override;

    // Чисто виртуальный метод базы: своя логика применения.
    void Apply(Fighter& target) const override;

    // Полностью заменяет описание базового класса.
    std::string Describe() const override;

    [[nodiscard]] std::unique_ptr<BaseItem> Clone() const override;

    [[nodiscard]] std::int32_t GetDefenseBonus() const;
};

} // namespace combat
