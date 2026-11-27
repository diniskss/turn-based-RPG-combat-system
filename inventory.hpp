#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "base_item.hpp"

namespace combat
{

// Инвентарь: общее хранилище предметов свиты.
// Владеет предметами (std::unique_ptr<BaseItem>) и работает с ними
// только через указатель на базовый класс.
class Inventory
{
public:
    // Правило предметной области: в инвентаре не больше MAX_CAPACITY предметов.
    static constexpr std::size_t MAX_CAPACITY = 8;

private:
    std::string m_name;
    std::vector<std::unique_ptr<BaseItem>> m_items;

public:
    explicit Inventory(std::string_view name);

    // Правило пяти: все пять операций указаны явно.
    // Копировать инвентарь нельзя: каждый предмет принадлежит ровно одному инвентарю.
    Inventory(const Inventory&) = delete;
    Inventory(Inventory&&) noexcept = default;
    Inventory& operator=(const Inventory&) = delete;
    Inventory& operator=(Inventory&&) noexcept = default;
    ~Inventory() = default;

    // Принимает предмет во владение: параметр передаётся по значению,
    // поэтому при отказе (инвентарь полон) предмет уничтожается вместе с параметром.
    // Чтобы не потерять предмет, сначала проверяется IsFull().
    [[nodiscard]] bool Add(std::unique_ptr<BaseItem> item);

    // Проверка правила: инвентарь заполнен, добавить предмет нельзя.
    [[nodiscard]] bool IsFull() const;

    // Кладёт глубокую копию предмета: копию делает сам предмет (Clone).
    [[nodiscard]] bool AddClone(const BaseItem& item);

    [[nodiscard]] std::size_t GetSize() const;

    // Полиморфизм: Describe() выбирается по фактическому типу предмета.
    void PrintContents() const;

    // Описания предметов, в названии которых встречается подстрока query.
    [[nodiscard]] std::vector<std::string> FindByName(std::string_view query) const;

    // Сортирует предметы по названию.
    void SortByName();
};

} // namespace combat
