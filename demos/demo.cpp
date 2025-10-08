#include "../includes/mathlib.hpp"

using namespace Mathlib;


int main() {
    size_t n = 3;
    Matrix<Fraction> A(n, n);
    for(size_t i = 1; i <= n; ++i)
        for(size_t j = 1; j <= n; ++j)
            A(i, j) = Fraction((i == j) ? 2 : 1, 1);

    cout << "Matrix A:\n" << A;
    try {
        cout << "REF of A:\n";
        Matrix<Fraction> A_ref = A; // Copy A to preserve original
        A_ref.REF();
        cout << A_ref;
        Matrix<Fraction> A_inv = A.Inverse();
        cout << "Inverse of A:\n" << A_inv;

        Matrix<Fraction> I = A * A_inv;
        cout << "A * A_inv (should be identity):\n" << I;
    } catch (const std::runtime_error& e) {
        cout << "Error: " << e.what() << "\n";
    }
}
