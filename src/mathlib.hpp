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
        Matrix(size_t r, size_t c);
        Matrix(const Matrix& other); // Copy
        Matrix(Matrix&& other); // Move
        Matrix& operator=(const Matrix& other); // Copy
        Matrix& operator=(Matrix&& other); // Move
        ~Matrix();

        size_t Rows() const ;
        size_t Cols() const ;

        Matrix& operator-() const ;
        Matrix& operator+(const Matrix& other) const ;
        Matrix& operator-(const Matrix& other) const ;
        Matrix& operator*(const Matrix& other) const ; // Move
        T& operator()(size_t r, size_t c) ; // Write element (1-indexed)
        const T& operator()(size_t r, size_t c) const; // Read element (1-indexed)

        //ostream& operator<<(ostream& os, const Matrix<T>& m);

        void print() const;
    };

    template <typename T>
    class Vector
    {
    private:
        size_t size;
        T * data;
        
    public:
        Vector (size_t _size);
        Vector (const Vector & v);
        Vector (Vector && v);
        ~Vector ();
        
        Vector & operator=(const Vector & v2);
        Vector & operator=(Vector && v2);
        size_t Size() const;
        T& operator()(size_t i);
        const T& operator()(size_t i) const;
        Vector<T> operator+(const Vector<T> & other);
        //friend ostream& operator<<(ostream& os, const Vector<T>& v);
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
}

// Need to fiddle with build setting later to destroy this abomination
//#include "polynomial.cpp"
//#include "fraction.cpp"
//#include "complex.cpp"
#include "matrix.cpp"
//#include "vector.cpp"
#endif