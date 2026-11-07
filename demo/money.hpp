#pragma once
// ============================================================
// Money — деньги в копейках (целое число), а не в double.
// Строим по шагам прямо на показе: ШАГ 1 -> 2 -> 3
// ============================================================
#include <iostream>
#include <string>

class Money {
public:
    // ШАГ 1. Конструктор + explicit:
    //   Money m = 100;   // ОШИБКА — 100 чего? рублей? копеек?
    //   Money m(100);    // OK — намерение явное
    explicit Money(long long kopecks = 0) : kopecks_(kopecks) {}

    static Money rub(long long r, int kop = 0) { return Money(r * 100 + kop); }

    // ШАГ 2. const-метод: «только читаю, объект не меняю»
    long long kopecks() const { return kopecks_; }

    std::string str() const {
        long long k = kopecks_ < 0 ? -kopecks_ : kopecks_;
        return std::string(kopecks_ < 0 ? "-" : "") + std::to_string(k / 100) + "." +
               (k % 100 < 10 ? "0" : "") + std::to_string(k % 100) + " руб.";
    }

    // ШАГ 3. Операторы. += меняет левый операнд -> член класса
    Money& operator+=(Money o) { kopecks_ += o.kopecks_; return *this; }
    Money& operator-=(Money o) { kopecks_ -= o.kopecks_; return *this; }

private:
    long long kopecks_;   // инвариант: всегда целое число копеек
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
