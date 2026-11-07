// ПОКАЗ 3. RAII, деструктор, Правило трёх/пяти/нуля, перемещение
// Сборка: g++ -std=c++20 -Wall -Wextra demo3_raii.cpp -o demo3 && ./demo3
#include <fstream>
#include <iostream>
#include <type_traits>
#include <utility>
#include "file_logger.hpp"

void risky() {
    FileLogger log("demo.log");
    log.write("начали рискованную операцию");
    throw std::runtime_error("что-то пошло не так");   // return/exception — файл закроется ВСЁ РАВНО
}

// Правило нуля: ничего не пишем — std::ofstream сам всё умеет
class ZeroLogger {
public:
    explicit ZeroLogger(const std::string& path) : out_(path, std::ios::app) {}
    void write(const std::string& msg) { out_ << msg << "\n"; }
private:
    std::ofstream out_;
};

int main() {
    std::cout << "1) Выход из области видимости — деструктор вызывается сам:\n";
    {
        FileLogger log("demo.log");
        log.write("hello");
        std::cout << "  ...конец блока\n";
    }

    std::cout << "\n2) Исключение. Файл закроется до catch:\n";
    try {
        risky();
    } catch (const std::exception& e) {
        std::cout << "  поймали: " << e.what() << "\n";
    }

    std::cout << "\n3) Копировать нельзя (закомментированная строка не скомпилируется):\n";
    FileLogger a("demo.log");
    // FileLogger b = a;               // ОШИБКА: copy deleted — иначе fclose дважды
    FileLogger b = std::move(a);       // передали владение; a — «пустой, но валидный»
    a.write("это сообщение никуда не попадёт, но программа не упадёт");
    b.write("а это — запишется");

    std::cout << "\n4) Правило нуля:\n";
    static_assert(!std::is_copy_constructible_v<ZeroLogger>, "ofstream нельзя копировать");
    static_assert(std::is_move_constructible_v<ZeroLogger>,  "но можно перемещать");
    ZeroLogger z("demo.log");
    z.write("ZeroLogger: ни деструктора, ни move, ни copy — и всё работает");
    std::cout << "  (деструктор не писали — файл закроется сам)\n";

    std::cout << "\n--- конец main ---\n";
    return 0;
}
