#include <iostream>
#include "fighter.hpp"
#include "suite.hpp"

int main()
{
    std::cout << "=== 1. Static initialization & pointer/reference demo ===\n";
    combat::Fighter staticFighter(1, 100, 50, 1);
    combat::Fighter& refFighter = staticFighter;
    combat::Fighter* ptrFighter = &staticFighter;

    refFighter.TakeDamage(30);
    ptrFighter->CastSpell(20);

    std::cout << "\n=== 2. Dynamic initialization (new / delete) & Aggregation demo ===\n";
    // Создаем бойца динамически во внешнем блоке (живет дольше агрегатора)
    combat::Fighter* externalFighter = new combat::Fighter(2, 120, 80, 1);

    {
        combat::Suite mySuite("Alpha Squad");
        mySuite.AddFighter(externalFighter); // Передаем внешнего бойца в свиту (агрегация)
        mySuite.InspectSuite();
        
        // Проверка срабатывания правил (успех, затем отказ)
        externalFighter->CastSpell(40); // Должно пройти
        externalFighter->CastSpell(100); // Должно отказать (мало маны)

        std::cout << "\n--- Leaving suite scope (Suite destruction) ---\n";
    } // Здесь уничтожается `mySuite`. Боец НЕ должен уничтожиться!

    std::cout << "\n=== 3. Verification that aggregated object is still alive ===\n";
    std::cout << "External fighter HP after suite is gone: " << externalFighter->GetHealth() << "\n";
    
    // Ручное удаление динамического бойца (так как он выделен через new)
    delete externalFighter;

    std::cout << "\n=== 4. Dynamic array and array of dynamic objects demo ===\n";
    // Массив динамических объектов (указателей)
    combat::Fighter* fightersArray[2] = {
        new combat::Fighter(3, 90, 30, 1),
        new combat::Fighter(4, 110, 60, 1)
    };

    fightersArray[0]->TakeDamage(50);
    fightersArray[1]->CastSpell(10);

    // Очистка памяти массива
    delete fightersArray[0];
    delete fightersArray[1];

    std::cout << "=== Program finished successfully ===\n";
    return 0;
}