#include "../src/mathlib.hpp"
#include "../src/matrix.hpp"
#include "../src/fraction.hpp"

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

	std::cout << "Row range (1,2) of each matrix:\n";
	std::cout << "A.RowRange(1,2):\n" << A.RowRange(1,2) << "\n";
	std::cout << "B.RowRange(1,2):\n" << B.RowRange(1,2) << "\n";
	std::cout << "C.RowRange(1,2):\n" << C.RowRange(1,2) << "\n";
	std::cout << "D.RowRange(1,2):\n" << D.RowRange(1,2) << "\n\n";

	std::cout << "Column range (1,2) of each matrix:\n";
	std::cout << "A.ColsRange(1,2):\n" << A.ColRange(1,2) << "\n";
	std::cout << "B.ColsRange(1,2):\n" << B.ColRange(1,2) << "\n";
	std::cout << "C.ColsRange(1,2):\n" << C.ColRange(1,2) << "\n";
	std::cout << "D.ColsRange(1,2):\n" << D.ColRange(1,2) << "\n\n";
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

void TestInverse(){
	Matrix<Fraction> A(3,3);
	for(size_t i=0; i<3; ++i)
		for(size_t j=0; j<3; ++j) 
			A(i,j) = (i == j) ? Fraction(2) : Fraction(1);

	std::cout << "Matrix A:\n" << A << "\n";
	std::cout << "Inverse of A:\n" << A.Inverse() << "\n";
}	

int main()
{
	Vector<double> v(5);
	for(size_t i=0; i<5; ++i)
		v(i) = i+1;
	std::cout << "Vector v: " << v << "\n";

	TestTranspose();
	TestColRowViews();
	TestInverse();
	return 0;	
}
