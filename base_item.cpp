#include "base_item.hpp"
#include <iostream>

#include "fighter.hpp"
#include "inventory.hpp"

namespace combat
{

BaseItem::BaseItem(std::string_view name, std::shared_ptr<Effect> effect)
    : m_itemNumber(++m_createdCount)
    , m_name(name)
    // У предмета всегда есть эффект: если эффект не задан, подставляется нейтральный «None».
    , m_effect(effect ? std::move(effect) : std::make_shared<Effect>("None", 0))
{
    std::cout << "[BaseItem] BaseItem(): #" << m_itemNumber << " '" << m_name << "'\n";
}

BaseItem::BaseItem(const BaseItem& other)
    // Копия — новый предмет в игре, она получает свой номер.
    : m_itemNumber(++m_createdCount)
    , m_name(other.m_name)
    // Глубокая копия: свой экземпляр эффекта, а не общий указатель на чужой.
    , m_effect(other.m_effect ? std::make_shared<Effect>(*other.m_effect)
                              : std::make_shared<Effect>("None", 0))
{
    std::cout << "[BaseItem] BaseItem(copy): #" << other.m_itemNumber
              << " -> #" << m_itemNumber << " '" << m_name << "'\n";
}

BaseItem::BaseItem(SharedEffectTag, const BaseItem& other)
    : m_itemNumber(++m_createdCount)
    , m_name(other.m_name)
    // Поверхностная копия: копируется только указатель, эффект остаётся общим.
    , m_effect(other.m_effect)
{
    std::cout << "[BaseItem] BaseItem(shallow copy): #" << other.m_itemNumber
              << " -> #" << m_itemNumber << " '" << m_name << "'\n";
}

BaseItem::BaseItem(BaseItem&& other) noexcept
    // Перемещение не создаёт новый предмет: номер переходит вместе с содержимым.
    : m_itemNumber(other.m_itemNumber)
    , m_name(std::move(other.m_name))
    , m_effect(std::move(other.m_effect))
{
    other.m_itemNumber = 0;
    std::cout << "[BaseItem] BaseItem(move): #" << m_itemNumber << " '" << m_name << "'\n";
}

BaseItem& BaseItem::operator=(const BaseItem& other)
{
    // this: проверка самоприсваивания — item = item ничего не копирует.
    if (this == &other)
    {
        std::cout << "[BaseItem] operator=(copy): self-assignment detected, nothing to copy\n";
        return *this;
    }

    std::cout << "[BaseItem] operator=(copy): #" << other.m_itemNumber
              << " -> #" << m_itemNumber << "\n";
    m_name = other.m_name;
    // Глубокое присваивание: свой экземпляр эффекта, старый освобождается присваиванием.
    m_effect = other.m_effect ? std::make_shared<Effect>(*other.m_effect)
                              : std::make_shared<Effect>("None", 0);

    // this: возврат самого объекта позволяет писать цепочку a = b = c.
    return *this;
}

BaseItem& BaseItem::operator=(BaseItem&& other) noexcept
{
    std::cout << "[BaseItem] operator=(move): #" << other.m_itemNumber
              << " -> #" << m_itemNumber << "\n";

    if (this != &other)
    {
        m_itemNumber = other.m_itemNumber;
        m_name = std::move(other.m_name);
        m_effect = std::move(other.m_effect);
        other.m_itemNumber = 0;
    }

    return *this;
}

BaseItem::~BaseItem()
{
    std::cout << "[BaseItem] ~BaseItem(): #" << m_itemNumber << " '" << m_name << "'\n";
}

std::string BaseItem::Describe() const
{
    return Header() + ", effect: " + (m_effect ? m_effect->Describe() : "none");
}

std::int32_t BaseItem::GetItemNumber() const
{
    return m_itemNumber;
}

std::string_view BaseItem::GetName() const
{
    return m_name;
}

std::int32_t BaseItem::GetEffectOwnerCount() const
{
    return static_cast<std::int32_t>(m_effect.use_count());
}

const Effect& BaseItem::GetEffect() const
{
    return *m_effect;
}

Effect& BaseItem::GetEffect()
{
    return *m_effect;
}

bool BaseItem::StoreCopyIn(Inventory& inventory) const
{
    return inventory.AddClone(*this);
}

std::int32_t BaseItem::GetCreatedCount()
{
    return m_createdCount;
}

std::string BaseItem::Header() const
{
    return "#" + std::to_string(m_itemNumber) + " '" + m_name + "'";
}

} // namespace combat
