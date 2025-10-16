#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"
#include <time.h>

using namespace Mathlib;

int main() {
	// using T1 = Fraction;
	// using T2 = double;

	// Matrix<T1> A(3, 3);
	// Matrix<T2>B(3, 3);

	// for (size_t i = 0; i < A.Rows(); ++i)
	// 	for (size_t j = 0; j < A.Cols(); ++j) {
	// 		A(i, j) = (i == j) ? T1(2) : T1(1);
	// 		// Invertible matrix with a zero on the diagonal
	// 		B(i, j) = (i == j) ? T2(0) : T2(2);
	// 	}
			

	// cout << "Matrix A:\n" << A << "\n\n";
	// cout << "Inverse of A:\n" << A.Invert() << "\n\n";
	// cout << "Matrix B:\n" << B << "\n\n";
	// cout << "Inverse of B:\n" << B.Invert() << "\n\n";
	// cout << "Trace of A: " << A.Trace() << "\n\n";
	// cout << "Determinant of A: " << A.Det() << "\n\n";
	// cout << "Determinant of B: " << B.Det() << "\n\n";

	Vector<double> v1(5);

	for (size_t i = 0; i < v1.Size(); ++i)
		v1(i) = i;

	Vector<double> v2 = 3 * v1;
	cout << "dot:" << Dot(v1+v1, v2) << "\n";
	cout << VecMul(v1+v1, v2) << "\n";
}
