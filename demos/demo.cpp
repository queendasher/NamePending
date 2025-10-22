#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"
#include <time.h>

using namespace Mathlib;

int main() {
    Matrix<double, ColMajor> m1(4, 3);
    for (size_t i = 0; i < m1.Rows(); ++i)
        for (size_t j = 0; j < m1.Cols(); ++j)
            m1(i, j) = static_cast<double>(i * m1.Cols() + j);

    cout << m1 << endl;
    Matrix<double, ColMajor> m2 = std::move(m1);
    cout << m2 << endl;
    cout << m1.Rows() << endl;
}

