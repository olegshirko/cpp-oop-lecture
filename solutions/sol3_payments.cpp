// Решение следующего занятия. Сначала выполните tasks/task3_payments.cpp.
// ============================================================
// СЛЕДУЮЩЕЕ ЗАНЯТИЕ. Полиморфизм: исправьте ошибки и добавьте способ оплаты
// ============================================================
// Требуются наследование, virtual и override; в текущие две пары не входит.
// Работайте по этапам:
// после каждого этапа увеличивайте  #define STAGE  и перекомпилируйте.
//
// ЭТАП 1. Платёж через Payment& вызывает Payment::pay, а не GiftCardPayment::pay.
//         Допишите override к GiftCardPayment::pay — прочитайте ошибку компилятора
//         и исправьте сигнатуру.
// ЭТАП 2. Для удаления через Payment* нужен виртуальный деструктор базы.
//         Без него поведение не определено (UB); static_assert не даст запустить
//         такой вариант. Исправьте деструктор и проверьте порядок сообщений.
// ЭТАП 3. Сделайте Payment абстрактным (чисто виртуальный pay) — «платёж вообще» не существует.
// ЭТАП 4. Напишите BonusPayment (оплата бонусными баллами, 1 балл = 1 рубль):
//           - конструктор BonusPayment(int points)
//           - отрицательное число баллов в конструкторе -> std::invalid_argument;
//           - pay(): сумма > 0 и кратна 100 копейкам; иначе false без списания;
//           - если баллов хватает — списать целые рубли и вернуть true, иначе false
//           - класс должен быть final
//
// Сборка из корня: g++ -std=c++20 -Wall -Wextra solutions/sol3_payments.cpp -o task3 && ./task3
// ============================================================
#define STAGE 4

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "../demo/money.hpp"

#ifdef NDEBUG
#error "Проверки требуют assert: собирайте без -DNDEBUG"
#endif

class Payment {
public:
    virtual ~Payment() { std::cout << "  ~Payment\n"; }        // баг 2: виртуальный деструктор
    virtual bool pay(Money amount) = 0;                          // этап 3: чисто виртуальная
};

class GiftCardPayment : public Payment {
public:
    explicit GiftCardPayment(Money balance) : balance_(balance) {
        if (balance.kopecks() < 0) throw std::invalid_argument("баланс не может быть < 0");
    }
    ~GiftCardPayment() override { std::cout << "  ~GiftCardPayment: карта деактивирована\n"; }

    bool pay(Money amount) override {                            // баг 1: сигнатура + override
        if (amount.kopecks() <= 0) return false;
        if (balance_ < amount) return false;
        balance_ -= amount;
        return true;
    }
private:
    Money balance_;
};

class BonusPayment final : public Payment {
public:
    explicit BonusPayment(int points) : points_(points) {
        if (points < 0) throw std::invalid_argument("баллы не могут быть < 0");
    }
    bool pay(Money amount) override {
        if (amount.kopecks() <= 0 || amount.kopecks() % 100 != 0) return false;
        long long rub = amount.kopecks() / 100;                  // точное число баллов
        if (points_ < rub) return false;
        points_ -= static_cast<int>(rub);
        return true;
    }
private:
    int points_;
};


// ----------------------- Проверки (не менять) -----------------------
template <class F>
std::string capture(F f) {
    std::ostringstream buf;
    auto* old = std::cout.rdbuf(buf.rdbuf());
    try {
        f();
    } catch (...) {
        std::cout.rdbuf(old);
        throw;
    }
    std::cout.rdbuf(old);
    return buf.str();
}

int main() {
#if STAGE >= 1
    {
        // Объект на стеке: нет удаления через базовый указатель до этапа 2.
        GiftCardPayment card(Money::rub(500));
        Payment& p = card;
        assert(p.pay(Money::rub(300)) == true);      // после исправления сигнатуры
        assert(p.pay(Money::rub(300)) == false);     // осталось 200 руб.
        assert(p.pay(Money(0)) == false);
        assert(p.pay(Money(-100)) == false);
        assert(p.pay(Money::rub(200)) == true);      // отказы не меняли баланс
    }
    std::cout << "Этап 1 пройден\n";
#endif
#if STAGE >= 2
    static_assert(std::has_virtual_destructor_v<Payment>,
                  "сначала сделайте деструктор Payment виртуальным");
    {
        std::string out = capture([] {
            std::unique_ptr<Payment> p = std::make_unique<GiftCardPayment>(Money(0));
        });
        assert(out.find("карта деактивирована") != std::string::npos);
    }
    std::cout << "Этап 2 пройден\n";
#endif
#if STAGE >= 3
    static_assert(std::is_abstract_v<Payment>, "Payment должен быть абстрактным");
    std::cout << "Этап 3 пройден\n";
#endif
#if STAGE >= 4
    static_assert(std::is_final_v<BonusPayment>, "BonusPayment должен быть final");
    {
        std::unique_ptr<Payment> p = std::make_unique<BonusPayment>(250);
        assert(p->pay(Money::rub(200)) == true);     // осталось 50 баллов
        assert(p->pay(Money::rub(100)) == false);    // не хватает
        assert(p->pay(Money(0))       == false);
        assert(p->pay(Money(-100))    == false);
        assert(p->pay(Money(99))      == false);   // неполный рубль нельзя оплатить бесплатно
        assert(p->pay(Money::rub(50)) == true);    // отказы не уменьшили 50 баллов
    }
    std::cout << "Этап 4 пройден\n";
#endif
    std::cout << "Задание 3: все проверки для STAGE=" << STAGE << " пройдены!\n";
}
