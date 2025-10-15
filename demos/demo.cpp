#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"

using namespace Mathlib;

int main()
{
	Matrix<double, ColMajor> A(3, 3);
	Matrix<double, RowMajor> B(3, 3);

	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j) {
			A(i, j) = i + j*3;
			B(i, j) = i + j*3+6;
		}

	cout << A << "\n";
	cout << A.RowRange(1,3) << "\n";
	cout << A.ColRange(1,3) << "\n";
	cout << A.Row(1) << "\n";
	cout << A.Col(1) << "\n\n\n";
	cout << B << "\n";
	cout << B.RowRange(1,3) << "\n";
	cout << B.ColRange(1,3) << "\n";
	cout << B.Row(1) << "\n";
	cout << B.Col(1) << "\n\n\n";
	cout << B.RowRange(1,3).ColRange(0,2) << "\n";
	cout << A.ColRange(1,3).RowRange(0,2) << "\n";
	cout << B.RowRange(1,3).ColRange(0,2) + A.ColRange(1,3).RowRange(0,2) << "\n";
}
