// ============================================================
// БОНУС. Класс Matrix — Правило нуля, operator(), transpose
// ============================================================
// Напишите класс Matrix (выше main):
//   - хранит double в std::vector<double> (плоский массив rows*cols)
//   - Matrix(size_t rows, size_t cols): заполняет нулями; если rows или cols == 0 — std::invalid_argument
//   - rows(), cols()  (const)
//   - double& operator()(size_t r, size_t c)  и  const-версия для чтения
//   - Matrix transpose() const — возвращает НОВУЮ транспонированную матрицу
//   - Копирование и перемещение: НЕ пишите руками! (Правило нуля — vector всё сделает сам)
//
// Сборка:  g++ -std=c++20 -Wall -Wextra task4_bonus_matrix.cpp -o task4 && ./task4
// ============================================================
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

// TODO: class Matrix { ... };


// ----------------------- Проверки (не менять) -----------------------
int main() {
    static_assert(std::is_copy_constructible_v<Matrix> && std::is_nothrow_move_constructible_v<Matrix>);

    Matrix m(2, 3);
    assert(m.rows() == 2 && m.cols() == 3);
    assert(m(1, 2) == 0.0);
    int k = 1;
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j) m(i, j) = k++;     // [[1 2 3],[4 5 6]]

    Matrix t = m.transpose();
    assert(t.rows() == 3 && t.cols() == 2);
    assert(t(0, 1) == 4 && t(2, 0) == 3 && t(2, 1) == 6);

    Matrix c = m;                                          // копия независима
    c(0, 0) = 99;
    assert(m(0, 0) == 1 && c(0, 0) == 99);

    Matrix moved = std::move(c);                           // перемещение
    assert(moved.rows() == 2 && moved(0, 0) == 99);

    const Matrix& cm = m;                                  // чтение через const
    assert(cm(1, 0) == 4);

    bool thrown = false;
    try { Matrix bad(0, 5); } catch (const std::invalid_argument&) { thrown = true; }
    assert(thrown);

    std::cout << "Бонус: все проверки пройдены!\n";
}
