#include "inventory.hpp"
#include <algorithm>
#include <iostream>

namespace combat
{

Inventory::Inventory(std::string_view name)
    : m_name(name)
{
    std::cout << "[Inventory] Created inventory '" << m_name << "' (max " << MAX_CAPACITY << ")\n";
}

bool Inventory::Add(std::unique_ptr<BaseItem> item)
{
    // Правило: в инвентаре не больше MAX_CAPACITY предметов.
    // Предмет передан по значению: при отказе он уничтожается вместе с параметром.
    if (m_items.size() >= MAX_CAPACITY)
    {
        std::cout << "[Inventory Error] '" << m_name << "' is full (max " << MAX_CAPACITY
                  << "), item rejected\n";
        return false;
    }
    if (!item)
    {
        std::cout << "[Inventory Error] cannot add an empty item to '" << m_name << "'\n";
        return false;
    }

    std::cout << "[Inventory] '" << item->GetName() << "' added to '" << m_name << "'\n";
    m_items.push_back(std::move(item));
    return true;
}

bool Inventory::IsFull() const
{
    return m_items.size() >= MAX_CAPACITY;
}

bool Inventory::AddClone(const BaseItem& item)
{
    // Правило: в инвентаре не больше MAX_CAPACITY предметов.
    if (m_items.size() >= MAX_CAPACITY)
    {
        std::cout << "[Inventory Error] '" << m_name << "' is full (max " << MAX_CAPACITY
                  << "), clone of '" << item.GetName() << "' rejected\n";
        return false;
    }

    // Виртуальный Clone(): копию нужного типа делает сам предмет.
    m_items.push_back(item.Clone());
    std::cout << "[Inventory] clone of '" << item.GetName() << "' added to '" << m_name << "'\n";
    return true;
}

std::size_t Inventory::GetSize() const
{
    return m_items.size();
}

void Inventory::PrintContents() const
{
    std::cout << "[Inventory] '" << m_name << "' contains " << m_items.size()
              << " of " << MAX_CAPACITY << ":\n";
    for (const auto& item : m_items)
    {
        // Указатель на базовый класс: виртуальный Describe() вызывается у наследника.
        std::cout << "  - " << item->Describe() << "\n";
    }
}

std::vector<std::string> Inventory::FindByName(std::string_view query) const
{
    std::vector<std::string> found;
    for (const auto& item : m_items)
    {
        // find: позиция подстроки в названии или npos, если её нет.
        if (item->GetName().find(query) != std::string_view::npos)
        {
            found.push_back(item->Describe());
        }
    }
    return found;
}

void Inventory::SortByName()
{
    // Сравнение строк: названия сортируются лексикографически.
    std::sort(m_items.begin(), m_items.end(),
              [](const std::unique_ptr<BaseItem>& left, const std::unique_ptr<BaseItem>& right)
              {
                  return left->GetName() < right->GetName();
              });
    std::cout << "[Inventory] '" << m_name << "' sorted by name\n";
}

} // namespace combat
