# The Matrix class

With the Matrix class you may create any n x m Matrix and perform operations on them. Matrix-Vector operations are also supported. An interface to Lapack is implemented, enabling very efficient computations.

## Creating a Matrix object

Matrix uses template parameters.
- **typename T:** Set data type of stored data. Currently, double is fully supported. Complex is in development. Custom data types are incompatible with Lapack or Python interfacing.
- **ORDERING ORD:** Choose between row-major or column-major storage. ORDERING is an Enumerator defining RowMajor and ColMajor. Default is ColMajor.

Create a double-Matrix with 5 cols and 5 rows using row-major storage:

```cpp
size_t n = 5;
Matrix<double> mat(n, n);
```

## Data views

You may extract a single row or column of a Matrix with Row() and Col() these functions return a VectorView, merely altering how you look at the stored data and thus incurring no additional memory cost.

```cpp
auto firstRow = mat.Row(0);
auto secondCol = mat.Col(1);
```

Similarly, you may extract a range of Rows or Columns using RowRange() or ColRange(). These functions return a MatrixView, once again incurring no additional memory cost. Indexing is [fromRow, toRow).

```cpp
auto rowRange = mat.RowRange(0, 2);
// Returns the first two rows

auto colRange = mat.ColRange(n-2, n);
// Returns the last two cols
```

Another function making use of data views is Transpose(), unsurprisingly returning a transposed view of the matrix.

```cpp
auto matT = mat.Transpose();
```

You may also extract the main diagonal of a matrix as a VectorView with

```cpp
auto diag = mat.Diag();
```

## Arithmetic operations

You may permorm arithmetic operations on Matrix objects very comfortably with operator overloads. Scalar multiplication is currently only supported for the double type.

```cpp
size_t n = 5;
Matrix<double> A(n,n);
Matrix<double> B(n,n);
Matrix<double> result(n, n);

// Fill A, B with data...

result = A + B;
result = A - B;
result = A * B;
result = 2.5 * A;
```

Matrix*Vector is also supported, though currently only with the Matrix as the left operand.

```cpp
Vector<double> v(5);
Vector<double> res(5);

res = A * v;
```

Operator overloads are implemented via expression templates using CRTP, enabling comfortable chaining of operations without incurring additional computational or memory cost.

```cpp
result = A + B * -(3 * A);
```

However, be careful when inlining matrix-matrix multiplication as the cost exponentially increases. For a chain of length $m$ and matrices of size $n$ x $n$, the cost for inline-multiplication is $O(n^m)$, whereas creating temporaries yields $O(mn^3)$. For this reason, recommend you make use of the Lapack interface for large matrix multiplications.

```cpp
result = A * B;             // slow...
result = A * B | Lapack;    // fast!
```

## Other functions

Matrix provides primitive functions for calculating its inverse and determinant using Gaussian elimination, as well as the trace;

```cpp
Matrix<double> invA = A.Invert();
double det = A.Det();
double trace = A.Trace();
```

However, for finding the inverse and determinant, interfacing Lapack is once again much more efficient. You may use LapackLU for this:

```cpp
Matrix<double> invA = LapackLU(A).inverse(); 
// Copy A by copy-ctor before being overwritten by LU factors

Matrix<double> invB = LapackLU(std::move(B)).inverse();
// Re-use B's memory for LU factors
```

