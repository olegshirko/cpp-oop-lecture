// РЕШЕНИЕ (для преподавателя). Условие — в tasks/task1_product.cpp
// ============================================================
// ЗАДАНИЕ 1 (15–20 мин). Класс Product — инвариант и инкапсуляция
// ============================================================
// Магазину нужен товар. Напишите класс Product (выше main).
//
//  Закрытые поля:   name_ (std::string), price_ (Money), stock_ (int)
//
//  Конструктор Product(std::string name, Money price, int stock)
//     бросает std::invalid_argument, если:  name пустое,  price <= 0,  stock < 0
//     (это ИНВАРИАНТ класса: невалидный товар создать нельзя;
//      проверку пишите в конструкторе, поля инициализируйте списком инициализации)
//
//  const-методы:     name(), price(), stock()
//
//  bool sell(int n)  — продать n штук:
//     n <= 0               -> бросить std::invalid_argument
//     на складе меньше n   -> вернуть false, склад НЕ менять
//     иначе                -> уменьшить склад, вернуть true
//
//  Money total_price(int n) const — стоимость n штук.
//     Для этого перегрузите оператор:   Money operator*(Money, int)
//     (свободная функция, до класса Product)
//
// Сборка (НЕ используйте -DNDEBUG, иначе assert отключится):
//     g++ -std=c++20 -Wall -Wextra task1_product.cpp -o task1 && ./task1
// Пока вы не написали класс, файл не компилируется — это нормально.
//
// Для сильных: добавьте метод restock(int n) и объясните, почему поле stock_
//              нельзя сделать public.
// ============================================================
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../demo/money.hpp"

inline Money operator*(Money m, int n) { return Money(m.kopecks() * n); }

class Product {
public:
    Product(std::string name, Money price, int stock)
        : name_(std::move(name)), price_(price), stock_(stock) {
        if (name_.empty())          throw std::invalid_argument("пустое название");
        if (price_ <= Money(0))     throw std::invalid_argument("цена должна быть > 0");
        if (stock_ < 0)             throw std::invalid_argument("остаток не может быть < 0");
    }

    const std::string& name() const { return name_; }
    Money price() const { return price_; }
    int stock() const { return stock_; }

    bool sell(int n) {
        if (n <= 0) throw std::invalid_argument("количество должно быть > 0");
        if (stock_ < n) return false;
        stock_ -= n;
        return true;
    }

    Money total_price(int n) const { return price_ * n; }

private:
    std::string name_;
    Money price_;
    int stock_;
};


// ----------------------- Проверки (не менять) -----------------------
template <class Ex, class F>
bool throws(F f) {
    try { f(); } catch (const Ex&) { return true; } catch (...) {}
    return false;
}

int main() {
    assert(Money::rub(10) * 3 == Money::rub(30));

    Product p("Клавиатура", Money::rub(2500), 10);
    assert(p.name() == "Клавиатура");
    assert(p.price() == Money::rub(2500));
    assert(p.stock() == 10);

    assert(p.sell(3) == true);
    assert(p.stock() == 7);
    assert(p.sell(100) == false);
    assert(p.stock() == 7);               // склад не изменился
    assert(p.total_price(2) == Money::rub(5000));

    const Product& cp = p;                // const-методы должны вызываться
    assert(cp.stock() == 7 && cp.name() == "Клавиатура" && cp.price() == Money::rub(2500));

    assert(throws<std::invalid_argument>([] { Product("",  Money::rub(1), 1); }));
    assert(throws<std::invalid_argument>([] { Product("A", Money(0),      1); }));
    assert(throws<std::invalid_argument>([] { Product("A", Money::rub(1), -1); }));
    assert(throws<std::invalid_argument>([&] { p.sell(0); }));

    std::cout << "Задание 1: все проверки пройдены!\n";
}
