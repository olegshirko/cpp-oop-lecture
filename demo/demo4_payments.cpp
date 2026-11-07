// ПОКАЗ 4. Полиморфизм: PaymentMethod, virtual, override, abstract, final
// Сборка: g++ -std=c++20 -Wall -Wextra demo4_payments.cpp -o demo4 && ./demo4
#include <iostream>
#include <memory>
#include <vector>
#include "payments.hpp"

int main() {
    Account buyer(Money::rub(2000));

    // Один тип указателя — разное поведение. Order не знает, чем платят.
    std::vector<std::unique_ptr<PaymentMethod>> methods;
    methods.push_back(std::make_unique<CardPayment>(buyer));
    methods.push_back(std::make_unique<SbpPayment>("+7-900-123-45-67"));
    methods.push_back(std::make_unique<CryptoPayment>());

    for (auto& m : methods) {
        std::cout << "Оплата способом «" << m->name() << "» на 1500 руб.:\n";
        bool ok = m->pay(Money::rub(1500));
        std::cout << "  результат: " << (ok ? "успешно" : "ОТКАЗ") << "\n\n";
    }
    // Карта сработает только 1 раз (на счёте 2000) — но тут она одна, поэтому ок.
    // Попробуйте поменять сумму на 3000: карта откажет.

    std::cout << "--- конец main: вектор уничтожается, смотрим на деструкторы ---\n";

    // Раскомментировать и показать:
    // PaymentMethod p;                       // ОШИБКА: абстрактный класс
    // class Hack : public CryptoPayment {};  // ОШИБКА: CryptoPayment — final
    return 0;
}
