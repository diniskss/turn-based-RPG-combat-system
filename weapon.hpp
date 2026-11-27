#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "base_item.hpp"

namespace combat
{

// Оружие: повышает атаку бойца.
class Weapon : public BaseItem
{
private:
    std::int32_t m_attackBonus;

public:
    // По умолчанию: тренировочное оружие новичка.
    Weapon();

    Weapon(std::string_view name, std::int32_t attackBonus, std::shared_ptr<Effect> effect);

    // Правило пяти: все пять операций указаны явно.
    Weapon(const Weapon& other) = default;            // глубокая копия (через BaseItem)
    Weapon(SharedEffectTag, const Weapon& other);     // поверхностная копия: общий эффект
    Weapon(Weapon&& other) noexcept = default;
    Weapon& operator=(const Weapon& other) = default;
    Weapon& operator=(Weapon&& other) noexcept = default;
    ~Weapon() override;

    // Чисто виртуальный метод базы: своя логика применения.
    void Apply(Fighter& target) const override;

    // Дополняет описание базового класса.
    std::string Describe() const override;

    [[nodiscard]] std::unique_ptr<BaseItem> Clone() const override;

    [[nodiscard]] std::int32_t GetAttackBonus() const;
};

} // namespace combat
