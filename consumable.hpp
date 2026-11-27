#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "base_item.hpp"

namespace combat
{

// Расходник: предмет одноразового применения, восстанавливает здоровье бойца.
// final: от расходника не наследуются, дальнейшая специализация не предусмотрена.
class Consumable final : public BaseItem
{
private:
    std::int32_t m_healAmount;

public:
    // По умолчанию: малое зелье здоровья.
    Consumable();

    Consumable(std::string_view name, std::int32_t healAmount, std::shared_ptr<Effect> effect);

    // Правило пяти: все пять операций указаны явно.
    Consumable(const Consumable& other) = default;          // глубокая копия (через BaseItem)
    Consumable(SharedEffectTag, const Consumable& other);   // поверхностная копия: общий эффект
    Consumable(Consumable&& other) noexcept = default;
    Consumable& operator=(const Consumable& other) = default;
    Consumable& operator=(Consumable&& other) noexcept = default;
    ~Consumable() override;

    // Чисто виртуальный метод базы: своя логика применения.
    void Apply(Fighter& target) const override;

    // Дополняет описание базового класса.
    std::string Describe() const override;

    [[nodiscard]] std::unique_ptr<BaseItem> Clone() const override;

    [[nodiscard]] std::int32_t GetHealAmount() const;
};

} // namespace combat
