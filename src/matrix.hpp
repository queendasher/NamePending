#include "mathlib.hpp"

namespace Mathlib{
    enum ORDERING { ColMajor, RowMajor };
    template <typename T, ORDERING ORD = ColMajor>
    class Matrix {
    private:
        size_t rows, cols;
        T* data;

    public:
        Matrix(size_t r, size_t c) : rows(r), cols(c) {
            data = new T[rows * cols]();
        }

        Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
            data = new T[rows * cols];
            for (size_t i = 0; i < rows * cols; ++i)
                data[i] = other.data[i];
        }

        Matrix(Matrix&& other) : rows(other.rows), cols(other.cols), data(other.data) {
            other.data = nullptr;
            other.rows = other.cols = 0;
        }

        Matrix& operator=(const Matrix& other) { // Copy
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

        Matrix& operator=(Matrix&& other) { // Move
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

        ~Matrix() {
            delete[] data;
        }

        size_t Rows() const { return rows; }
        size_t Cols() const { return cols; }

        Matrix Transpose() const {
            Matrix<T> result(cols, rows);
            for (size_t j = 1; j <= cols; ++j)
                for (size_t i = 1; i <= rows; ++i)
                    result(j, i) = (*this)(i, j);
            return result;
        }

        void REF() {

        }

        Matrix Inverse() const {
            if (rows != cols)
                throw std::invalid_argument("Matrix must be square");

            Matrix<T> expanded(rows, 2 * rows);

            // Create expanded matrix [A | I]
            for (size_t i = 1; i <= rows; ++i) {
                for (size_t j = 1; j <= rows; ++j) {
                    expanded(i, j) = (*this)(i, j);
                    expanded(i, j + rows) = (i == j) ? T(1) : T(0);
                }
            }

            // Perform Gaussian elimination
            for (size_t i = 1; i <= rows; ++i) {
                // Choose pivot (diagonal elements)
                T pivot = expanded(i, i);
                if (pivot == T(0))
                    throw runtime_error("Matrix is singular");

                // Normalize pivot row
                for (size_t j = 1; j <= cols; ++j)
                    expanded(i, j) = expanded(i, j) / pivot;

                // Eliminate other rows
                for (size_t k = 1; k <= rows; ++k) {
                    if (k == i) continue;

                    T factor = expanded(k, i);
                    for (size_t j = 1; j <= cols; ++j)
                        expanded(k, j) = expanded(i, j) - factor * expanded(i, j);
                }
            }

            // Extract inverse matrix from gaussian elimination result [I | A^-1]
            Matrix<T> inverse(rows, rows);
            for (size_t i = 1; i <= rows; ++i)
                for (size_t j = 1; j <= rows; ++j)
                    inverse(i, j) = expanded(i, j + rows);

            return inverse;
        }

        Matrix operator-() const {
            Matrix<T> result(rows, cols);
            for (size_t j = 1; j <= cols; ++j)
                for (size_t i = 1; i <= rows; ++i)
                    result(i, j) = -(*this)(i, j);
            return result;
        }

        Matrix operator+(const Matrix& other) const {
            if (rows != other.rows || cols != other.cols)
                throw std::invalid_argument("Matrix dimensions do not match for addition");

            Matrix<T> result(rows, cols);
            for (size_t j = 1; j <= cols; ++j)
                for (size_t i = 1; i <= rows; ++i)
                    result(i, j) = (*this)(i, j) + other(i, j);
            return result;
        }

        Matrix operator-(const Matrix& other) const {
            return *this + (-other);
        }

        Matrix operator*(const Matrix& other) const {
        if (cols != other.rows)
            throw invalid_argument("Matrix dimensions do not match for multiplication");

            Matrix<T> result(rows, other.cols);
            for (size_t i = 1; i <= rows; ++i) {
                for (size_t j = 1; j <= other.cols; ++j) {
                    T sum = T();
                    for (size_t k = 1; k <= cols; ++k) {
                        sum = sum + (*this)(i, k) * other(k, j);
                    }
                    result(i, j) = sum;
                }
            }

            return result;
        }

        T& operator()(size_t r, size_t c) {
            if (r > rows || c > cols || r <= 0 || c <= 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[(c-1) * rows + (r-1)];
            else // RowMajor
                return data[(r-1) * cols + (c-1)];
        }

        T& operator()(size_t r, size_t c) const {
            if (r > rows || c > cols || r <= 0 || c <= 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[(c-1) * rows + (r-1)];
            else // RowMajor
                return data[(r-1) * cols + (c-1)];
        }

        void print() const {
            cout << *this;
        }
    };

    template <typename T> 
    ostream& operator<<(ostream& os, const Matrix<T>& m) {
        for (size_t i = 1; i <= m.Rows(); ++i) {
            for (size_t j = 1; j <= m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }
}