#include "weapon.hpp"
#include <iostream>

#include "fighter.hpp"

namespace combat
{

Weapon::Weapon()
    // Конструктор по умолчанию: тренировочное оружие новичка.
    // Конструктор базового класса вызывается в списке инициализации.
    : BaseItem("Wooden Sword", std::make_shared<Effect>("None", 0))
    , m_attackBonus(1)
{
    std::cout << "[Weapon] Weapon(): default '" << GetName() << "'\n";
}

Weapon::Weapon(std::string_view name, std::int32_t attackBonus, std::shared_ptr<Effect> effect)
    // Сначала создаётся базовая часть предмета: конструктор BaseItem в списке инициализации.
    : BaseItem(name, std::move(effect))
    // Затем — поле наследника.
    , m_attackBonus(attackBonus < 0 ? 0 : attackBonus)
{
    // Правило: оружие повышает атаку, а не снижает.
    if (attackBonus < 0)
    {
        std::cout << "[Weapon Error] negative attack bonus replaced with 0\n";
    }
    std::cout << "[Weapon] Weapon('" << GetName() << "'): attack +" << m_attackBonus << "\n";
}

Weapon::Weapon(SharedEffectTag, const Weapon& other)
    : BaseItem(SharedEffectTag{}, other)
    , m_attackBonus(other.m_attackBonus)
{
    std::cout << "[Weapon] Weapon(shallow copy) '" << GetName() << "'\n";
}

Weapon::~Weapon()
{
    std::cout << "[Weapon] ~Weapon() '" << GetName() << "'\n";
}

void Weapon::Apply(Fighter& target) const
{
    std::cout << "[Weapon] '" << GetName() << "' is applied to fighter " << target.GetId() << "\n";
    target.IncreaseAttack(m_attackBonus);
}

std::string Weapon::Describe() const
{
    // Явный вызов версии базового класса: без BaseItem:: метод вызвал бы сам себя.
    return BaseItem::Describe() + ", weapon: attack +" + std::to_string(m_attackBonus);
}

std::unique_ptr<BaseItem> Weapon::Clone() const
{
    // Копию нужного типа делает сам предмет через конструктор копирования.
    return std::make_unique<Weapon>(*this);
}

std::int32_t Weapon::GetAttackBonus() const
{
    return m_attackBonus;
}

} // namespace combat
