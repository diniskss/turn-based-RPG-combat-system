#pragma once

class Item {
private:
    std::string name;
    int weight;
    int quantity;

public:
    Item();
    Item(const std::string& name, int weight, int quantity);
    ~Item();

    // Содержательные методы
    bool use(int amount);
    void addQuantity(int amount);

    // Геттеры
    std::string getName() const;
    int getWeight() const;
    int getQuantity() const;
    int getTotalWeight() const;
};