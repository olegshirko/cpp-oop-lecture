// ============================================================
// ЗАДАНИЕ 2 (15–20 мин). RAII: защитник транзакции
// ============================================================
// При переводе денег между счетами нужно гарантировать: если что-то пошло не так
// (ранний return, исключение) — транзакция ОТКАТЫВАЕТСЯ. Помнить про это руками —
// значит забыть. Поручим это деструктору.
//
// Напишите класс Transaction (выше main):
//
//   Transaction(std::string name)  печатает   "  BEGIN <name>"
//   void commit()                  печатает   "  COMMIT <name>"  и запоминает, что фиксация была
//   ~Transaction()                 если commit() НЕ вызывали — печатает "  ROLLBACK <name>"
//   Копировать транзакцию нельзя   (= delete) — подумайте, почему.
//
// Сборка:  g++ -std=c++20 -Wall -Wextra task2_transaction.cpp -o task2 && ./task2
//
// Вопросы для обсуждения после выполнения:
//   1. Почему ROLLBACK печатается даже при исключении?
//   2. Что будет, если разрешить копирование? (сколько раз выполнится ROLLBACK?)
//   3. Нужно ли здесь писать конструктор перемещения? (Правило нуля / пяти)
// ============================================================
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "../demo/account.hpp"

// TODO: class Transaction { ... };


// ----------------------- Проверки (не менять) -----------------------
static_assert(!std::is_copy_constructible_v<Transaction>, "Transaction нельзя копировать");
static_assert(!std::is_copy_assignable_v<Transaction>,    "Transaction нельзя копировать");

// Перевод денег: Transaction гарантирует откат при любом «неудачном» выходе
bool transfer(Account& from, Account& to, Money m) {
    Transaction tx("transfer");
    if (!from.withdraw(m)) return false;      // ранний выход -> ROLLBACK
    to.deposit(m);
    tx.commit();
    return true;
}

template <class F>
std::string capture(F f) {                    // перехват того, что печатает f()
    std::ostringstream buf;
    auto* old = std::cout.rdbuf(buf.rdbuf());
    try { f(); } catch (...) {}
    std::cout.rdbuf(old);
    return buf.str();
}

int main() {
    Account a(Money::rub(1000)), b(Money::rub(0));

    std::string out1 = capture([&] { assert(transfer(a, b, Money::rub(300))); });
    assert(out1 == "  BEGIN transfer\n  COMMIT transfer\n");
    assert(a.balance() == Money::rub(700) && b.balance() == Money::rub(300));

    std::string out2 = capture([&] { assert(!transfer(a, b, Money::rub(5000))); });
    assert(out2 == "  BEGIN transfer\n  ROLLBACK transfer\n");

    std::string out3 = capture([&] {          // исключение посреди операции
        Transaction tx("boom");
        throw std::runtime_error("сбой сети");
    });
    assert(out3 == "  BEGIN boom\n  ROLLBACK boom\n");

    std::cout << "Задание 2: все проверки пройдены!\n";
}
