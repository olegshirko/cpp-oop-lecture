// ЗАДАНИЕ 1 (15–20 мин). Product: инвариант и const-методы
// Напишите класс Product с закрытыми полями:
//   name_ (std::string), price_ (Money), stock_ (int).
// Конструктор Product(std::string name, Money price, int stock):
//   name не пустое, price.kopecks() > 0, stock >= 0;
//   при нарушении бросает std::invalid_argument.
// Методы name(), price(), stock() — const, возвращают значения полей.
// bool sell(int n):
//   n <= 0 -> std::invalid_argument;
//   n > stock_ -> false, остаток не меняется;
//   иначе уменьшить stock_ и вернуть true.
//
// Дополнительно, после темы операторов (не нужно для основных проверок):
//   реализуйте Money operator*(Money, int) и total_price(int n) const;
//   договоритесь, что n > 0, и проверяйте это условие.
// Ещё один вариант расширения: restock(int n), n > 0.
// Сборка из корня репозитория:
//   g++ -std=c++20 -Wall -Wextra tasks/task1_product.cpp -o task1 && ./task1
// Пока класс не написан, файл не компилируется — это ожидается.
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../demo/money.hpp"

#ifdef NDEBUG
#error "Проверки требуют assert: собирайте без -DNDEBUG"
#endif

// TODO: class Product { ... };

// ----------------------- Проверки (не менять) -----------------------
template <class Ex, class F>
bool throws(F f) {
    try { f(); } catch (const Ex&) { return true; } catch (...) {}
    return false;
}

int main() {
    Product p("Клавиатура", Money(250000), 10);
    assert(p.name() == "Клавиатура");
    assert(p.price().kopecks() == 250000 && p.stock() == 10);
    bool sold = p.sell(3);
    assert(sold && p.stock() == 7);
    sold = p.sell(100);
    assert(!sold && p.stock() == 7);

    assert(throws<std::invalid_argument>([&] { p.sell(0); }));
    assert(throws<std::invalid_argument>([&] { p.sell(-1); }));
    assert(p.stock() == 7);
    sold = p.sell(7);
    assert(sold && p.stock() == 0);
    sold = p.sell(1);
    assert(!sold && p.stock() == 0);

    const Product cp("Мышь", Money(50000), 0);
    assert(cp.stock() == 0 && cp.name() == "Мышь");
    assert(cp.price().kopecks() == 50000);
    assert(throws<std::invalid_argument>([] { Product("", Money(1), 1); }));
    assert(throws<std::invalid_argument>([] { Product("A", Money(0), 1); }));
    assert(throws<std::invalid_argument>([] { Product("A", Money(-1), 1); }));
    assert(throws<std::invalid_argument>([] { Product("A", Money(1), -1); }));

    std::cout << "Задание 1: все проверки пройдены!\n";
}
