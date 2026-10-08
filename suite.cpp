#include "suite.hpp"
#include <iostream>

namespace combat
{

Suite::Suite(std::string_view name)
    : m_name(name)
{
    std::cout << "[Suite] Created suite: " << m_name << "\n";
}

Suite::~Suite()
{
    // Агрегация: при уничтожении свиты сами бойцы НЕ удаляются через delete, так как они существуют независимо от свиты.
    std::cout << "[Suite] Destroyed suite: " << m_name << " (fighters remain intact)\n";
}

std::string_view Suite::GetName() const
{
    return m_name;
}

void Suite::AddFighter(Fighter* fighter)
{
    if (fighter != nullptr)
    {
        m_fighters.push_back(fighter);
        std::cout << "[Suite] Fighter added to suite " << m_name << "\n";
    }
}

void Suite::InspectSuite() const
{
    std::cout << "[Suite] Inspecting suite '" << m_name << "'. Total fighters: " << m_fighters.size() << "\n";
    for (const auto* fighter : m_fighters)
    {
        if (fighter != nullptr)
        {
            std::cout << "  - Fighter ID: " << fighter->GetId() 
                      << ", HP: " << fighter->GetHealth() 
                      << ", Mana: " << fighter->GetMana() << "\n";
        }
    }
}

} // namespace combat