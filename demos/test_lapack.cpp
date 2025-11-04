#include <iostream>

#include <vector.hpp>
#include <matrix.hpp>
#include <chrono>
#include <lapack_interface.hpp>


using namespace Mathlib;
using namespace std;


int main()
{
	// Benchmark matrix-matrix multiplication for sizes (10, 100, 1000)
	for(auto n : {10, 100, 1000})
	{
		cout << "----------------------------------------\nsize: " << n << endl;
		size_t flops = n*n*n;
		size_t runs = size_t (1e9 / flops) + 1;

		Matrix<double, RowMajor> A(n,n);
		Matrix<double, RowMajor> B(n,n);
		for(size_t i = 0; i < n; ++i) {
			for(size_t j = 0; j < n; ++j) {
				A(i,j) = i + (j*n);
				B(i,j) = i + (j*n);
			}
		}

		Matrix<double, RowMajor> C(n,n);

		{
			cout << "Lapack Benchmarking: " << endl;
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < runs; ++i)
				C = A * B | Lapack;
			auto end = std::chrono::high_resolution_clock::now();
			double time = std::chrono::duration<double>(end-start).count();
					
			cout << "n = " << n << " (runs = " << runs << "), time = " << time << " s (" << time/runs << " s per run), GFlops = " 
				<< (flops*runs)/time*1e-9 << endl;
		}

		{
			cout << "Own Benchmarking: " << endl;
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < runs; ++i)
				C = A * B;
			auto end = std::chrono::high_resolution_clock::now();
			double time = std::chrono::duration<double>(end-start).count();
					
			cout << "n = " << n << " (runs = " << runs << "), time = " << time << " s (" << time/runs << " s per run), GFlops = " 
				<< (flops*runs)/time*1e-9 << endl;
		}
	}


}

  
