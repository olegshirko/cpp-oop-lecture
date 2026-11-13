// СЛЕДУЮЩЕЕ ЗАНЯТИЕ. Не входит в обязательную программу этих двух пар.
// ПОКАЗ 5. Всё вместе: Order (композиция) = Money + Account + PaymentMethod + FileLogger
// Сборка: g++ -std=c++20 -Wall -Wextra demo5_order.cpp -o demo5 && ./demo5
#include <iostream>
#include <memory>
#include <string>
#include "payments.hpp"
#include "file_logger.hpp"

// Order НЕ наследуется от логгера и способа оплаты, а ИМЕЕТ их (has-a).
// Члены сами освобождают ресурсы. Копирование Order недоступно из-за его членов.
// FileLogger в этой версии не перемещается, поэтому Order также не перемещается.
class Order {
public:
    Order(std::string id, std::unique_ptr<PaymentMethod> payment, const std::string& logfile)
        : id_(std::move(id)), payment_(std::move(payment)), log_(logfile) {}

    void add_item(const std::string& name, Money price) {
        total_ += price;
        log_.write(id_ + ": + " + name + " (" + price.str() + ")");
    }

    bool checkout() {
        log_.write(id_ + ": оплата " + payment_->name() + ", сумма " + total_.str());
        bool ok = payment_->pay(total_);
        log_.write(id_ + (ok ? ": ОПЛАЧЕН" : ": ОТКАЗ"));
        return ok;
    }

private:
    std::string id_;
    std::unique_ptr<PaymentMethod> payment_;   // полиморфный объект -> владеет unique_ptr
    FileLogger log_;                           // конкретный тип -> хранится по значению
    Money total_{};
    // Члены уничтожаются в обратном порядке объявления:
    // total_, log_, payment_, id_.
};

int main() {
    Account buyer(Money::rub(3000));

    {
        Order o1("order-1", std::make_unique<CardPayment>(buyer), "orders.log");
        o1.add_item("Клавиатура", Money::rub(2490));
        bool ok = o1.checkout();
        std::cout << "order-1: " << (ok ? "оплачен" : "отказ") << "\n";
    }
    std::cout << "Остаток на счёте: " << buyer.balance() << "\n\n";

    {
        Order o2("order-2", std::make_unique<CardPayment>(buyer), "orders.log");
        o2.add_item("Мышь", Money::rub(1200));
        bool ok = o2.checkout();
        std::cout << "order-2: " << (ok ? "оплачен" : "отказ — мало денег") << "\n\n";
    }
    {
        // Тот же Order, другой способ оплаты — Order не менялся!
        Order o3("order-3", std::make_unique<SbpPayment>("+7-900-000-00-00"), "orders.log");
        o3.add_item("Мышь", Money::rub(1200));
        bool ok = o3.checkout();
        std::cout << "order-3: " << (ok ? "оплачен" : "отказ") << "\n";
    }
    return 0;
}
