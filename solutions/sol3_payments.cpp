// РЕШЕНИЕ (для преподавателя). Условие — в tasks/task3_payments.cpp
// ============================================================
// ЗАДАНИЕ 3 (20–25 мин). Полиморфизм: найди баги и добавь новый способ оплаты
// ============================================================
// Коллега написал способы оплаты, но «что-то не работает». Работайте по этапам:
// после каждого этапа увеличивайте  #define STAGE  и перекомпилируйте.
//
// ЭТАП 1. Платёж через  Payment*  вызывает Payment::pay (заглушку), а не GiftCardPayment::pay.
//         Допишите override к GiftCardPayment::pay — прочитайте ошибку компилятора
//         и исправьте сигнатуру.
// ЭТАП 2. Запустите с -Wall: компилятор предупреждает об удалении через указатель на базу.
//         Деструктор GiftCardPayment не вызывается (карта не деактивируется!). Исправьте.
// ЭТАП 3. Сделайте Payment абстрактным (чисто виртуальный pay) — «платёж вообще» не существует.
// ЭТАП 4. Напишите BonusPayment (оплата бонусными баллами, 1 балл = 1 рубль):
//           - конструктор BonusPayment(int points)
//           - pay(): если баллов хватает — списать целые рубли и вернуть true, иначе false
//           - класс должен быть final
//
// Сборка:  g++ -std=c++20 -Wall -Wextra task3_payments.cpp -o task3 && ./task3
// ============================================================
#define STAGE 4

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include "../demo/money.hpp"

class Payment {
public:
    virtual ~Payment() { std::cout << "  ~Payment\n"; }        // баг 2: виртуальный деструктор
    virtual bool pay(Money amount) = 0;                          // этап 3: чисто виртуальная
};

class GiftCardPayment : public Payment {
public:
    explicit GiftCardPayment(Money balance) : balance_(balance) {}
    ~GiftCardPayment() override { std::cout << "  ~GiftCardPayment: карта деактивирована\n"; }

    bool pay(Money amount) override {                            // баг 1: сигнатура + override
        if (balance_ < amount) return false;
        balance_ -= amount;
        return true;
    }
private:
    Money balance_;
};

class BonusPayment final : public Payment {
public:
    explicit BonusPayment(int points) : points_(points) {}
    bool pay(Money amount) override {
        long long rub = amount.kopecks() / 100;                  // целые рубли
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
    f();
    std::cout.rdbuf(old);
    return buf.str();
}

int main() {
#if STAGE >= 1
    {
        std::unique_ptr<Payment> p = std::make_unique<GiftCardPayment>(Money::rub(500));
        assert(p->pay(Money::rub(300)) == true);     // сработает, только если pay переопределён
        assert(p->pay(Money::rub(300)) == false);    // осталось 200 руб.
    }
    std::cout << "Этап 1 пройден\n";
#endif
#if STAGE >= 2
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
        assert(p->pay(Money::rub(50))  == true);
    }
    std::cout << "Этап 4 пройден\n";
#endif
    std::cout << "Задание 3: все проверки для STAGE=" << STAGE << " пройдены!\n";
}
