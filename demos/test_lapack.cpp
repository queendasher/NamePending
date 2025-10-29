#include <iostream>

#include <vector.hpp>
#include <matrix.hpp>
#include <lapack_interface.hpp>


using namespace Mathlib;
using namespace std;


int main()
{
	size_t n = 5;
	Matrix<double, RowMajor> A(n,n);
	Matrix<double, ColMajor> B(n,n);

	for(size_t i = 0; i < n; ++i)
	for(size_t j = 0; j < n; ++j) {
		A(i,j) = i + (j*n);
		B(i,j) = i + (j*n);
	}

	Matrix<double, RowMajor> C(n,n);
	multMatMatLapack(A, B, C);
	cout << "C = A*B = " << C << endl;

}

  
