#pragma once
#include "Item.hpp"

class Inventory {
private:
    int maxWeight;
    Item* items;        // Массив предметов (Композиция: создаются и уничтожаются вместе с Inventory)
    int itemCount;
    int capacity;

    void resize();

public:
    Inventory();
    explicit Inventory(int maxWeight);
    ~Inventory();

    // Содержательные методы
    bool addItem(const Item& item);
    void printContents() const;

    // Геттеры
    int getCurrentWeight() const;
    int getMaxWeight() const;
};