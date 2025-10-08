#include "mathlib.hpp"

namespace Mathlib {
    class Fraction {
    private:
        int p;
        int q;

        void reduce() {
            if(q == 0) {
                throw logic_error("Divide by zero.");
            }
            int _gcd = gcd(p, q);

            p /= _gcd;
            q /= _gcd;

            if (q < 0) {
                p = -p;
                q = -q;
            }
        }

        void expand(int q_new) {
            if (q_new % q == 0) { // Only expand if new denominator is whole number multiple of current denominator
                int ratio = q_new / q;
                q = q_new;
                p *= ratio;
            }
        }

    public:
        Fraction(int p=0, int q=1) : p(p), q(q) {
            reduce();
        }

        Fraction(double d) {
            if (fabs(d) < __FLT_EPSILON__) {
                p = 0;
                q = 1;
                return;
            }

            int sign = 1;
            if (d < 0) {
                sign = -1;
                d = -d;
            }

            int a = floor(d);
            double b = d - a;
            int c = round(b*1e9);

            Fraction result = Fraction(a, 1) + Fraction(c, (int)1e9);
            p = result.p * sign;
            q = result.q;
            reduce();
        }

        ~Fraction() { }

        double toDouble() const {
            return (double)(p) / q;
        }

        int getP() const { return p; }
        int getQ() const { return q; }

        Fraction expanded(int q_new) const {
            if (q_new % q != 0) {
                throw invalid_argument("New denominator must be a whole number multiple of the current denominator.");
            }

            Fraction result;
            result.p = p*(q_new/q);
            result.q = q_new;

            return result;
        }

        Fraction sqrtHeron(const Fraction& x0, const double tau) const {
            const Fraction zero = Fraction();
            const Fraction one_half = Fraction(1, 2);

            Fraction xn = x0;
            Fraction tol = Fraction(tau);
            Fraction diff = tol;
            
            while (tol <= diff) {
                Fraction xn_next = one_half * (xn + (*this)/xn);
                diff = xn_next - xn;
                xn = xn_next;
                if (diff < zero) diff = -diff;
            } 
            
            return xn;
        }

        Fraction operator+(const Fraction& f) const {
            Fraction result;
            int _lcm = std::lcm(q, f.q);

            result.q = _lcm;
            result.p = this->expanded(_lcm).p + f.expanded(_lcm).p;
            result.reduce();

            return result;
        }

        Fraction operator-(const Fraction& f) const {
            return *this + (-f);
        }

        Fraction operator*(const Fraction& f) const {
            return Fraction(p*f.p, q*f.q);
        }

        Fraction operator/(const Fraction& f) const {
            return Fraction(p*f.q, q*f.p);
        }

        Fraction operator-() const {
            return Fraction(-p, q);
        }

        Fraction operator||(const Fraction& f) const {
            int _lcm = std::lcm(q, f.q);
            int _max = max(this->expanded(_lcm).p, f.expanded(_lcm).p);
            
            return Fraction(_max, _lcm);
        }

        Fraction operator&&(const Fraction& f) const {
            int _lcm = std::lcm(q, f.q);
            int _min = min(this->expanded(_lcm).p, f.expanded(_lcm).p);
            
            return Fraction(_min, _lcm);
        }

        bool operator==(const Fraction& f) const {
            int _lcm = std::lcm(q, f.q);

            return this->expanded(_lcm).p == f.expanded(_lcm).p;
        }

        bool operator!=(const Fraction& f) const {
            return !(*this == f);
        }

        bool operator<(const Fraction& f) const {
            int _lcm = std::lcm(q, f.q);

            return this->expanded(_lcm).p < f.expanded(_lcm).p;
        }

        bool operator<=(const Fraction& f) const {
            int _lcm = std::lcm(q, f.q);

            return this->expanded(_lcm).p <= f.expanded(_lcm).p;
        }

        void print() const {
            cout << (this);
        }
    };

    ostream& operator<<(ostream& os, const Fraction& f) {
        return os << f.getP() << "/" << f.getQ() << " (" << f.toDouble() << ")";
    }
}