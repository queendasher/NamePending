#ifndef INCLUDE_MATHLIB__
#define INCLUDE_MATHLIB__
#include <cmath>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <numeric>
using namespace std;

namespace Mathlib {
    bool doubleIsEqual(const double a, const double b, const double epsilon = __FLT_EPSILON__) { 
        return fabs(a - b) < epsilon; 
    }

    class Complex {
    private:
        double a; // Real
        double b; // Imaginary

    public:
        Complex(double real, double imag); // Constructor

        int Re();
        int Im();

        bool isReal(); // Returns true if the number is purely real, false otherwise
        bool isImaginary(); // Returns true if the number is purely imaginary, false otherwise
        double AbsVal(); // Returns the absolute value sqrt(a^2 + b^2)

        Complex operator-(); // Unary negation
        Complex operator+(Complex z); // Called when addition is performed with another complex number
        Complex operator-(Complex z); // Called when subtraction is performed with another complex number
        Complex operator*(Complex z); // Called when multiplication is performed with another complex number
        Complex operator~() const ; // Conjugate

        friend Complex operator+(double r, Complex z); // Called when addition is performed with a real number
        friend Complex operator+(Complex z, double r); // Called when addition is performed with a real number
        friend Complex operator-(double r, Complex z); // Called when subtraction is performed with a real number
        friend Complex operator-(Complex z, double r); // Called when subtraction is performed with a real number
        friend Complex operator*(double r, Complex z); // Called when multiplication is performed with a real number
        friend Complex operator*(Complex z, double r); // Called when multiplication is performed with a real number
        
        
        void print(); // Prints the complex number with format "a + bi"
        friend ostream& operator<<(ostream& os, Complex z); // Called when number is used in ostream
    };

    class Fraction {
    private:
        int p;
        int q;

        void reduce();
        void expand(int q_new); // Expands fraction to a multiple of current denominator

    public:
        Fraction();
        Fraction(int p, int q);
        Fraction(double d);
        ~Fraction();

        double toDouble() const ;
        Fraction expanded(int q_new) const; // Returns expanded form of fraction for a multiple of current denominator
        Fraction sqrtHeron(const Fraction& x0, const double tau) const; // Returns approximated square root of fraction using Heron method

        Fraction operator+(const Fraction& f) const;
        Fraction operator-(const Fraction& f) const;
        Fraction operator*(const Fraction& f) const;
        Fraction operator/(const Fraction& f) const;
        Fraction operator-() const; // Negation
        Fraction operator||(const Fraction& f) const;
        Fraction operator&&(const Fraction& f) const;

        bool operator==(const Fraction& f) const;
        bool operator!=(const Fraction& f) const;
        bool operator<(const Fraction& f) const;
        bool operator<=(const Fraction& f) const;

        friend ostream& operator<<(ostream& os, const Fraction& f);
        void print() const;
    };

    template <typename T>
    class Matrix {
    private:
        size_t rows, cols; // Currently using column-store first
        T* data;

    public:
        Matrix(size_t r, size_t c) : rows(r), cols(c) {
            data = new T[rows * cols]();
        }

        Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
            data = new T[rows * cols];
            for (size_t i = 0; i < rows * cols; ++i)
                data[i] = other.data[i];
        }

        Matrix(Matrix&& other) : rows(other.rows), cols(other.cols), data(other.data) {
            other.data = nullptr;
            other.rows = other.cols = 0;
        }

        Matrix& operator=(const Matrix& other) { // Copy
            if (this != &other) {
                delete[] data;
                rows = other.rows;
                cols = other.cols;
                data = new T[rows * cols];
                for (size_t i = 0; i < rows * cols; ++i)
                    data[i] = other.data[i];
            }
            return *this;
        }

        Matrix& operator=(Matrix&& other) { // Move
            if (this != &other) {
                delete[] data;
                rows = other.rows;
                cols = other.cols;
                data = other.data;
                other.data = nullptr;
                other.rows = other.cols = 0;
            }
            return *this;
        }

        ~Matrix() {
            delete[] data;
        }

        size_t Rows() const { return rows; }
        size_t Cols() const { return cols; }

        Matrix operator-() const {
            Matrix<T> result(rows, cols);
            for (size_t j = 1; j <= cols; ++j)
                for (size_t i = 1; i <= rows; ++i)
                    result(i, j) = -(*this)(i, j);
            return result;
        }

        Matrix operator+(const Matrix& other) const {
            if (rows != other.rows || cols != other.cols)
                throw std::invalid_argument("Matrix dimensions do not match for addition");

            Matrix<T> result(rows, cols);
            for (size_t j = 1; j <= cols; ++j)
                for (size_t i = 1; i <= rows; ++i)
                    result(i, j) = (*this)(i, j) + other(i, j);
            return result;
        }

        Matrix operator-(const Matrix& other) const {
            return *this + (-other);
        }

        Matrix operator*(const Matrix& other) const {
        if (cols != other.rows)
            throw invalid_argument("Matrix dimensions do not match for multiplication");

            Matrix<T> result(rows, other.cols);
            for (size_t i = 1; i <= rows; ++i) {
                for (size_t j = 1; j <= other.cols; ++j) {
                    T sum = T();
                    for (size_t k = 1; k <= cols; ++k) {
                        sum += (*this)(i, k) * other(k, j);
                    }
                    result(i, j) = sum;
                }
            }

            return result;
        }

        T& operator()(size_t r, size_t c) {
            if (r > rows || c > cols || r <= 0 || c <= 0)
                throw out_of_range("Matrix index out of range");
            return data[(c-1) * rows + (r-1)];
        }

        const T& operator()(size_t r, size_t c) const {
            if (r > rows || c > cols || r <= 0 || c <= 0)
                throw out_of_range("Matrix index out of range");
            return data[(c-1) * rows + (r-1)];
        }

        void print() const {
            cout << *this;
        }
    };

    template <typename T>
    class Vector
    {
    private:
        size_t size;
        T * data;
        
    public:
        Vector(size_t _size) : size(_size), data(new T[size]) { ; }
        
        Vector (const Vector & v) : Vector(v.Size()) {
            *this = v;
        }

        Vector (Vector && v) : size(0), data(nullptr) {
            std::swap(size, v.size);
            std::swap(data, v.data);
        }

        ~Vector () { delete [] data; }
    
        Vector<T>& operator=(const Vector<T> & v2) {
            for (size_t i = 0; i < size; i++)
                data[i] = v2(i);
            return *this;
        }

        Vector<T>& operator=(Vector && v2) {
            std::swap(size, v2.size);
            std::swap(data, v2.data);
            return *this;
        }
        
        size_t Size() const { return size; }
        T& operator()(size_t i) { return data[i]; }
        const T& operator()(size_t i) const { return data[i]; }

        Vector<T> operator+(const Vector<T> & other)
        {
            Vector<T> sum(this->Size());
            for (size_t i = 0; i < this->Size(); i++)
            sum(i) = *this(i)+other(i);
            return sum;
        }
    };

    class Polynomial {
    private:
        int n;
        vector<double> coefficients;

        void trim(); // Helper function to trim lead zeros

    public:
        Polynomial(); 
        Polynomial(vector<double>& coefficients);
        ~Polynomial();

        Polynomial(const Polynomial& p);
        Polynomial& operator=(const Polynomial& p);

        int getDegree() const ;

        double evaluate(double x) const ; // Evaluates the polynomial for x
        double evaluate(double a, double b) const ; // Evaluates the defined integral from a to b
        Polynomial derivative(int degree = 1) const ; // Returns the degree-th derivative
        Polynomial integral(double c = 0.0) const ; // Returns the antiderivative with constant c
        double computeZero(const double x0, const double tau, const int max_iter = 1e6) const ; // Computes root using Newton's method

        Polynomial operator+(const Polynomial& p) const ;
        Polynomial operator*(const Polynomial& p) const ;

        friend ostream& operator<<(ostream& os, const Polynomial& p); // Called when polynomial is used in ostream
        void print() const ;
    };

    // << overload for Vector class
    template <typename T>
        ostream& operator<<(ostream& os, const Vector<T>& v)
        {
            if (v.Size() > 0)
            os << v(0);
            for (size_t i = 1; i < v.Size(); i++)
            os << ", " << v(i);
            return os << "\n";
        }
    
    // << overload for Matrix class
    template <typename T> 
    ostream& operator<<(ostream& os, const Matrix<T>& m) {
        for (size_t i = 1; i <= m.Rows(); ++i) {
            for (size_t j = 1; j <= m.Cols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }

        return os;
    }

}
#endif