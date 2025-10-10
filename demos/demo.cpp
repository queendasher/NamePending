#include "../src/mathlib.hpp"
#include "../src/vector.hpp"

using namespace Mathlib;

int main()
{
  size_t n = 10;
  Vector<double> x(n), y(n);

  for (size_t i = 1; i <= x.Size(); i++)
    {
      x(i) = i;
      y(i) = 10;
    }

  Vector<double> z = x+y;
  
  std::cout << "x+y = " << z << std::endl;
  
  std::cout << "type of (x+3*y) is  " << typeid(x+3*y).name() << std::endl;

  std::cout << "x+3*y = " << x+3*y << std::endl;

  std::cout << "sizeof(x+3*y) = " << sizeof(x+3*y) << std::endl;
  
  x.Range(2,9) = 3;
  x.Slice(1,2) = 99;
  
  std::cout << "x = " << x << std::endl;  
}
