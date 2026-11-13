// ПАРА 1, ПОСЛЕ Account и практики. Операторы для знакомых действий Money.
// Сборка: g++ -std=c++20 -Wall -Wextra demo1_money.cpp -o demo1 && ./demo1
#include <iostream>
#include "money.hpp"

int main() {
    Money price(19990);
    Money delivery(35000);

    // Обычный метод уже знаком: меняется объект named_total.
    Money named_total = price;
    named_total.add(delivery);
    std::cout << "Через add: " << named_total.str() << "\n";

    // += — привычная запись того же изменения.
    Money assigned_total = price;
    assigned_total += delivery;
    std::cout << "Через +=:  " << assigned_total.str() << "\n";

    // + создаёт результат; исходные price и delivery не меняются.
    Money total = price + delivery;
    std::cout << "Через +:   " << total.str() << "\n";
    std::cout << "Цена осталась: " << price.str() << "\n";
    std::cout << "Доставка осталась: " << delivery.str() << "\n";

    // Дополнительно: operator<< позволяет вывести Money без явного вызова str().
    std::cout << "Итого: " << total << "\n";

    // Money bad = 100;    // ОШИБКА: explicit запрещает неявное преобразование.
    // auto bad = 100 + total;  // ОШИБКА: int не превращается в Money неявно.
    Money plus_one = Money(100) + total;  // Оба операнда имеют тип Money.
    std::cout << "С добавлением рубля: " << plus_one.str() << "\n";
    // const Money c(100); c += price;  // ОШИБКА: += не является const-методом.
}
