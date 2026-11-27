#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "armor.hpp"
#include "base_item.hpp"
#include "consumable.hpp"
#include "effect.hpp"
#include "fighter.hpp"
#include "inventory.hpp"
#include "suite.hpp"
#include "weapon.hpp"

namespace
{

void PrintHeader(std::string_view title)
{
    std::cout << "\n=== " << title << " ===\n";
}

// Обработка строк по теме проекта: разбирает код предмета вида
// "type:name:power:effect:duration" (например, "weapon:Fire Sword:15:Bleeding:3")
// и создаёт предмет нужного наследника. Возвращает nullptr, если код неверен.
std::unique_ptr<combat::BaseItem> MakeItemFromCode(const std::string& code)
{
    // find: позиции разделителей; std::string::npos, если разделителя нет.
    const std::size_t first = code.find(':');
    const std::size_t second = code.find(':', first + 1);
    const std::size_t third = code.find(':', second + 1);
    const std::size_t fourth = code.find(':', third + 1);
    if (first == std::string::npos || second == std::string::npos
        || third == std::string::npos || fourth == std::string::npos)
    {
        std::cout << "[Item Code Error] expected type:name:power:effect:duration, got '" << code << "'\n";
        return nullptr;
    }

    // substr: части строки между разделителями.
    const std::string type = code.substr(0, first);
    const std::string name = code.substr(first + 1, second - first - 1);
    const std::string powerText = code.substr(second + 1, third - second - 1);
    const std::string effectName = code.substr(third + 1, fourth - third - 1);
    const std::string durationText = code.substr(fourth + 1);

    std::int32_t power = 0;
    std::int32_t duration = 0;
    try
    {
        power = std::stoi(powerText);
        duration = std::stoi(durationText);
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "[Item Code Error] power and duration must be numbers, got '" << code << "'\n";
        return nullptr;
    }

    auto effect = std::make_shared<combat::Effect>(effectName, duration);

    // Сравнение строк: тип определяет, какой наследник создаётся.
    if (type == "weapon")
    {
        return std::make_unique<combat::Weapon>(name, power, std::move(effect));
    }
    if (type == "armor")
    {
        return std::make_unique<combat::Armor>(name, power, std::move(effect));
    }
    if (type == "consumable")
    {
        return std::make_unique<combat::Consumable>(name, power, std::move(effect));
    }

    std::cout << "[Item Code Error] unknown item type '" << type << "'\n";
    return nullptr;
}

} // namespace

int main()
{
    using combat::Armor;
    using combat::BaseItem;
    using combat::Consumable;
    using combat::Effect;
    using combat::Fighter;
    using combat::Inventory;
    using combat::SharedEffectTag;
    using combat::Suite;
    using combat::Weapon;

    // ---------- Часть ЛР 3: умные указатели ----------

    PrintHeader("1. unique_ptr: destruction at scope exit, ownership transfer with std::move");
    {
        auto first = std::make_unique<Fighter>(3, 90, 30, 1, "Spark", 10, 8, 2);
        auto second = std::move(first);   // владение перешло к second
        std::cout << "first is empty after std::move: " << (first == nullptr) << "\n";
    }                                    // деструктор Fighter 3 — здесь, без delete

    PrintHeader("2. shared_ptr: use_count; weak_ptr: lock() before and after destruction");
    auto owner = std::make_shared<Fighter>(2, 120, 80, 1, "Ice Lance", 50, 10, 5);
    std::cout << "use_count after make_shared: " << owner.use_count() << "\n";
    {
        const auto secondOwner = owner;
        std::cout << "use_count with a second owner: " << owner.use_count() << "\n";
    }
    std::cout << "use_count after the second owner is gone: " << owner.use_count() << "\n";

    const std::weak_ptr<Fighter> observer = owner;
    if (const auto locked = observer.lock())
    {
        std::cout << "weak_ptr lock() while the fighter is alive: fighter " << locked->GetId() << "\n";
    }
    owner.reset();   // последний владелец ушёл — здесь деструктор
    std::cout << "weak_ptr expired after the last owner released the fighter: " << observer.expired() << "\n";

    // ---------- Часть ЛР 4: наследование ----------

    PrintHeader("3. Constructors: default and with parameters; the base constructor runs first");
    {
        // Конструктор по умолчанию.
        const Weapon trainingWeapon;

        // Конструктор с параметрами: Weapon вызывает конструктор BaseItem в списке инициализации.
        const Weapon sword("Fire Sword", 15, std::make_shared<Effect>("Bleeding", 3));
    }
    // Объекты покидают область видимости в порядке, обратном созданию:
    // сначала sword (~Weapon, затем ~BaseItem), потом trainingWeapon.

    PrintHeader("4. Copy constructor (deep copy) and move constructor");
    {
        Weapon original("Ice Axe", 12, std::make_shared<Effect>("Freeze", 2));
        const Weapon copy{ original };        // конструктор копирования: эффект копируется
        std::cout << "copy: " << copy.Describe() << "\n";

        Weapon moved{ std::move(original) };  // конструктор перемещения: содержимое переходит в новый объект
        std::cout << "moved-from item: #" << original.GetItemNumber() << ", name '" << original.GetName() << "'\n";
        std::cout << "moved item: #" << moved.GetItemNumber() << ", effect owners: " << moved.GetEffectOwnerCount() << "\n";

        Weapon receiver("Old Sword", 1, std::make_shared<Effect>("None", 0));
        receiver = std::move(moved);          // оператор присваивания перемещением
    }

    PrintHeader("5. Virtual destructor: an heir is destroyed through unique_ptr<BaseItem>");
    {
        std::unique_ptr<BaseItem> item =
            std::make_unique<Armor>("Knight Armor", 12, std::make_shared<Effect>("Protection", 2));
        item.reset();   // уничтожение через указатель на базовый класс: сначала ~Armor, затем ~BaseItem
    }

    PrintHeader("6. Polymorphism: an array of unique_ptr<BaseItem> with different heirs");
    const auto hero = std::make_shared<Fighter>(1, 100, 50, 1, "Fireball", 20, 10, 5);
    Suite party("Alpha Squad");
    party.AddFighter(hero);

    std::array<std::unique_ptr<BaseItem>, 3> loadout = {{
        std::make_unique<Weapon>("Fire Sword", 15, std::make_shared<Effect>("Bleeding", 3)),
        std::make_unique<Armor>("Knight Armor", 12, std::make_shared<Effect>("Protection", 2)),
        std::make_unique<Consumable>("Health Potion", 40, std::make_shared<Effect>("Regeneration", 2)),
    }};

    hero->TakeDamage(60);
    for (const auto& item : loadout)
    {
        // Указатель на базовый класс: виртуальный Describe() выбирается по фактическому типу предмета.
        std::cout << "  - " << item->Describe() << "\n";
    }
    for (const auto& item : loadout)
    {
        // Чисто виртуальный Apply() в каждом наследнике реализован по-своему.
        item->Apply(*hero);
    }
    party.InspectSuite();

    Inventory stash("Stash");
    for (auto& item : loadout)
    {
        // Владение предметом переходит инвентарю.
        const bool transferred = stash.Add(std::move(item));
        std::cout << "transfer accepted: " << transferred << "\n";
    }

    PrintHeader("7. A shallow copy shares the nested effect, a deep copy owns its own");
    {
        Weapon dagger("Poisoned Dagger", 8, std::make_shared<Effect>("Poison", 3));

        Weapon deepCopy{ dagger };                        // глубокая копия: свой экземпляр эффекта
        Weapon shallowCopy{ SharedEffectTag{}, dagger };  // поверхностная копия: эффект общий с оригиналом

        std::cout << "effect owners: original " << dagger.GetEffectOwnerCount()
                  << ", deep copy " << deepCopy.GetEffectOwnerCount()
                  << ", shallow copy " << shallowCopy.GetEffectOwnerCount() << "\n";

        deepCopy.GetEffect().SetDuration(9);     // меняет только эффект своей копии
        std::cout << "after deepCopy.SetDuration(9): original duration "
                  << dagger.GetEffect().GetDuration() << " (unchanged)\n";

        shallowCopy.GetEffect().SetDuration(1);  // меняет общий эффект
        std::cout << "after shallowCopy.SetDuration(1): original duration "
                  << dagger.GetEffect().GetDuration() << " (changed)\n";
    }

    PrintHeader("8. Clone(): the object itself makes a copy through a base pointer");
    {
        std::unique_ptr<BaseItem> basePointer =
            std::make_unique<Consumable>("Mana Potion", 30, std::make_shared<Effect>("Clarity", 1));
        const std::unique_ptr<BaseItem> twin = basePointer->Clone();   // копия того же типа, глубокая

        std::cout << "original: " << basePointer->Describe() << "\n";
        std::cout << "clone:    " << twin->Describe() << "\n";

        // this: предмет передаёт себя (*this) инвентарю, инвентарь кладёт его копию.
        const bool stored = basePointer->StoreCopyIn(stash);
        std::cout << "copy stored in the inventory: " << stored << "\n";
    }

    PrintHeader("9. this: self-assignment check in operator= and returning *this");
    {
        Weapon target("Training Bow", 5, std::make_shared<Effect>("None", 0));
        const Weapon source("Hunter Bow", 14, std::make_shared<Effect>("Piercing", 1));
        Weapon another("Short Bow", 6, std::make_shared<Effect>("None", 0));

        // Самоприсваивание в реальном коде происходит через другое имя того же предмета.
        Weapon& sameItem = target;
        target = sameItem;
        std::cout << "after target = target: '" << target.GetName()
                  << "', effect " << target.GetEffect().Describe() << "\n";

        another = target = source;   // цепочка: target = source возвращает *this, его получает another
        std::cout << "after another = target = source: '" << another.GetName()
                  << "' and '" << target.GetName() << "'\n";
        std::cout << "effect owners: another " << another.GetEffectOwnerCount()
                  << ", target " << target.GetEffectOwnerCount()
                  << ", source " << source.GetEffectOwnerCount() << " (each item has its own copy)\n";
    }

    PrintHeader("10. std::string: concatenation, find, substr, comparison");
    {
        // Конкатенация: код предмета собирается из частей.
        const std::string itemCode =
            std::string("weapon:") + "Fire Sword:" + std::to_string(15) + ":Bleeding:3";
        std::cout << "item code: " << itemCode << "\n";

        // find и substr разбирают код, сравнение строк выбирает наследника.
        auto parsed = MakeItemFromCode(itemCode);
        std::cout << "parsed: " << parsed->Describe() << "\n";
        const bool parsedStored = stash.Add(std::move(parsed));
        std::cout << "parsed item stored: " << parsedStored << "\n";

        const auto unknown = MakeItemFromCode("scroll:Fire Scroll:10:Burning:2");
        std::cout << "unknown item type gives an empty pointer: " << (unknown == nullptr) << "\n";

        // find: поиск предметов по части названия.
        std::cout << "items with 'Potion' in the name:\n";
        for (const std::string& description : stash.FindByName("Potion"))
        {
            std::cout << "  - " << description << "\n";
        }

        // Правило: в инвентаре не больше MAX_CAPACITY предметов.
        auto spare = std::make_unique<Weapon>("Spare Sword", 1, std::make_shared<Effect>("None", 0));
        const std::size_t freeSlots = Inventory::MAX_CAPACITY - stash.GetSize();
        for (std::size_t slot = 0; slot < freeSlots; ++slot)
        {
            // Право владения передаётся инвентарю: параметр Add принимается по значению.
            const bool filled =
                stash.Add(std::make_unique<Consumable>("Small Potion", 5, std::make_shared<Effect>("None", 0)));
            std::cout << "free slot filled: " << filled << "\n";
        }
        std::cout << "inventory is full: " << stash.IsFull()
                  << ", the spare item was not handed over and stays with the caller: " << (spare != nullptr) << "\n";

        // Если всё же передать предмет в полный инвентарь, инвентарь откажет,
        // а предмет будет уничтожен вместе с параметром.
        const bool accepted = stash.Add(std::move(spare));
        std::cout << "adding to a full inventory accepted: " << accepted
                  << ", the rejected item is destroyed: " << (spare == nullptr) << "\n";

        // Сравнение (<): сортировка по названию.
        stash.SortByName();
        stash.PrintContents();
    }

    PrintHeader("11. Static members: one created-items counter for the whole class");
    std::cout << "BaseItem::GetCreatedCount() is called without any object: " << BaseItem::GetCreatedCount() << "\n";
    {
        const Weapon temporary("Broken Sword", 2, std::make_shared<Effect>("None", 0));
        std::cout << "after one more item was created: " << BaseItem::GetCreatedCount() << "\n";
    }
    std::cout << "after it was destroyed: " << BaseItem::GetCreatedCount()
              << " (the counter counts created items: copies and clones included)\n";

    std::cout << "\n=== Program finished ===\n";
    return 0;
}
