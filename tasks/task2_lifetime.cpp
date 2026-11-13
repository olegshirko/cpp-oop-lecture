// ЗАДАНИЕ 2 (15–20 мин). ScopeTrace: время жизни локального объекта
// Напишите класс ScopeTrace:
//   explicit ScopeTrace(std::string name) сохраняет name и печатает ENTER <name>;
//   ~ScopeTrace() печатает LEAVE <name>;
//   каждая запись оканчивается символом '\n', без начальных пробелов;
//   копирующий конструктор и копирующее присваивание запрещены (= delete).
// Одна метка описывает одну область выполнения; копии в этом упражнении не нужны.
// Перемещение не требуется. В деструкторе нет throw.
// Перед запуском предскажите порядок сообщений для трёх сценариев ниже.
// ScopeTrace показывает время жизни; файлом или другим ресурсом он не владеет.
// Для настоящего ресурса смотрите demo/demo3_raii.cpp.
// Сборка из корня репозитория:
//   g++ -std=c++20 -Wall -Wextra tasks/task2_lifetime.cpp -o task2 && ./task2
// Пока класс не написан, файл не компилируется — это ожидается.
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#ifdef NDEBUG
#error "Проверки требуют assert: собирайте без -DNDEBUG"
#endif

// TODO: class ScopeTrace { ... };

// ----------------------- Проверки (не менять) -----------------------
static_assert(!std::is_copy_constructible_v<ScopeTrace>, "копирование запрещено");
static_assert(!std::is_copy_assignable_v<ScopeTrace>, "присваивание запрещено");

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

void early_return() {
    ScopeTrace trace("early");
    std::cout << "RETURN\n";
    return;
}

int main() {
    const std::string nested = capture([] {
        ScopeTrace outer("outer");
        {
            ScopeTrace inner("inner");
        }
    });
    assert(nested == "ENTER outer\nENTER inner\nLEAVE inner\nLEAVE outer\n");

    const std::string early = capture([] { early_return(); });
    assert(early == "ENTER early\nRETURN\nLEAVE early\n");

    const std::string failure = capture([] {
        try {
            ScopeTrace trace("error");
            throw std::runtime_error("сбой");
        } catch (const std::runtime_error&) {
            std::cout << "CATCH\n";
        }
    });
    assert(failure == "ENTER error\nLEAVE error\nCATCH\n");

    std::cout << "Задание 2: время жизни проверено!\n";
}
