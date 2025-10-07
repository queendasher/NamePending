#include "mathlib.hpp"

namespace Mathlib{
    Complex::Complex(double real, double imag) : a(real), b(imag) {}

    // GETTERS / SETTERS

    int Complex::getReal() { return a; }
    void Complex::setReal(double val) { a = val; }
    int Complex::getImag() { return b; }
    void Complex::setImag(double val) { b = val; }

    // MATH FUNCTIONS

    bool Complex::isReal() { return doubleIsEqual(a, 0); }
    bool Complex::isImaginary()  { return doubleIsEqual(b, 0); }
    double Complex::absoluteValue() { return sqrt(a*a + b*b); }

    // OPERATIONS

    Complex Complex::operator+(Complex z) {
        return Complex(a + z.a, b + z.b);
    }

    Complex Complex::operator-(Complex z) {
        return Complex(a - z.a, b - z.b);
    }

    Complex Complex::operator*(Complex z) {
        return Complex(a*z.a - b*z.b, a*z.b + b*z.a);
    }

    Complex Complex::operator~() const {
        return Complex(a, -b); 
    }

    Complex operator+(double r, Complex z) {return Complex(z.a + r, z.b); }
    Complex operator+(Complex z, double r) {return r+z; }

    Complex operator-(double r, Complex z) { return Complex(z.a - r, z.b); }
    Complex operator-(Complex z, double r) { return r-z; }

    Complex operator*(double r, Complex z) { return Complex(z.a*r, z.b*r); }
    Complex operator*(Complex z, double r) { return r*z; }

    // STRING OUTPUT

    void Complex::print() { 
        cout << a << ((b < 0) ? " - " : " + ") << fabs(b) << "i" << endl; 
    }

    ostream& operator<<(ostream& os, Complex z) {
        return os << z.a << ((z.b < 0) ? " - " : " + ") << fabs(z.b) << "i";
    }
}


