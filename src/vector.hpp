#include "mathlib.hpp"
namespace Mathlib {
    template <typename T>
    class Vector
    {
    private:
        size_t size;
        T * data;
        
    public:
        Vector(size_t _size) : size(_size), data(new T[size]) { }

        template <size_t _size>
        Vector(const T (&arr)[_size]) {
            size = _size;
            data = new T[_size];
            for (size_t i = 0; i < _size; ++i)
            data[i] = arr[i];
        }
        
        Vector (const Vector& v) : Vector(v.Size()) {
            *this = v;
        }

        Vector (Vector&& v) : size(0), data(nullptr) {
            std::swap(size, v.size);
            std::swap(data, v.data);
        }

        ~Vector () { delete [] data; }
    
        Vector<T>& operator=(const Vector<T>& v2) {
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

        template<typename TB>
        auto operator+(const Vector<TB>& other) const
            -> Vector<decay_t<decltype(declval<T>() + declval<TB>())>>
        {
            using TRES = decay_t<decltype(declval<T>() + declval<TB>())>;

            if (this->Size() != other.Size())
                throw runtime_error("Vector sizes must match for addition");

            Vector<TRES> sum(this->Size());
            for (size_t i = 0; i < this->Size(); ++i) {
                sum(i) = (*this)(i) + other(i);
            }
            return sum;
        }
    };



    template <typename T>
    ostream& operator<<(ostream& os, const Vector<T>& v) {
        if (v.Size() > 0)
        os << v(0);
        for (size_t i = 1; i < v.Size(); i++)
        os << ", " << v(i);
        return os << "\n";
    }
}