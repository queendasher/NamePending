#ifndef FILE_VECTOR
#define FILE_VECTOR

#include "mathlib.hpp"
#include "expression.hpp"

namespace Mathlib
{
	template<typename T, typename TDIST = integral_constant<size_t, 1>>
	class VectorView : public VecExpr<VectorView<T, TDIST>> {
	protected:
		T* data{};
		size_t size{};
		TDIST dist{};

	public:
		VectorView() = default; // initializes members to zero / nullptr
		VectorView(const VectorView&) = default; // shallow copy

		template<typename TDIST2>
		VectorView(const VectorView<T, TDIST2>& other) : data(other.Data()), size(other.Size()), dist(other.Dist()) { } // allow implicit conversion between different dist types
		VectorView(size_t _size, T* _data) : data(_data), size(_size) { }
		VectorView(size_t _size, TDIST _dist, T* _data) : data(_data), size(_size), dist(_dist) { }

		template<typename E>
		VectorView& operator=(const VecExpr<E>& other) {
			for (size_t i = 0; i < size; ++i)
				data[dist*i] = other(i);
			return *this;
		}

		VectorView& operator=(T scal) {
			for (size_t i = 0; i < size; ++i)
				data[dist*i] = scal;
			return *this;
		}

		T* Data() const { return data; }
		size_t Size() const { return size; }
		auto Dist() const { return dist; }

		T& operator()(size_t i) { 
			if (i >= size) throw out_of_range("Vector index out of range");
			return data[dist*i]; 
		}

		const T& operator()(size_t i) const { 
			if (i >= size) throw out_of_range("Vector index out of range");
			return data[dist*i]; 
		}

		// Create a sub-view of the vector from [first, next)
		auto Range(size_t first, size_t next) const {
			if (first >= next || next > size) throw out_of_range("Range out of range");
			return VectorView<T, size_t>(next-first, dist, data+first*dist);
		}

		// Create a slice view of every 'slice'-th element starting from 'first'
		auto Slice(size_t first, size_t slice) const {
			if (first >= size) throw out_of_range("Slice out of range");
			if (slice == 0) throw invalid_argument("Slice step cannot be zero");
			return VectorView<T, size_t>(size/slice, dist*slice, data+first*dist);
		}
	};




	template<typename T>
	class Vector : public VectorView<T> {
		typedef VectorView<T> BASE;
		using BASE::size;
		using BASE::data;

	public:
		Vector(size_t size_) : VectorView<T>() { 
			size = size_; 
			T* data_;
			try {
				data_ = new T[size_];
			} catch (bad_alloc&) {
				throw runtime_error("Memory allocation failed");
			}
			*this = VectorView<T>(size, data_);
		}

		Vector(const Vector& other) : Vector(other.Size()) {
			*this = other;
		}

		Vector(Vector&& other) : VectorView<T>(0, nullptr) {
			swap(size, other.size);
			swap(data, other.data);
		}

		template <typename T2>
		Vector(const VecExpr<T2>& other) : Vector(other.Size()) {
			*this = other;
		}

		using BASE::operator=;
		Vector& operator=(const Vector& other) {
			size = other.size;
			for (size_t i = 0; i < size; i++)
				data[i] = other(i);
			return *this;
		}

		Vector& operator=(Vector&& other) {
			if (this == &other) return *this; // self-assignment check
			delete[] data;
			size = other.size;
			data = other.data;
			other.size = 0;
			other.data = nullptr;
			return *this;
		}

		~Vector() { delete[] data; }
	};


	template <typename T, typename TDIST>
	ostream& operator<<(ostream& os, const VectorView<T, TDIST>& v) {
		if (v.Size() > 0)
			os << v(0);
		for (size_t i = 1; i < v.Size(); ++i)
			os << ", " << v(i);
		return os;
	}
}

#endif