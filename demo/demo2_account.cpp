// ПОКАЗ 2. Account: инвариант, private, конструктор, const-методы
// Сборка: g++ -std=c++20 -Wall -Wextra demo2_account.cpp -o demo2 && ./demo2
#include <iostream>
#include "account.hpp"

// «Как было бы без инкапсуляции» — показать СНАЧАЛА, чтобы студенты сами увидели проблему
struct BadAccount { Money balance; };

int main() {
    BadAccount bad{Money::rub(1000)};
    bad.balance = Money::rub(-999999);   // компилятор не против! Кто следит за балансом?
    std::cout << "BadAccount: " << bad.balance << "  <-- инвариант сломан\n\n";

    // Теперь правильно
    Account acc(Money::rub(1000));
    acc.deposit(Money::rub(500));
    std::cout << "После пополнения: " << acc.balance() << "\n";

    std::cout << "Снять 2000: " << (acc.withdraw(Money::rub(2000)) ? "ок" : "не хватает средств") << "\n";
    std::cout << "Снять 300:  " << (acc.withdraw(Money::rub(300))  ? "ок" : "не хватает средств") << "\n";
    std::cout << "Баланс: " << acc.balance() << "\n";

    // Невалидный объект создать нельзя — конструктор не даст
    try {
        Account broken(Money::rub(-5));
    } catch (const std::invalid_argument& e) {
        std::cout << "Конструктор отказал: " << e.what() << "\n";
    }

    // acc.balance_ = Money(0);   // ОШИБКА: private
    // const Account ca(Money(0)); ca.deposit(Money(1));  // ОШИБКА: deposit не const
    return 0;
}
