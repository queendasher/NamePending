#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"

using namespace Mathlib;

void TestColRowViews(){
	Matrix<double, RowMajor> A(3,3);
	Matrix<double, ColMajor> B(3,3);	
	Matrix<double, RowMajor> C(3,3);
	Matrix<double, ColMajor> D(3,3);

	for(size_t i=0; i<3; ++i)
		for(size_t j=0; j<3; ++j) {
			A(i,j) = i + j*3;
			B(i,j) = i + j*3 + 1;
			C(i,j) = i + j*3 + 2;
			D(i,j) = i + j*3 + 3;
		}

	std::cout << "Matrix A (RowMajor):\n" << A << "\n";
	std::cout << "Matrix B (ColMajor):\n" << B << "\n";
	std::cout << "Matrix C (RowMajor):\n" << C << "\n";
	std::cout << "Matrix D (ColMajor):\n" << D << "\n";

	std::cout << "2nd row of each matrix:\n";
	std::cout << "A.Row(1): " << A.Row(1) << "\n";
	std::cout << "B.Row(1): " << B.Row(1) << "\n";
	std::cout << "C.Row(1): " << C.Row(1) << "\n";
	std::cout << "D.Row(1): " << D.Row(1) << "\n\n";

	std::cout << "2nd column of each matrix:\n";
	std::cout << "A.Col(1): " << A.Col(1) << "\n";
	std::cout << "B.Col(1): " << B.Col(1) << "\n";
	std::cout << "C.Col(1): " << C.Col(1) << "\n";
	std::cout << "D.Col(1): " << D.Col(1) << "\n\n";
}

void TestTranspose(){
	Matrix<double, RowMajor> A(3,2);
	Matrix<double, ColMajor> B(2,3);	
	
	for(size_t i=0; i<3; ++i)
		for(size_t j=0; j<2; ++j) {
			A(i,j) = i + j*3;
			B(j,i) = i + j*3 + 1;
		}

	std::cout << "Matrix A (RowMajor):\n" << A << "\n";
	std::cout << "Matrix B (ColMajor):\n" << B << "\n";
	std::cout << "Transpose of A:\n" << A.Transpose() << "\n";
}

int main()
{
	Vector<double> v(5);
	for(size_t i=0; i<5; ++i)
		v(i) = i+1;
	std::cout << "Vector v: " << v << "\n";

	TestTranspose();
	TestColRowViews();
	return 0;	
}
