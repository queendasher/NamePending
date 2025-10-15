#include "mathlib.hpp"
#include "vector.hpp"
#include "expression.hpp"
#include <type_traits>

namespace Mathlib{
    enum ORDERING { ColMajor, RowMajor };
    template <typename T, ORDERING ORD = ColMajor>
    class MatrixView : public MatExpr<MatrixView<T, ORD>> {
    protected:
        size_t rows{}, cols{};
        T* data{};
        size_t dist{};

        constexpr size_t index(size_t r, size_t c) const {
            return (ORD == ColMajor) ? c * dist + r : r * dist + c;
        }

    public:
        MatrixView() = default;
        MatrixView(const MatrixView&) = default;

        // Natural contiguous dist by default: dist = rows (col-major) or cols (row-major)
        MatrixView(size_t r, size_t c, T* d)
            : rows(r), cols(c), data(d),
            dist((ORD == ColMajor) ? r : c) {}

        // Explicit dist. Must satisfy dist >= rows (col-major) or dist >= cols (row-major).
        MatrixView(size_t r, size_t c, size_t _dist, T* d)
            : rows(r), cols(c), data(d), dist(_dist) {
            if (!(ORD == ColMajor ? dist >= rows : dist >= cols)) throw invalid_argument("Invalid leading dimension");
        }

        // Assignment from any matrix expression
        template<typename E>
        MatrixView& operator=(const MatExpr<E>& other) {
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    data[index(i, j)] = other(i, j);
            return *this;
        }

        // Scalar assignment
        MatrixView& operator=(T scal) {
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    data[index(i, j)] = scal;
            return *this;
        }

        // Accessors
        T*     Data()  const { return data; }
        size_t Rows()  const { return rows; }
        size_t Cols()  const { return cols; }
        size_t Dist()  const { return dist; }

        // Element access
        const T& operator()(size_t r, size_t c) const {
            if (r >= rows || c >= cols) throw out_of_range("Matrix index out of range");
            return data[index(r, c)];
        }
        T& operator()(size_t r, size_t c) {
            if (r >= rows || c >= cols) throw out_of_range("Matrix index out of range");
            return data[index(r, c)];
        }

        auto Row(size_t r) const {
            if (r >= rows) throw out_of_range("Row out of range");
            auto row_view = VectorView<T, size_t>();
            if constexpr (ORD == ColMajor)
                row_view = VectorView<T, size_t>(cols, dist, data + r);
            else 
                row_view = VectorView<T, size_t>(cols, 1, data + r*dist);
            return row_view;
        }

        auto Col(size_t c) const {
            if (c >= cols) throw out_of_range("Col out of range");
            auto col_view = VectorView<T, size_t>();
            if constexpr (ORD == ColMajor)
                col_view = VectorView<T, size_t>(rows, 1, data + c*dist);
            else 
                col_view = VectorView<T, size_t>(rows, dist, data + c);
            
                return col_view;
        }

        // Return a MatrixView corresponding to the specified range of rows [first, next)
        auto RowRange(size_t first, size_t next) const {
            if (first >= next || next > rows) throw out_of_range("Row range out of range");
            auto mat_view = MatrixView<T, ORD>();
            if constexpr (ORD == ColMajor)
                mat_view = MatrixView<T, ORD>(next - first, cols, dist, data + first);
            else
                mat_view = MatrixView<T, ORD>(next - first, cols, dist, data + first*dist);
            return mat_view;
        }

        // Return a MatrixView corresponding to the specified range of columns [first, next)
        auto ColRange(size_t first, size_t next) const {
            if (first >= next || next > cols) throw out_of_range("Col range out of range");
            auto mat_view = MatrixView<T, ORD>();
            if constexpr (ORD == ColMajor)
                mat_view = MatrixView<T, ORD>(rows, next - first, dist, data + first*dist);
            else
                mat_view = MatrixView<T, ORD>(rows, next - first, dist, data + first);
            return mat_view;
        }

        auto SubMatrix(size_t r_first, size_t r_next, size_t c_first, size_t c_next) {
            return RowRange(r_first, r_next).ColRange(c_first, c_next);
        }

        auto Transpose() const {
            return MatrixView<T, (ORD == ColMajor ? RowMajor : ColMajor)>(cols, rows, dist, data);
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

        Matrix<T> Inverse() const {
            if (rows != cols)
                throw runtime_error("Matrix must be square to compute its inverse");

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
                    throw runtime_error("Matrix is singular and cannot be inverted");

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
            return augmented.RangeCols(cols, 2 * cols);
        }

    };

    template <typename T, ORDERING ORD>
    ostream& operator<<(ostream& os, const MatrixView<T, ORD>& m) {
        for (size_t i = 0; i < m.Rows(); ++i) {
            for (size_t j = 0; j < m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }
}