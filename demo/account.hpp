#pragma once
// ============================================================
// Account — банковский счёт. Главная идея: ИНВАРИАНТ balance >= 0
// Публичные операции проверяют суммы и сохраняют это правило.
// Как и в Money, работаем с небольшими суммами без переполнения long long.
// ============================================================
#include <stdexcept>
#include "money.hpp"

class Account {
public:
    // Конструктор задаёт допустимое начальное состояние.
    explicit Account(Money initial) : balance_(initial) {
        if (initial.kopecks() < 0)
            throw std::invalid_argument("начальный баланс не может быть < 0");
    }

    void deposit(Money m) {
        if (m.kopecks() <= 0) throw std::invalid_argument("сумма пополнения должна быть > 0");
        balance_.add(m);
    }

    // false — если денег не хватает (баланс не меняется)
    bool withdraw(Money m) {
        if (m.kopecks() <= 0) throw std::invalid_argument("сумма списания должна быть > 0");
        if (balance_.kopecks() < m.kopecks()) return false;
        balance_.subtract(m);
        return true;
    }

    Money balance() const { return balance_; }   // наблюдатель (const)

private:
    Money balance_;   // Доступ закрыт; корректность обеспечивают проверки в методах.
};
