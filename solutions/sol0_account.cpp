// Решение практики 0. Сначала выполните tasks/task0_account.cpp.
// ПРАКТИКА 0 (10–15 мин). Account: безопасное снятие денег
// Money и конструктор уже знакомы. Допишите только withdraw(Money m).
//   m <= 0: бросить std::invalid_argument, баланс не менять;
//   денег не хватает: вернуть false, баланс не менять;
//   иначе: вычесть m и вернуть true.
// Используйте m.kopecks(), balance_.kopecks() и balance_.subtract(m).
// Перегрузка операторов для этого задания не нужна.
// После выполнения объясните, почему private без проверок недостаточно.
// Сборка из корня репозитория:
//   g++ -std=c++20 -Wall -Wextra solutions/sol0_account.cpp -o sol0 && ./sol0
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "../demo/money.hpp"

#ifdef NDEBUG
#error "Проверки требуют assert: собирайте без -DNDEBUG"
#endif

class Account {
public:
    explicit Account(Money initial) : balance_(initial) {
        if (initial.kopecks() < 0)
            throw std::invalid_argument("начальный баланс не может быть < 0");
    }

    bool withdraw(Money m) {
        if (m.kopecks() <= 0)
            throw std::invalid_argument("сумма списания должна быть > 0");
        if (balance_.kopecks() < m.kopecks()) return false;
        balance_.subtract(m);
        return true;
    }

    Money balance() const { return balance_; }

private:
    Money balance_;
};

// ----------------------- Проверки (не менять) -----------------------
template <class Ex, class F>
bool throws(F f) {
    try { f(); } catch (const Ex&) { return true; } catch (...) {}
    return false;
}

int main() {
    Account a(Money(500)), b(Money(500));
    bool ok = a.withdraw(Money(200));
    assert(ok);
    assert(a.balance().kopecks() == 300);
    assert(b.balance().kopecks() == 500);  // второй объект независим

    ok = a.withdraw(Money(700));
    assert(!ok && a.balance().kopecks() == 300);
    assert(throws<std::invalid_argument>([&] { a.withdraw(Money(0)); }));
    assert(a.balance().kopecks() == 300);
    assert(throws<std::invalid_argument>([&] { a.withdraw(Money(-100)); }));
    assert(a.balance().kopecks() == 300);

    ok = a.withdraw(Money(300));
    assert(ok && a.balance().kopecks() == 0);  // можно снять весь остаток
    assert(throws<std::invalid_argument>([] { Account bad(Money(-1)); }));
    const Account empty(Money(0));
    assert(empty.balance().kopecks() == 0);    // наблюдатель работает у const

    std::cout << "Практика 0: все проверки пройдены!\n";
}
