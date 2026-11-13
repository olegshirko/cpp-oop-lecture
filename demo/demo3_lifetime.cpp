// ПАРА 2. Время жизни объектов до знакомства с файловым ресурсом.
// Сборка: g++ -std=c++20 -Wall -Wextra demo3_lifetime.cpp -o lifetime && ./lifetime
#include <iostream>
#include <stdexcept>
#include <string>

// Учебный наблюдатель печатает начало и конец жизни локального объекта.
// Он не управляет файлом и не выполняет откат каких-либо действий.
class ScopeTrace {
public:
    explicit ScopeTrace(const std::string& name) : name_(name) {
        std::cout << "ENTER " << name_ << "\n";
    }
    ~ScopeTrace() {
        std::cout << "LEAVE " << name_ << "\n";
    }
private:
    std::string name_;
};

void early_return() {
    ScopeTrace local("return");
    std::cout << "return из функции\n";
    return;
}

void with_exception() {
    ScopeTrace local("throw");
    throw std::runtime_error("учебная ошибка");
}

int main() {
    std::cout << "1) Вложенные блоки:\n";
    {
        ScopeTrace a("A");
        {
            ScopeTrace b("B");
            ScopeTrace c("C");
        }  // Сначала C, затем B.
        std::cout << "A ещё существует\n";
    }  // Теперь A.

    std::cout << "\n2) Ранний return:\n";
    early_return();
    std::cout << "вернулись в main\n";

    std::cout << "\n3) Исключение с обработчиком:\n";
    try {
        with_exception();
    } catch (const std::runtime_error& e) {
        std::cout << "catch: " << e.what() << "\n";
    }
}

// Здесь стандартный cout работает с обычной маской исключений потока.
// В реальном деструкторе нельзя допускать выход исключения наружу.
