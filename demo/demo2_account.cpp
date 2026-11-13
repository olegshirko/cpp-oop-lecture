// ПАРА 1, ДО перегрузки операторов. Account: инвариант и проверки операций.
// Сборка: g++ -std=c++20 -Wall -Wextra demo2_account.cpp -o demo2 && ./demo2
#include <cassert>
#include <iostream>
#include "account.hpp"

struct BadAccount { Money balance; };

int main() {
    BadAccount bad{Money(100000)};
    bad.balance = Money(-99999900);
    std::cout << "Открытое поле: " << bad.balance.str() << " — правило нарушено\n\n";

    Account acc(Money(100000));
    acc.deposit(Money(50000));
    std::cout << "После пополнения: " << acc.balance().str() << "\n";

    bool rejected = acc.withdraw(Money(200000));
    assert(!rejected);
    assert(acc.balance().kopecks() == 150000);
    std::cout << "Снять 2000 руб.: отказ; баланс " << acc.balance().str() << "\n";

    bool accepted = acc.withdraw(Money(30000));
    assert(accepted);
    assert(acc.balance().kopecks() == 120000);
    std::cout << "Снять 300 руб.: успех; баланс " << acc.balance().str() << "\n";

    try {
        acc.withdraw(Money(-100));
        assert(false && "отрицательная сумма должна быть отвергнута");
    } catch (const std::invalid_argument& e) {
        std::cout << "Отрицательное снятие: " << e.what() << "\n";
    }
    assert(acc.balance().kopecks() == 120000);

    try {
        Account broken(Money(-500));
        assert(false && "отрицательный начальный баланс должен быть отвергнут");
    } catch (const std::invalid_argument& e) {
        std::cout << "Конструктор отказал: " << e.what() << "\n";
    }

    // balance() возвращает значение: изменение копии не меняет счёт.
    Money snapshot = acc.balance();
    snapshot.subtract(Money(120000));
    assert(snapshot.kopecks() == 0);
    assert(acc.balance().kopecks() == 120000);

    // acc.balance_ = Money(0);  // ОШИБКА: поле закрыто.
    // const Account ca(Money(0)); ca.deposit(Money(1));  // ОШИБКА: deposit не const.
}
