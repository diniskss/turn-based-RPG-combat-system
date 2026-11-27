#include "armor.hpp"
#include <iostream>

#include "fighter.hpp"

namespace combat
{

Armor::Armor()
    // Конструктор базового класса вызывается в списке инициализации.
    : BaseItem("Cloth Shirt", std::make_shared<Effect>("None", 0))
    , m_defenseBonus(1)
{
    std::cout << "[Armor] Armor(): default '" << GetName() << "'\n";
}

Armor::Armor(std::string_view name, std::int32_t defenseBonus, std::shared_ptr<Effect> effect)
    // Сначала — конструктор базового класса, затем — поле наследника.
    : BaseItem(name, std::move(effect))
    , m_defenseBonus(defenseBonus < 0 ? 0 : defenseBonus)
{
    // Правило: броня повышает защиту, а не снижает.
    if (defenseBonus < 0)
    {
        std::cout << "[Armor Error] negative defense bonus replaced with 0\n";
    }
    std::cout << "[Armor] Armor('" << GetName() << "'): defense +" << m_defenseBonus << "\n";
}

Armor::Armor(SharedEffectTag, const Armor& other)
    : BaseItem(SharedEffectTag{}, other)
    , m_defenseBonus(other.m_defenseBonus)
{
    std::cout << "[Armor] Armor(shallow copy) '" << GetName() << "'\n";
}

Armor::~Armor()
{
    std::cout << "[Armor] ~Armor() '" << GetName() << "'\n";
}

void Armor::Apply(Fighter& target) const
{
    std::cout << "[Armor] '" << GetName() << "' is applied to fighter " << target.GetId() << "\n";
    target.IncreaseDefense(m_defenseBonus);
}

std::string Armor::Describe() const
{
    // Полностью заменяет описание базового класса — без вызова BaseItem::Describe().
    // Общую часть наследник берёт из protected-метода Header().
    return "Armor " + Header() + ": defense +" + std::to_string(m_defenseBonus)
           + ", effect '" + std::string(GetEffect().GetName()) + "'";
}

std::unique_ptr<BaseItem> Armor::Clone() const
{
    return std::make_unique<Armor>(*this);
}

std::int32_t Armor::GetDefenseBonus() const
{
    return m_defenseBonus;
}

} // namespace combat
