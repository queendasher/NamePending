#include "../src/mathlib.hpp"

namespace bla = Mathlib;


int main() {
    bla::Matrix<double> m(2, 3);
    m(1, 1) = 1.1; m(1, 2) = 2.2; m(1, 3) = 3.3;
    m(2, 1) = 4.4; m(2, 2) = 5.5; m(2, 3) = 6.6;

    std::cout << m;

    return 0;
}
