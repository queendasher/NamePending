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
            for (size_t j = 0; j < cols; ++j)
                for (size_t i = 0; i < rows; ++i)
                    result(j, i) = (*this)(i, j);
            return result;
        }

        void REF() {

        }

        Matrix Inverse() const {
            if (rows != cols)
                throw std::runtime_error("Matrix must be square to compute its inverse");

            // Create an augmented matrix [A | I]
            Matrix<T> augmented(rows, 2 * cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    augmented(i, j) = (*this)(i, j);
                    augmented(i, j + cols) = (i == j) ? T(1) : T(0);
                }
            }

            // Perform Gaussian elimination
            for (size_t i = 0; i < rows; ++i) {
                // Find pivot (diagonal element)
                T pivot = augmented(i, i);
                if (pivot == T(0))
                    throw std::runtime_error("Matrix is singular and cannot be inverted");

                // Normalize pivot row
                for (size_t j = 0; j < 2 * cols; ++j)
                    augmented(i, j) = augmented(i, j) / pivot;

                // Eliminate other rows
                for (size_t k = 0; k < rows; ++k) {
                    if (k != i) {
                        T factor = augmented(k, i);
                        for (size_t j = 0; j < 2 * cols; ++j)
                            augmented(k, j) = augmented(k, j) - factor * augmented(i, j);
                    }
                }
            }

            // Extract the inverse matrix from the augmented matrix
            Matrix<T> inverse(rows, cols);
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    inverse(i, j) = augmented(i, j + cols);

            return inverse;
        }

        Matrix operator-() const {
            Matrix<T> result(rows, cols);
            for (size_t j = 0; j < cols; ++j)
                for (size_t i = 0; i < rows; ++i)
                    result(i, j) = -(*this)(i, j);
            return result;
        }

        Matrix operator+(const Matrix& other) const {
            if (rows != other.rows || cols != other.cols)
                throw std::invalid_argument("Matrix dimensions do not match for addition");

            Matrix<T> result(rows, cols);
            for (size_t j = 0; j < cols; ++j)
                for (size_t i = 0; i < rows; ++i)
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
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < other.cols; ++j) {
                    T sum = T();
                    for (size_t k = 0; k < cols; ++k) {
                        sum = sum + (*this)(i, k) * other(k, j);
                    }
                    result(i, j) = sum;
                }
            }

            return result;
        }

        T& operator()(size_t r, size_t c) {
            if (r => rows || c => cols || r < 0 || c < 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[c * rows + r];
            else // RowMajor
                return data[r * cols + c];
        }

        T& operator()(size_t r, size_t c) const {
            if (r => rows || c => cols || r < 0 || c < 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[c * rows + r];
            else // RowMajor
                return data[r * cols + c];
        }

        void print() const {
            cout << *this;
        }
    };

    template <typename T> 
    ostream& operator<<(ostream& os, const Matrix<T>& m) {
        for (size_t i = 0; i < m.Rows(); ++i) {
            for (size_t j = 0; j < m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }
}