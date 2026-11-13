// ПАРА 1. Продолжение уже написанного Money: повтор и const.
// Сборка: g++ -std=c++20 -Wall -Wextra demo0_review.cpp -o review && ./review
// Операторы Money пока не используем: обращаемся к обычным методам.
#include <iostream>
#include "money.hpp"

int main() {
    Money first(10000);
    Money second(10000);
    first.add(Money(500));
    std::cout << "first:  " << first.kopecks() << " коп.\n";
    std::cout << "second: " << second.kopecks() << " коп.\n";

    const Money price(19990);
    std::cout << "Цена: " << price.kopecks() << " коп.\n";
    std::cout << "Цена для человека: " << price.str() << "\n";

    // Раскомментируйте по одной строке и объясните ошибку компилятора.
    // Money bad = 100;       // explicit запрещает неявное преобразование числа.
    // first.kopecks_ = 0;    // Поле существует, но доступ к нему закрыт.
    // price.add(Money(100)); // add меняет объект и не является const-методом.
}
