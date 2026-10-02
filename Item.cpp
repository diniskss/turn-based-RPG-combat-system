#include "Item.hpp"
#include 

Item::Item() : name("Неизвестный предмет"), weight(1), quantity(1) {}

Item::Item(const std::string& name, int weight, int quantity)
    : name(name), weight(weight), quantity(quantity) {
    if (this->weight <= 0) {
        std::cout << "[Ошибка Item]: Вес предмета должен быть > 0. Установлен вес 1.\n";
        this->weight = 1;
    }
    if (this->quantity < 0) {
        std::cout << "[Ошибка Item]: Количество не может быть отрицательным. Установлено 0.\n";
        this->quantity = 0;
    }
}

Item::~Item() {
    std::cout << "[Деструктор Item]: Предмет \"" << name << "\" уничтожен.\n";
}

bool Item::use(int amount) {
    if (amount <= 0) {
        std::cout << "[Ошибка Item]: Количество для использования должно быть > 0.\n";
        return false;
    }
    if (quantity < amount) {
        std::cout << "[Отказ Item]: Недостаточно предметов \"" << name 
                  << "\" (Запрошено: " << amount << ", есть: " << quantity << ").\n";
        return false;
    }
    quantity -= amount;
    std::cout << "[Успех Item]: Использовано " << amount << " шт. предмета \"" << name << "\".\n";
    return true;
}

void Item::addQuantity(int amount) {
    if (amount <= 0) {
        std::cout << "[Ошибка Item]: Нельзя добавить отрицательное или нулевое количество.\n";
        return;
    }
    quantity += amount;
}

std::string Item::getName() const { return name; }
int Item::getWeight() const { return weight; }
int Item::getQuantity() const { return quantity; }
int Item::getTotalWeight() const { return weight * quantity; }