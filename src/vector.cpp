#include "mathlib.hpp"

namespace Mathlib {
    template <typename T>
    Vector<T>::Vector(size_t _size) : size(_size), data(new T[size]) { ; }
    
    template <typename T>
    Vector<T>::Vector (const Vector & v)
    : Vector(v.Size())
    {
    *this = v;
    }

    template <typename T>
    Vector<T>::Vector (Vector && v)
    : size(0), data(nullptr)
    {
    std::swap(size, v.size);
    std::swap(data, v.data);
    }

    template <typename T>
    Vector<T>::~Vector () { delete [] data; }
    
    template <typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T> & v2)
    {
    for (size_t i = 0; i < size; i++)
        data[i] = v2(i);
    return *this;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(Vector && v2)
    {
    std::swap(size, v2.size);
    std::swap(data, v2.data);
    return *this;
    }
    
    template <typename T> size_t Vector<T>::Size() const { return size; }
    template <typename T> T& Vector<T>::operator()(size_t i) { return data[i]; }
    template <typename T> const T& Vector<T>::operator()(size_t i) const { return data[i]; }

    template <typename T>
    Vector<T> Vector<T>::operator+(const Vector<T> & other)
    {
        Vector<T> sum(this->Size());
        for (size_t i = 0; i < this->Size(); i++)
        sum(i) = *this(i)+other(i);
        return sum;
    }

    template <typename T>
    ostream& operator<<(ostream& os, const Vector<T>& v)
    {
        if (v.Size() > 0)
        os << v(0);
        for (size_t i = 1; i < v.Size(); i++)
        os << ", " << v(i);
        return os << "\n";
    }
}
