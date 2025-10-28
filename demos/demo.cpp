#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"
#include <time.h>

using namespace Mathlib;
using namespace std;

int main() {
    Matrix<double, ColMajor> m1(3, 3);
    Matrix<double, ColMajor> m2(3, 3);

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j) {
            m1(i, j) = i + j + 1;
        }

    cout << m1 << "\n";
    m2 = m1 * m1;
    m2 = 3 * m1;
    cout << m2 << endl;
}

