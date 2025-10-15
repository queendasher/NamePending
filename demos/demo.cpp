#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"
#include <time.h>

using namespace Mathlib;

void TestRepeatedMultiplication(){
	size_t n = 7;
	Matrix<double> A(n, n);
	Matrix<double> B(n, n);

	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			A(i, j) = 2;


	//Measure runtime
	clock_t start = clock();
	cout << A * A * A * A * A * A * A * A * A * A * A << "\n";
	clock_t end = clock();
	double elapsed = double(end - start) / CLOCKS_PER_SEC;
	cout << "Inline multiplication with templates: " << elapsed << " seconds\n";

	start = clock();
	for(size_t i = 0; i < 10; ++i)
		B = A * B;
	end = clock();
	elapsed = double(end - start) / CLOCKS_PER_SEC;
	cout << "Repeated multiplication with loops: " << elapsed << " seconds\n";


}

int main()
{
	// Matrix<double, ColMajor> A(3, 3);
	// Matrix<double, RowMajor> B(3, 3);

	// for (size_t i = 0; i < 3; ++i)
	// 	for (size_t j = 0; j < 3; ++j) {
	// 		A(i, j) = i + j*3;
	// 		B(i, j) = i + j*3+6;
	// 	}

	// cout << A << "\n";
	// cout << A.RowRange(1,3) << "\n";
	// cout << A.ColRange(1,3) << "\n";
	// cout << A.Row(1) << "\n";
	// cout << A.Col(1) << "\n\n\n";
	// cout << B << "\n";
	// cout << B.RowRange(1,3) << "\n";
	// cout << B.ColRange(1,3) << "\n";
	// cout << B.Row(1) << "\n";
	// cout << B.Col(1) << "\n\n\n";
	// cout << B.RowRange(1,3).ColRange(0,2) << "\n";
	// cout << A.ColRange(1,3).RowRange(0,2) << "\n";
	// cout << B.RowRange(1,3).ColRange(0,2) + A.ColRange(1,3).RowRange(0,2) << "\n";

	TestRepeatedMultiplication();
}
