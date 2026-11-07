#pragma once
// ============================================================
// Способы оплаты: интерфейс + три реализации (полиморфизм)
// ============================================================
#include <iostream>
#include <string>
#include "account.hpp"

// Абстрактный класс = «контракт»: ЧТО умеет способ оплаты (не КАК)
class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;          // виртуальный деструктор — обязателен!
    virtual bool pay(Money amount) = 0;          // чисто виртуальная: = 0
    virtual std::string name() const = 0;
};

class CardPayment : public PaymentMethod {
public:
    explicit CardPayment(Account& acc) : acc_(acc) {}
    ~CardPayment() override { std::cout << "  [Карта] соединение с банком закрыто\n"; }

    bool pay(Money amount) override {
        std::cout << "  [Карта] списываем " << amount << "\n";
        return acc_.withdraw(amount);
    }
    std::string name() const override { return "Карта"; }
private:
    Account& acc_;   // ссылка: счёт принадлежит покупателю, не нам
};

class SbpPayment : public PaymentMethod {
public:
    explicit SbpPayment(std::string phone) : phone_(std::move(phone)) {}
    ~SbpPayment() override { std::cout << "  [СБП] сессия закрыта\n"; }

    bool pay(Money amount) override {
        if (amount > Money::rub(100000)) {
            std::cout << "  [СБП] превышен лимит 100 000 руб.\n";
            return false;
        }
        std::cout << "  [СБП] перевод по номеру " << phone_ << ": " << amount << "\n";
        return true;
    }
    std::string name() const override { return "СБП"; }
private:
    std::string phone_;
};

// final — от этого класса наследоваться больше нельзя
class CryptoPayment final : public PaymentMethod {
public:
    ~CryptoPayment() override { std::cout << "  [Крипто] кошелёк отключён\n"; }

    bool pay(Money amount) override {
        Money fee = Money::rub(50);   // комиссия сети
        std::cout << "  [Крипто] сумма " << amount << " + комиссия сети " << fee << "\n";
        return true;
    }
    std::string name() const override { return "Крипто"; }
};
