#pragma once
// ============================================================
// FileLogger — RAII: ресурс (файл) привязан к времени жизни объекта
//   конструктор открывает, деструктор закрывает — всегда.
// ============================================================
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

class FileLogger {
public:
    explicit FileLogger(const std::string& path) : f_(std::fopen(path.c_str(), "a")) {
        if (!f_) throw std::runtime_error("не удалось открыть " + path);
        std::cout << "  [Logger] файл открыт\n";
    }

    ~FileLogger() {
        if (f_) {
            std::fclose(f_);
            std::cout << "  [Logger] файл закрыт\n";
        }
    }

    // Копировать нельзя: два объекта закрыли бы ОДИН файл дважды (Правило трёх)
    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    // Перемещать можно: «передаём владение файлом» (Правило пяти)
    FileLogger(FileLogger&& o) noexcept : f_(o.f_) { o.f_ = nullptr; }
    FileLogger& operator=(FileLogger&& o) noexcept {
        if (this != &o) {
            if (f_) std::fclose(f_);
            f_ = o.f_;
            o.f_ = nullptr;
        }
        return *this;
    }

    void write(const std::string& msg) { if (f_) std::fprintf(f_, "%s\n", msg.c_str()); }

private:
    std::FILE* f_;
};
