#include <iostream>
#include "fighter.hpp"
#include "suite.hpp"

int main()
{
    std::cout << "=== 1. Static initialization & pointer/reference demo ===\n";
    combat::Fighter staticFighter(1, 100, 50, 1, "Fireball", 20);
    combat::Fighter& refFighter = staticFighter;
    combat::Fighter* ptrFighter = &staticFighter;

    refFighter.TakeDamage(30);
    ptrFighter->CastSpell();

    std::cout << "\n=== 2. Dynamic initialization (new / delete) & Aggregation demo ===\n";
    combat::Fighter* externalFighter = new combat::Fighter(2, 120, 80, 1, "Ice Lance", 50);

    {
        combat::Suite mySuite("Alpha Squad");
        mySuite.AddFighter(externalFighter);
        mySuite.InspectSuite();

        externalFighter->CastSpell(); // 80 -> 30, должно пройти
        externalFighter->CastSpell(); // 30 < 50, должно отказать

        std::cout << "\n--- Leaving suite scope (Suite destruction) ---\n";
    } // Уничтожается mySuite. Боец НЕ должен уничтожиться!

    std::cout << "\n=== 3. Verification that aggregated object is still alive ===\n";
    std::cout << "External fighter HP after suite is gone: " << externalFighter->GetHealth() << "\n";

    delete externalFighter;

    std::cout << "\n=== 4. Dynamic array and array of dynamic objects demo ===\n";
    combat::Fighter* fightersArray[2] = {
        new combat::Fighter(3, 90, 30, 1, "Spark", 10),
        new combat::Fighter(4, 110, 60, 1, "Shield", 10)
    };

    fightersArray[0]->TakeDamage(50);
    fightersArray[1]->CastSpell();

    delete fightersArray[0];
    delete fightersArray[1];

    std::cout << "\n=== 5. Composition demo (destruction order) ===\n";
    {
        combat::Fighter composed(5, 80, 40, 1, "Lightning", 15);
        std::cout << "--- Leaving block (Fighter destruction) ---\n";
    } // Сначала ~Fighter, потом автоматически ~Spell

    std::cout << "=== Program finished successfully ===\n";
    return 0;
}