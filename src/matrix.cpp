#include "mathlib.hpp"

namespace Mathlib {
    template <typename T>
    Matrix<T>::Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data = new T[rows * cols]();
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new T[rows * cols];
        for (size_t i = 0; i < rows * cols; ++i)
            data[i] = other.data[i];
    }

    template <typename T>
    Matrix<T>::Matrix(Matrix&& other) : rows(other.rows), cols(other.cols), data(other.data) {
        other.data = nullptr;
        other.rows = other.cols = 0;
    }

    template <typename T>
    Matrix<T>::~Matrix() {
        delete[] data;
    }

    template <typename T> size_t Matrix<T>::Rows() const { return rows; }
    template <typename T> size_t Matrix<T>::Cols() const { return cols; }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(const Matrix& other) { // Copy
        if (this != &other) {
            delete[] data;
            rows = other.rows;
            cols = other.cols;
            data = new T[rows * cols];
            for (size_t i = 0; i < rows * cols; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix&& other) { // Move
        if (this != &other) {
            delete[] data;
            rows = other.rows;
            cols = other.cols;
            data = other.data;
            other.data = nullptr;
            other.rows = other.cols = 0;
        }
        return *this;
    }

    template <typename T>
    T& Matrix<T>::operator()(size_t r, size_t c) {
        if (r > rows || c > cols)
            throw std::out_of_range("Matrix index out of range");
        return data[(c-1) * rows + (r-1)];
    }

    template <typename T>
    const T& Matrix<T>::operator()(size_t r, size_t c) const {
        if (r > rows || c > cols)
            throw std::out_of_range("Matrix index out of range");
        return data[(c-1) * rows + (r-1)];
    }

    template <typename T>
    ostream& operator<<(ostream& os, const Matrix<T>& m) {
        for (size_t i = 1; i <= m.Rows(); ++i) {
            for (size_t j = 1; j <= m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }

    template <typename T>
    void Matrix<T>::print() const {
        cout << *this;
    }
}
