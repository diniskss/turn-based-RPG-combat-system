#include "spell.hpp"
#include <iostream>

namespace combat
{

Spell::Spell(std::string_view name, std::int32_t manaCost)
    : m_name(name)
    , m_manaCost(manaCost)
{
    std::cout << "[Spell] Created spell: " << m_name << "\n";
}

Spell::~Spell()
{
    std::cout << "[Spell] Destroyed spell: " << m_name << "\n";
}

std::string_view Spell::GetName() const
{
    return m_name;
}

std::int32_t Spell::GetManaCost() const
{
    return m_manaCost;
}

void Spell::Use() const
{
    std::cout << "[Spell] Using spell '" << m_name << "' (Cost: " << m_manaCost << ")\n";
}

} // namespace combat