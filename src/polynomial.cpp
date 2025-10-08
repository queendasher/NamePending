#include "../includes/mathlib.hpp"

namespace Mathlib {
    void Polynomial::trim() {
        while (n > 0 && fabs(coefficients[n]) < __FLT_EPSILON__) {
            --n;
        }

        coefficients.resize(n+1);
    }

    Polynomial::Polynomial() {
        n = 0;
        coefficients.resize(1);
        coefficients[0] = 0.0;
    }

    Polynomial::Polynomial(vector<double>& coefficients) {
        this->n = coefficients.size() - 1;
        this->coefficients = coefficients;
        this->trim();
    }

    Polynomial::~Polynomial() { }

    Polynomial::Polynomial(const Polynomial& p) { 
        *this = p;
    }

    Polynomial& Polynomial::operator=(const Polynomial& p) {
        if (this != &p) {
            this->n = p.n;
            this->coefficients.resize(n+1);
            
            for(int i = 0; i <= n; ++i) {
                this->coefficients[i] = p.coefficients[i];
            }

            this->trim();
        }

        return *this;
    }

    int Polynomial::getDegree() const {
        return n;
    }

    double Polynomial::evaluate(double x) const {
        double result = 0.0;
        double power = 1;

        for (int i = 0; i <= n; ++i) {
            result += coefficients[i] * power;
            power *= x;
        }
        return result;
    }

    double Polynomial::evaluate(double a, double b) const {
        Polynomial integral = this->integral();

        return integral.evaluate(b) - integral.evaluate(a);
    }

    Polynomial Polynomial::derivative(int degree) const {
        if (degree < 1){
            return *this;
        }

        if (this->n == 0) {
            return Polynomial(); // Derivative of Polynomial of degree 0 is 0
        }

        vector<double> derivative_coeffs(this->n, 0.0);

        for (int i = 0; i < this->n; ++i) {
            derivative_coeffs[i] = coefficients[i+1] * (i+1); // c*x^n becomes c*n*x^(n-1)
        }

        return Polynomial(derivative_coeffs).derivative(degree-1);
    }

    Polynomial Polynomial::integral(double c) const {
        vector<double> integral_coeffs(this->n+2, 0.0);
        integral_coeffs[0] = c;

        for (int i = 0; i <= this->n; ++i) {
            integral_coeffs[i+1] = coefficients[i] / (i+1);
        }

        return Polynomial(integral_coeffs);
    }

    double Polynomial::computeZero(double x0, double tau, int max_iter) const {
        if(tau < 0) {
            throw invalid_argument("Tolerance < 0");
        }

        Polynomial d = derivative();
        double xk = x0;
        
        for (int k = 0; k < max_iter; ++k) {
            if (fabs(evaluate(xk)) < tau) { // Return xk if p(xk) is near-zero (within tolerance)
                return xk;
            }
            
            if (fabs(d.evaluate(xk)) < tau) {
                throw runtime_error("Derivative too small, Newton's method inapplicable.");
            }
            
            xk = xk - evaluate(xk) / d.evaluate(xk);
        }

        throw runtime_error("Failed to converge in time.");
    }

    Polynomial Polynomial::operator+(const Polynomial& p) const {
        vector<double> result_coeffs(max(n, p.n) + 1, 0.0);
        
        for (int i = 0; i < result_coeffs.size(); ++i) {
            if (i <= n) result_coeffs[i] += coefficients[i];
            if (i <= p.n) result_coeffs[i] += p.coefficients[i];
        }

        return Polynomial(result_coeffs);
    }

    Polynomial Polynomial::operator*(const Polynomial& p) const {
        vector<double> result_coeffs(n+p.n + 1, 0.0);

        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= p.n; ++j) {
                result_coeffs[i+j] += coefficients[i] * p.coefficients[j];
            }
        }

        return Polynomial(result_coeffs);
    }

    ostream& operator<<(ostream& os, const Polynomial& p) {
        for (int i = p.n; i >= 0; --i) {
            if (fabs(p.coefficients[i]) < __FLT_EPSILON__ && i > 0) continue;

            if (i == p.n) {
                os << p.coefficients[i];
            } else {
                os << ((p.coefficients[i] > 0) ? " + " : " - ");
                os << abs(p.coefficients[i]);
            }
            
            if (i > 0) {
                os << "x^" << i;
            }
        }
        
        return os << "\n";
    }

    void Polynomial::print() const {
        cout << *this;
    }
}
