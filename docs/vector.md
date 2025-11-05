# The Vector class

With the Vector class you may create any Vector of size and perform operations on them. An interface to Lapack is implemented, enabling very efficient computations.

## Creating a Vector object

Vector uses template parameters.
- **typename T:** Set data type of stored data. Custom data types are incompatible with Lapack or Python interfacing.
- **typename TDIST:** Choose the distance between two data entries in the memory. Default is `std::integral_constant<1>`.

Create a double-Vector of size 4:

```cpp
size_t n = 4;
Vector<double> vec(n);
vec = 24 // Overwrite all elements with 24
```

## Arithmetic operations

You may permorm arithmetic operations on Vector objects with operator overloads. Scalar multiplication is currently only supported for the double type.

```cpp
size_t n = 5;
Vector<double> vec1(n);
Vector<double> vec2(n);
Vector<double> result(n);

// Fill vec1, vec2 with data...

result = vec1 + vec2;
result = vec1 - vec2;
result = 2.5 * vec1;
result = -vec1;
int dotProd = Dot(vec1, vec2);
result = VecMul(vec1, vec2); // Element-wise multiplication
```

Operator overloads are implemented via expression templates using CRTP, enabling comfortable chaining of operations without incurring additional computational or memory cost.

```cpp
result = vec1 + vec2 * -(3 * vec1);
```

## Other functions

Vector provides functions for creating views containing certain elements of the original vector. 

The `Range`-function creates a sub-view of the vector from [`begin`,`end`):
```cpp
int begin = 1;
int end = 3;
// View containing the elements with index 1 and 2:
auto rangeView = vec.Range(begin, end);
```

The `Slice`-function creates a sub-view containing every `n`-th element starting with `start`:
```cpp
int n = 2;
int start = 1;
// View containing the elements with index 1 and 3:
auto sliceView = vec.Slice(start,n);
```
