#pragma once
// ============================================================
// Money — деньги в копейках (целое число), а не в double.
// Конструктор уже знаком. Продолжаем: const -> обычные методы -> операторы.
// Учебные суммы небольшие: аргументы и результаты арифметики должны
// помещаться в long long. Проверка переполнения здесь не реализована.
// ============================================================
#include <iostream>
#include <string>

class Money {
public:
    // Повтор прошлого занятия: конструктор + explicit.
    //   Money m = 100;   // ОШИБКА — 100 чего? рублей? копеек?
    //   Money m(100);    // OK — намерение явное
    explicit Money(long long kopecks = 0) : kopecks_(kopecks) {}

    static Money rub(long long r, int kop = 0) { return Money(r * 100 + kop); }

    // Наблюдатель: можно вызвать и у const Money.
    long long kopecks() const { return kopecks_; }

    std::string str() const {
        // Модуль в беззнаковом типе: даже минимальное long long печатается корректно.
        unsigned long long k = kopecks_ < 0
            ? 0ULL - static_cast<unsigned long long>(kopecks_)
            : static_cast<unsigned long long>(kopecks_);
        return std::string(kopecks_ < 0 ? "-" : "") + std::to_string(k / 100) + "." +
               (k % 100 < 10 ? "0" : "") + std::to_string(k % 100) + " руб.";
    }

    // Обычные методы изменения. Разбираем их до перегрузки операторов.
    void add(Money amount) { kopecks_ += amount.kopecks_; }
    void subtract(Money amount) { kopecks_ -= amount.kopecks_; }

    // Позже: операторы дают знакомую запись для тех же действий.
    Money& operator+=(Money o) { add(o); return *this; }
    Money& operator-=(Money o) { subtract(o); return *this; }

private:
    long long kopecks_;   // Сумма в копейках; отрицательные значения допустимы.
};

// Бинарные операторы — свободные функции (оба операнда равноправны)
inline Money operator+(Money a, Money b) { return a += b; }
inline Money operator-(Money a, Money b) { return a -= b; }
inline bool operator==(Money a, Money b) { return a.kopecks() == b.kopecks(); }
inline bool operator!=(Money a, Money b) { return !(a == b); }
inline bool operator<(Money a, Money b)  { return a.kopecks() <  b.kopecks(); }
inline bool operator>(Money a, Money b)  { return b < a; }
inline bool operator<=(Money a, Money b) { return !(b < a); }
inline bool operator>=(Money a, Money b) { return !(a < b); }

inline std::ostream& operator<<(std::ostream& os, Money m) { return os << m.str(); }
