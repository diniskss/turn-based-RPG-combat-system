#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "effect.hpp"

namespace combat
{

class Fighter;
class Inventory;

// Тег для конструктора: создать поверхностную копию предмета,
// у которой вложенный эффект остаётся общим с оригиналом.
struct SharedEffectTag
{
};

// Абстрактный базовый класс иерархии предметов.
// Создать объект BaseItem нельзя: в нём есть чисто виртуальные методы.
// Существуют только наследники: Weapon, Armor, Consumable.
class BaseItem
{
private:
    // Статическое поле: одно на весь класс, а не по одному у каждого предмета.
    inline static std::int32_t m_createdCount{ 0 };

    std::int32_t m_itemNumber;
    std::string m_name;

    // Вложенный объект: эффект, который накладывает предмет.
    std::shared_ptr<Effect> m_effect;

public:
    BaseItem() = delete;

    // Виртуальный деструктор: наследник может уничтожаться через указатель на базовый класс.
    virtual ~BaseItem();

    // Чисто виртуальный: у каждого наследника своя логика применения.
    virtual void Apply(Fighter& target) const = 0;

    // В базе есть реализация: наследник может дополнить её или заменить полностью.
    virtual std::string Describe() const;

    // Чисто виртуальный: копию предмета делает сам объект, знающий свой тип.
    [[nodiscard]] virtual std::unique_ptr<BaseItem> Clone() const = 0;

    [[nodiscard]] std::int32_t GetItemNumber() const;
    [[nodiscard]] std::string_view GetName() const;

    // Сколько предметов владеют эффектом: 1 — эффект собственный, больше 1 — общий.
    [[nodiscard]] std::int32_t GetEffectOwnerCount() const;

    [[nodiscard]] const Effect& GetEffect() const;
    [[nodiscard]] Effect& GetEffect();

    // this: предмет передаёт себя (*this) инвентарю, а инвентарь кладёт его копию.
    // Возвращает false, если инвентарь полон и копия не поместилась.
    [[nodiscard]] bool StoreCopyIn(Inventory& inventory) const;

    // Статический метод: вызывается без объекта и работает только со статическим полем.
    [[nodiscard]] static std::int32_t GetCreatedCount();

protected:
    // Создать предмет может только наследник.
    BaseItem(std::string_view name, std::shared_ptr<Effect> effect);

    // Глубокая копия: своё имя и свой экземпляр эффекта.
    // Protected: наследники и Clone() ими пользуются, а снаружи через ссылку
    // на базу нельзя «срезать» предмет до базовой части.
    BaseItem(const BaseItem& other);
    BaseItem& operator=(const BaseItem& other);

    // Поверхностная копия: эффект остаётся общим с оригиналом.
    BaseItem(SharedEffectTag, const BaseItem& other);

    BaseItem(BaseItem&& other) noexcept;
    BaseItem& operator=(BaseItem&& other) noexcept;

    // Общая часть описания: нужна наследникам для Describe(), но не остальному коду.
    [[nodiscard]] std::string Header() const;
};

} // namespace combat
