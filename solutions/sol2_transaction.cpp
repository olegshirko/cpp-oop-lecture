// Решение дополнительного задания. Сначала выполните tasks/task2_transaction.cpp.
// ДОПОЛНЕНИЕ. Transaction: модель сообщений о завершении операции
// Не входит в обязательный план двух пар; выполните после task2_lifetime.cpp.
// Это учебная имитация: Transaction печатает BEGIN/COMMIT/ROLLBACK,
// но НЕ сохраняет старые балансы и НЕ откатывает уже сделанные изменения.
// Настоящий откат требует отдельной реализации восстановления состояния.
//
// Напишите класс Transaction:
//   explicit Transaction(std::string name) печатает "  BEGIN <name>\n";
//   commit() запоминает завершение и печатает "  COMMIT <name>\n";
//   деструктор печатает "  ROLLBACK <name>\n", если commit не вызван;
//   копирование запрещено (= delete); перемещение не требуется.
// Поля: std::string name_; bool committed_ = false;
// Почему сообщение ROLLBACK само по себе не возвращает деньги?
//
// Сборка из корня репозитория:
//   g++ -std=c++20 -Wall -Wextra solutions/sol2_transaction.cpp -o transaction && ./transaction
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "../demo/account.hpp"

#ifdef NDEBUG
#error "Проверки требуют assert: собирайте без -DNDEBUG"
#endif

class Transaction {
public:
    explicit Transaction(std::string name) : name_(name) {
        std::cout << "  BEGIN " << name_ << "\n";
    }
    ~Transaction() {
        if (!committed_) std::cout << "  ROLLBACK " << name_ << "\n";
    }
    void commit() {
        committed_ = true;
        std::cout << "  COMMIT " << name_ << "\n";
    }
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

private:
    std::string name_;
    bool committed_ = false;
};

// ----------------------- Проверки (не менять) -----------------------
static_assert(!std::is_copy_constructible_v<Transaction>, "копирование запрещено");
static_assert(!std::is_copy_assignable_v<Transaction>, "присваивание запрещено");

// Только журналирует этапы перевода: гарантии отката здесь нет.
bool logged_transfer(Account& from, Account& to, Money m) {
    Transaction tx("transfer");
    if (!from.withdraw(m)) return false;
    to.deposit(m);
    tx.commit();
    return true;
}

// Служебная функция проверок: сохраняет текст, напечатанный в cout.
// Её реализация не входит в задание.
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
    Account a(Money(100000)), b(Money(0));
    const std::string success = capture([&] {
        const bool ok = logged_transfer(a, b, Money(30000));
        assert(ok);
    });
    assert(success == "  BEGIN transfer\n  COMMIT transfer\n");
    assert(a.balance().kopecks() == 70000 && b.balance().kopecks() == 30000);

    const std::string refused = capture([&] {
        const bool ok = logged_transfer(a, b, Money(500000));
        assert(!ok);
    });
    assert(refused == "  BEGIN transfer\n  ROLLBACK transfer\n");
    assert(a.balance().kopecks() == 70000 && b.balance().kopecks() == 30000);

    Account c(Money(100000));
    const std::string failed = capture([&] {
        try {
            Transaction tx("boom");
            const bool ok = c.withdraw(Money(30000));
            assert(ok);
            throw std::runtime_error("сбой после списания");
        } catch (const std::runtime_error&) {
            // Деструктор уже напечатал ROLLBACK, но баланс не восстановлен.
        }
    });
    assert(failed == "  BEGIN boom\n  ROLLBACK boom\n");
    assert(c.balance().kopecks() == 70000);  // демонстрирует ограничение модели

    std::cout << "Дополнение Transaction: сообщения проверены, отката денег нет.\n";
}
