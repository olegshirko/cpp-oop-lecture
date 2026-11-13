#pragma once
// FileLogger — учебная RAII-обёртка: объект владеет одним открытым файлом.
// Локальный объект закрывает файл при конце блока, return и раскрутке стека.
// Аварийное завершение процесса не даёт такой гарантии.
#include <cstdio>
#include <stdexcept>
#include <string>

class FileLogger {
public:
    explicit FileLogger(const std::string& path)
        : f_(std::fopen(path.c_str(), "a")) {
        if (!f_) throw std::runtime_error("не удалось открыть " + path);
    }

    ~FileLogger() {
        std::fclose(f_);
    }

    // Копия указателя не создаёт новый файл: два владельца закрыли бы один FILE*.
    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    void write(const std::string& msg) {
        if (std::fprintf(f_, "%s\n", msg.c_str()) < 0)
            throw std::runtime_error("не удалось записать сообщение");
    }

private:
    // В успешно созданном объекте f_ не равен nullptr.
    // Перемещение в этом занятии не реализуем и не используем.
    std::FILE* f_;
};

// В учебном деструкторе результат fclose не обрабатывается: деструктор
// не сообщает об ошибке записи на диск. Для важных данных нужен отдельный
// проверяемый шаг сохранения; закрытие файла само по себе не гарантирует успех записи.
