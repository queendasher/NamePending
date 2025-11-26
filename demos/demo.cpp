#include <iostream>
#include <sstream>

#include <../NamePending-HPC/src/taskmanager.hpp>
#include <../NamePending-HPC/src/timer.hpp>
#include <../NamePending-HPC/src/lock_guard.hpp>

#include "matrix.hpp"
#include "lapack_interface.hpp"


using namespace ASC_HPC;
using namespace Mathlib;
using namespace std;


int main()
{
  size_t n = 2000;
  Matrix<double> A(n, n);
  Matrix<double> B(n, n);
  Matrix<double> C(n, n);

  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j) {
      A(i, j) = 2.0;
      B(i, j) = 3.0;
    }

  timeline = std::make_unique<TimeLine>("demo.trace");

  
  C = A * B | Parallel;

  cout << "C(20,20) = " << C(20,20) << endl;

  return 0;
}

