#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/matrix.hpp"
using namespace Mathlib;

template <typename T, ORDERING ORD>
void run_1(size_t rows, size_t cols) {
    // Constructor of a 3x3 matrix
    Matrix<T, ORD> m1(rows, cols);
    for (size_t i = 0; i < m1.Rows(); ++i)
        for (size_t j = 0; j < m1.Cols(); ++j)          // m1 = 0, 1, 2 ...
            m1(i, j) = static_cast<T>(i * cols + j);    //      ...  

    // Read Access
    for (size_t i = 0; i < m1.Rows(); ++i)
        for (size_t j = 0; j < m1.Cols(); ++j)
            REQUIRE(m1(i, j) == static_cast<T>(i * cols + j));

    // Copy constructor
    Matrix<T, ORD> m2 = m1;
    REQUIRE(m2.Rows() == m1.Rows());
    REQUIRE(m2.Cols() == m1.Cols());
    for (size_t i = 0; i < m2.Rows(); ++i)
        for (size_t j = 0; j < m2.Cols(); ++j)
            REQUIRE(m2(i, j) == m1(i, j));

    // Copy constructor with different ordering
    Matrix<T, (ORD == ColMajor ? RowMajor : ColMajor)> m3 = m1;
    REQUIRE(m3.Rows() == m1.Rows());
    REQUIRE(m3.Cols() == m1.Cols());
    for (size_t i = 0; i < m3.Rows(); ++i)
        for (size_t j = 0; j < m3.Cols(); ++j)
            REQUIRE(m3(i, j) == m1(i, j));

    // Move constructor
    Matrix<T, ORD> m4 = std::move(m1);
    REQUIRE(m4.Rows() == rows);
    REQUIRE(m4.Cols() == cols);
    for (size_t i = 0; i < m4.Rows(); ++i)
        for (size_t j = 0; j < m4.Cols(); ++j)
            REQUIRE(m4(i, j) == static_cast<T>(i * cols + j));
    REQUIRE(m1.Rows() == 0);
    REQUIRE(m1.Cols() == 0);

    // Move constructor with different ordering
    Matrix<T, (ORD == ColMajor ? RowMajor : ColMajor)> m5 = std::move(m2);
    REQUIRE(m5.Rows() == rows);
    REQUIRE(m5.Cols() == cols);
    for (size_t i = 0; i < m5.Rows(); ++i)
        for (size_t j = 0; j < m5.Cols(); ++j)
            REQUIRE(m5(i, j) == static_cast<T>(i * cols + j));
    // REQUIRE(m2.Rows() == 0);
    // REQUIRE(m2.Cols() == 0);

    // Copy assignment
    Matrix<T, ORD> m6(rows, cols);
    m6 = m4;
    REQUIRE(m6.Rows() == m4.Rows());
    REQUIRE(m6.Cols() == m4.Cols());
    for (size_t i = 0; i < m6.Rows(); ++i)
        for (size_t j = 0; j < m6.Cols(); ++j)
            REQUIRE(m6(i, j) == m4(i, j));

    // Copy assignment with different ordering
    Matrix<T, (ORD == ColMajor ? RowMajor : ColMajor)> m7(rows, cols);
    m7 = m5;
    REQUIRE(m7.Rows() == m5.Rows());
    REQUIRE(m7.Cols() == m5.Cols());
    for (size_t i = 0; i < m7.Rows(); ++i)
        for (size_t j = 0; j < m7.Cols(); ++j)
            REQUIRE(m7(i, j) == m5(i, j));

    // Move assignment
    Matrix<T, ORD> m8(rows, cols);
    m8 = std::move(m4);
    REQUIRE(m8.Rows() == rows);
    REQUIRE(m8.Cols() == cols);
    for (size_t i = 0; i < m8.Rows(); ++i)
        for (size_t j = 0; j < m8.Cols(); ++j)
            REQUIRE(m8(i, j) == static_cast<T>(i * cols + j));
    REQUIRE(m4.Rows() == 0);
    REQUIRE(m4.Cols() == 0);

    // Move assignment with different ordering
    Matrix<T, (ORD == ColMajor ? RowMajor : ColMajor)> m9(rows, cols);
    m9 = std::move(m5);
    REQUIRE(m9.Rows() == rows);
    REQUIRE(m9.Cols() == cols);
    for (size_t i = 0; i < m9.Rows(); ++i)
        for (size_t j = 0; j < m9.Cols(); ++j)
            REQUIRE(m9(i, j) == static_cast<T>(i * cols + j));
    // REQUIRE(m5.Rows() == 0);
    // REQUIRE(m5.Cols() == 0);

    // Self-assignment (copy)
    m6 = m6;
    REQUIRE(m6.Rows() == rows);
    REQUIRE(m6.Cols() == cols);
    for (size_t i = 0; i < m6.Rows(); ++i)
        for (size_t j = 0; j < m6.Cols(); ++j)
            REQUIRE(m6(i, j) == static_cast<T>(i * cols + j));

    // Self-assignment (move)
    m7 = std::move(m7);
    REQUIRE(m7.Rows() == rows);
    REQUIRE(m7.Cols() == cols);
    for (size_t i = 0; i < m7.Rows(); ++i)
        for (size_t j = 0; j < m7.Cols(); ++j)
            REQUIRE(m7(i, j) == static_cast<T>(i * cols + j));

    // Scalar assignment
    m8 = static_cast<T>(-1);
    for (size_t i = 0; i < m8.Rows(); ++i)
        for (size_t j = 0; j < m8.Cols(); ++j)
            REQUIRE(m8(i, j) == static_cast<T>(-1));
}

TEST_CASE( "construction, copy, move, assignment" ) {
    run_1<double, ColMajor>(3, 3);
    run_1<double, RowMajor>(3, 3);
    run_1<double, ColMajor>(5, 3);
    run_1<double, RowMajor>(5, 3);
}