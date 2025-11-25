#include "../NamePending-HPC/src/simd.hpp"
#include "matrix.hpp"

using namespace Mathlib;
using namespace ASC_HPC;
using namespace std;

int main() {
    size_t rows = 4;
    size_t cols = 4;
    Matrix<double, ColMajor> m1(rows, cols);
    Matrix<double, ColMajor> m2(rows, cols);
    Matrix<double, ColMajor> res(rows, cols);

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j) {
            m1(i, j) = i + j + 1;
            m2(i, j) = i + j + 2;
        }

    res = m1 * m2;

    
    cout << "m1:" << endl;
    cout << m1 << endl;

    cout << "m2:" << endl;
    cout << m2 << endl;

    cout << "\nres:" << endl;
    cout << res << endl;
}

