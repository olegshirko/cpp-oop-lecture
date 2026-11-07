// ПОКАЗ 4б. БАГ: нет виртуального деструктора.
// Сборка: g++ -std=c++20 -Wall -Wextra demo4b_virtual_dtor_bug.cpp -o demo4b && ./demo4b
// Обратите внимание: компилятор ПРЕДУПРЕЖДАЕТ (-Wdelete-non-virtual-dtor).
// Поведение — неопределённое (UB); на практике деструктор производного класса не вызывается.
#include <iostream>

struct Base {
    virtual void pay() {}                              // есть виртуальная функция...
    ~Base() { std::cout << "  ~Base\n"; }             // ...а деструктор НЕ виртуальный!
};
struct Derived : Base {
    ~Derived() { std::cout << "  ~Derived: закрываем соединение с банком\n"; }
};

int main() {
    std::cout << "Удаляем Derived через Base*:\n";
    Base* p = new Derived();
    delete p;       // UB! «соединение с банком» не закрылось — утечка ресурса
    std::cout << "\nИсправление: добавьте virtual перед ~Base() и запустите снова.\n";
    return 0;
}
