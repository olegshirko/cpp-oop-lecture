#pragma once
// ============================================================
// Account — банковский счёт. Главная идея: ИНВАРИАНТ balance >= 0
// Класс сам охраняет своё состояние, снаружи его не сломать.
// ============================================================
#include <stdexcept>
#include "money.hpp"

class Account {
public:
    // Конструктор превращает «сырую память» в ВАЛИДНЫЙ объект
    explicit Account(Money initial) : balance_(initial) {
        if (initial < Money(0))
            throw std::invalid_argument("начальный баланс не может быть < 0");
    }

    void deposit(Money m) {
        if (m <= Money(0)) throw std::invalid_argument("сумма пополнения должна быть > 0");
        balance_ += m;
    }

    // false — если денег не хватает (баланс не меняется)
    bool withdraw(Money m) {
        if (m <= Money(0)) throw std::invalid_argument("сумма списания должна быть > 0");
        if (balance_ < m) return false;
        balance_ -= m;
        return true;
    }

    Money balance() const { return balance_; }   // наблюдатель (const)

private:
    Money balance_;   // private -> инвариант нельзя нарушить снаружи
};
