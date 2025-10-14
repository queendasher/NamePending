#include "mathlib.hpp"
#include "vector.hpp"
#include "expression.hpp"

namespace Mathlib{
    enum ORDERING { ColMajor, RowMajor };

    template <typename T, ORDERING ORD = ColMajor, typename TDIST = integral_constant<size_t, 1>>
    class MatrixView: public MatExpr<MatrixView<T, ORD, TDIST>> {
    protected:
        size_t rows, cols;
        T* data;
        TDIST dist;

    public:
        MatrixView() = default; // initializes members to zero / nullptr
        MatrixView(const MatrixView&) = default; // shallow copy

        template<typename TDIST2>
        MatrixView(const MatrixView<T, ORD, TDIST2>& other) :  // allow implicit conversion between different dist types
            rows(other.Rows()), 
            cols(other.Cols()), 
            data(other.Data()), 
            dist(other.Dist()) { }
        MatrixView(size_t r, size_t c, T* d) : rows(r), cols(c), data(d) { }
        MatrixView(size_t r, size_t c, TDIST _dist, T* d) : rows(r), cols(c), data(d), dist(_dist) { }

        template<typename T2>
        MatrixView& operator=(const MatExpr<T2>& other) {
            for (size_t j = 0; j < cols; ++j)
                for (size_t i = 0; i < rows; ++i)
                    (*this)(i, j) = other(i, j);
            return *this;
        }

        T* Data() const { return data; }
        size_t Rows() const { return rows; }
        size_t Cols() const { return cols; }
        auto Dist() const { return dist; }

        const T& operator()(size_t r, size_t c) const {
            if (r >= rows || c >= cols || r < 0 || c < 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[c * rows + r];
            else // RowMajor
                return data[r * cols + c];
        }

        T& operator()(size_t r, size_t c) {
            if (r >= rows || c >= cols || r < 0 || c < 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[c * rows + r];
            else // RowMajor
                return data[r * cols + c];
        }

        auto Row(size_t r) const {
            if (r >= rows || r < 0)
                throw out_of_range("Matrix row index out of range");
            if (ORD == ColMajor)
                return VectorView<T, size_t>(cols, rows, data + r);
            else // RowMajor
                return VectorView<T, size_t>(cols, 1, data + r * cols);
        }

        auto Col(size_t c) const {
            if (c >= cols || c < 0)
                throw out_of_range("Matrix column index out of range");
            if (ORD == ColMajor)
                return VectorView<T, size_t>(rows, 1, data + c * rows);
            else // RowMajor
                return VectorView<T, size_t>(rows, cols, data + c);
        }

        auto Transpose() const {
            if constexpr (ORD == ColMajor) // need constexpr so auto knows return type at compile time
                return MatrixView<T, RowMajor>(cols, rows, data);
            else // RowMajor
                return MatrixView<T, ColMajor>(cols, rows, data);
        }
    };




    template <typename T, ORDERING ORD = ColMajor>
    class Matrix : public MatrixView<T, ORD> {
        typedef MatrixView<T, ORD> BASE;
        using BASE::rows;
        using BASE::cols;
        using BASE::data;

    public:
        Matrix(size_t r, size_t c) : 
            MatrixView<T, ORD>(r, c, new T[r * c]) { }

        Matrix(const Matrix& other) : Matrix(other.Rows(), other.Cols()) {
            *this = other;
        }

        Matrix(Matrix&& other) : MatrixView<T, ORD>(0, 0, nullptr) {
            swap(rows, other.rows);
            swap(cols, other.cols);
            swap(data, other.data);
        }

        template <typename T2>
        Matrix(const MatExpr<T2>& other) : Matrix(other.Rows(), other.Cols()) {
            *this = other;
        }

        using BASE::operator=;
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
            if (r >= rows || c >= cols || r < 0 || c < 0)
                throw out_of_range("Matrix index out of range");
            if (ORD == ColMajor)
                return data[c * rows + r];
            else // RowMajor
                return data[r * cols + c];
        }

        T& operator()(size_t r, size_t c) const {
            if (r >= rows || c >= cols || r < 0 || c < 0)
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

    template <typename T, ORDERING ORD> 
    ostream& operator<<(ostream& os, const Matrix<T, ORD>& m) {
        for (size_t i = 0; i < m.Rows(); ++i) {
            for (size_t j = 0; j < m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }
    
    template <typename T, ORDERING ORD, typename TDIST>
    ostream& operator<<(ostream& os, const MatrixView<T, ORD, TDIST>& m) {
        for (size_t i = 0; i < m.Rows(); ++i) {
            for (size_t j = 0; j < m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }
}