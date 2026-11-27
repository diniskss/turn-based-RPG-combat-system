#include "consumable.hpp"
#include <iostream>

#include "fighter.hpp"

namespace combat
{

Consumable::Consumable()
    // Конструктор базового класса вызывается в списке инициализации.
    : BaseItem("Small Health Potion", std::make_shared<Effect>("None", 0))
    , m_healAmount(20)
{
    std::cout << "[Consumable] Consumable(): default '" << GetName() << "'\n";
}

Consumable::Consumable(std::string_view name, std::int32_t healAmount, std::shared_ptr<Effect> effect)
    // Сначала — конструктор базового класса, затем — поле наследника.
    : BaseItem(name, std::move(effect))
    , m_healAmount(healAmount < 0 ? 0 : healAmount)
{
    // Правило: зелье восстанавливает здоровье, а не отнимает.
    if (healAmount < 0)
    {
        std::cout << "[Consumable Error] negative heal amount replaced with 0\n";
    }
    std::cout << "[Consumable] Consumable('" << GetName() << "'): restores +" << m_healAmount << "\n";
}

Consumable::Consumable(SharedEffectTag, const Consumable& other)
    : BaseItem(SharedEffectTag{}, other)
    , m_healAmount(other.m_healAmount)
{
    std::cout << "[Consumable] Consumable(shallow copy) '" << GetName() << "'\n";
}

Consumable::~Consumable()
{
    std::cout << "[Consumable] ~Consumable() '" << GetName() << "'\n";
}

void Consumable::Apply(Fighter& target) const
{
    std::cout << "[Consumable] '" << GetName() << "' is applied to fighter " << target.GetId() << "\n";
    target.RestoreHealth(m_healAmount);
}

std::string Consumable::Describe() const
{
    // Дополняет описание базового класса вызовом BaseItem::Describe().
    return BaseItem::Describe() + ", consumable: restores +" + std::to_string(m_healAmount);
}

std::unique_ptr<BaseItem> Consumable::Clone() const
{
    return std::make_unique<Consumable>(*this);
}

std::int32_t Consumable::GetHealAmount() const
{
    return m_healAmount;
}

} // namespace combat
