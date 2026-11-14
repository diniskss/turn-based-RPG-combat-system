#include <iostream>
#include "fighter.hpp"
#include "suite.hpp"

int main()
{
    // unique_ptr: удаление при выходе из блока + std::move
    {
        auto u1 = std::make_unique<combat::Fighter>(3, 90, 30, 1, "Spark", 10);
        auto u2 = std::move(u1);
        std::cout << "u1 empty: " << (u1 == nullptr) << "\n";   // 1
    } // деструктор Fighter 3 тут, без delete

    // массив: vector<unique_ptr>
    std::vector<std::unique_ptr<combat::Fighter>> arr;
    arr.push_back(std::make_unique<combat::Fighter>(4, 110, 60, 1, "Shield", 10));

    // shared_ptr: use_count
    auto s1 = std::make_shared<combat::Fighter>(2, 120, 80, 1, "Ice Lance", 50);
    std::cout << s1.use_count() << "\n";        // 1
    {
        auto s2 = s1;
        std::cout << s1.use_count() << "\n";    // 2
    }
    std::cout << s1.use_count() << "\n";        // 1

    // weak_ptr: lock() до и после
    std::weak_ptr<combat::Fighter> w = s1;
    if (auto p = w.lock()) { std::cout << "alive, id " << p->GetId() << "\n"; }
    s1.reset();                                  // последний владелец ушёл, тут деструктор
    if (auto p = w.lock()) { /* не зайдёт */ } else { std::cout << "expired\n"; }
}