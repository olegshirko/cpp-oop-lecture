// ПОКАЗ 1. Money: зачем нужен класс, конструктор, explicit, операторы
// Сборка: g++ -std=c++20 -Wall -Wextra demo1_money.cpp -o demo1 && ./demo1
#include <iomanip>
#include <iostream>
#include "money.hpp"

int main() {
    // --- Проблема: деньги в double ---
    double a = 0.1 + 0.2;
    std::cout << std::boolalpha << "0.1 + 0.2 == 0.3 ?  " << (a == 0.3) << "\n";
    std::cout << std::setprecision(17) << "0.1 + 0.2 = " << a << "\n\n";

    // --- Решение: класс Money ---
    Money price = Money::rub(199, 90);   // 199.90
    Money delivery(35000);               // 350.00 (в копейках — явно!)
    Money total = price + delivery;
    std::cout << "Товар:     " << price << "\n";
    std::cout << "Доставка:  " << delivery << "\n";
    std::cout << "Итого:     " << total << "\n";
    std::cout << "Дороже 500 руб.? " << (total > Money::rub(500)) << "\n";

    // Money bad = 100;              // ОШИБКА: explicit — 100 чего: рублей или копеек?
    // total.kopecks_ = 5;           // ОШИБКА: kopecks_ is private
    // const Money c(100); c += price;  // ОШИБКА: += меняет объект, а он const
    return 0;
}
