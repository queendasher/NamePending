# Welcome to NamePending's documentation!


NamePending is a C++ library for basic linear algebra operations.
The library provides template classes **Vector**, **Matrix**, **Fraction** and **Polynomial**.

## Installation

install it via git-clone:

    git clone https://github.com/queendasher/NamePending.git


To configure and build some tests do

    cd NamePending
    mkdir build
    cd build
    cmake ..
    make
    

## Using NamePending

To use NamePending in your code, set the compiler include path properly, and include the header files

    #include "../src/fraction.hpp"
    #include "../src/matrix.hpp"

All objects are implemented in the namespace Mathlib. To use them with less typing, you can set

    namespace mLib = Mathlib;

or even

    
    using namespace Mathlib;

    

You can create vectors and compute with vectors like:

                 
```cpp
Vector<double> x(5), y(5), z(5);
for (int i = 0; i < x.Size(); i++)
   x(i) = i;
y = 5.0
z = x+3*y;
cout << "z = " << z << endl;
```

You can create matrices and compute with matrices like:

```cpp
size_t n = 3;
Matrix<double> m1(n, n);
Matrix<double> m2(n, n);
for(size_t i = 1; i <= n; ++i)
    for(size_t j = 1; j <= n; ++j)
    {
        m1(i, j) = 1.5;
        m2(i, j) = 2.0;
    }
Matrix product = m1 * m2;
```

You can extract a row or a column from a matrix:

```cpp
// Todo: Implement row extraction
```

TODO ...  

   
