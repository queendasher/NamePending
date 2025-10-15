#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"

using namespace Mathlib;

int main()
{
	Matrix<double, RowMajor> A(4, 7);
	Matrix<double, ColMajor> B(7, 4);
	Matrix<Fraction> C(3, 3);
	Vector<double> v(3);
	Matrix<double> D(3, 3);

	for (size_t i = 0; i < A.Rows(); ++i)
		for (size_t j = 0; j < A.Cols(); ++j) {
			A(i, j) = i + j + 1;
			B(j, i) = i + j + 1;
		}

	for (size_t i = 0; i < C.Rows(); ++i)
		for (size_t j = 0; j < C.Cols(); ++j) {
			C(i, j) = (i==j) ? Fraction(2) : Fraction(1);
			v(i) = i + 1;
			D(i, j) = (i==j) ? 2.0 : 1.0;
		}


	cout << "A = \n" << A << "\n";
	cout << "Transpose(A) = \n" << A.Transpose() << "\n\n";
	cout << "B = \n" << B << "\n";
	cout << "Rows 2-4 of B = \n" << B.RangeRows(1, 4) << "\n\n";
	A.RangeCols(1, 5) = 7.0;
	cout << "After setting Columns 2-5 of A to 7.0:\nA = \n" << A << "\n\n";
	cout << "C = \n" << C << "\n";
	cout << "C Inverse = \n" << C.Inverse() << "\n";
	cout << "5 C = \n" << C + 4*C << "\n";
	cout << "5 C * C^-1 = \n" << (C +4*C) * C.Inverse() << "\n\n"; // Should be 5 * I
	// cout << "D = \n" << D << "\n";
	// cout << "D Inverse = \n" << D.Inverse() << "\n";
	// cout << "D+3D = \n" << 4*D << "\n";
}
