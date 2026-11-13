// ПАРА 2. RAII: конец блока, ранний return, исключение и запрет копирования.
// Сборка: g++ -std=c++20 -Wall -Wextra demo3_raii.cpp -o demo3 && ./demo3
// Создаёт/дополняет demo.log в текущей папке. Ошибки ввода-вывода не подавляются.
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include "file_logger.hpp"

void early_return() {
    FileLogger log("demo.log");
    log.write("return: файл будет закрыт");
    return;  // ~FileLogger вызывается до возвращения в main.
}

void risky() {
    FileLogger log("demo.log");
    log.write("throw: файл будет закрыт");
    throw std::runtime_error("учебная ошибка");
}

// После закрытия файла читаем записанную строку.
// Наличие строки проверяет результат записи; порядок вызовов деструкторов
// отдельно наблюдаем в demo3_lifetime.cpp.
std::string last_line(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("не удалось прочитать " + path);
    std::string line;
    std::string last;
    while (std::getline(in, line)) last = line;
    if (in.bad()) throw std::runtime_error("ошибка чтения " + path);
    return last;
}

int main() {
    std::cout << "1) Конец блока:\n";
    {
        FileLogger log("demo.log");
        log.write("block: файл будет закрыт");
    }  // Здесь завершилась жизнь log и вызван ~FileLogger.
    assert(last_line("demo.log") == "block: файл будет закрыт");
    std::cout << "  после блока: " << last_line("demo.log") << "\n";

    std::cout << "\n2) Ранний return:\n";
    early_return();
    assert(last_line("demo.log") == "return: файл будет закрыт");
    std::cout << "  после return: " << last_line("demo.log") << "\n";

    std::cout << "\n3) Исключение с обработчиком:\n";
    try {
        risky();
    } catch (const std::runtime_error& e) {
        std::cout << "  catch: " << e.what() << "\n";
    }
    assert(last_line("demo.log") == "throw: файл будет закрыт");
    std::cout << "  после catch: " << last_line("demo.log") << "\n";

    std::cout << "\n4) Один открытый файл — один владелец:\n";
    static_assert(!std::is_copy_constructible_v<FileLogger>);
    static_assert(!std::is_copy_assignable_v<FileLogger>);
    {
        FileLogger a("demo.log");
        // FileLogger b = a;  // ОШИБКА: копирующий конструктор удалён.
        a.write("копирование запрещено");
    }
    std::cout << "  запрещённую копию можно проверить, раскомментировав строку\n";

    std::cout << "\n5) Готовый владелец ресурса: std::ofstream:\n";
    {
        std::ofstream out("demo.log", std::ios::app);
        if (!out) throw std::runtime_error("не удалось открыть demo.log");
        out << "ofstream: файл закрывает сам объект потока\n";
        if (!out) throw std::runtime_error("ошибка записи в demo.log");
    }  // У std::ofstream уже есть деструктор, закрывающий файл.
    assert(last_line("demo.log") == "ofstream: файл закрывает сам объект потока");
    std::cout << "  собственный класс для закрытия файла здесь не понадобился\n";
}
