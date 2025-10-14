#include "mathlib.hpp"
#include "expression.hpp"

namespace Mathlib
{
	template<typename T, typename TDIST = integral_constant<size_t,1>>
	class VectorView : public VecExpr<VectorView<T, TDIST>> {
	protected:
		T* data;
		size_t size;
		TDIST dist;

	public:
		VectorView() = default;
		VectorView(const VectorView&) = default;

		template <typename TDIST2>
		VectorView(const VectorView<T,TDIST2>& other) : data(other.Data()), size(other.Size()), dist(other.Dist()) { }

		VectorView(size_t _size, T* _data) : data(_data), size(_size) { }

		VectorView(size_t _size, TDIST _dist, T* _data) : data(_data), size(_size), dist(_dist) { }

		template<typename TB>
		VectorView& operator= (const VecExpr<TB>& other) {
			for (size_t i = 0; i < size; ++i)
			data[dist*i] = other(i);
			return *this;
		}

		VectorView& operator= (T scal) {
			for (size_t i = 0; i < size; ++i)
			data[dist*i] = scal;
			return *this;
		}

		T* Data() const { return data; }
		size_t Size() const { return size; }
		auto Dist() const { return dist; }

		T& operator()(size_t i) { return data[dist*i]; }
		const T& operator()(size_t i) const { return data[dist*i]; }

		auto Range(size_t first, size_t next) const {
			return VectorView(next-first, dist, data+first*dist);
		}

		auto Slice(size_t first, size_t slice) const {
			return VectorView<T,size_t>(size/slice, dist*slice, data+first*dist);
		}
	};




	template<typename T>
	class Vector : public VectorView<T> {
		typedef VectorView<T> BASE;
		using BASE::size;
		using BASE::data;

	public:
		Vector(size_t size) : VectorView<T>(size, new T[size]) { }

		Vector(const Vector& v) : Vector(v.Size()) { 
			*this = v;
		}

		Vector(Vector&& v) : VectorView<T> (0, nullptr) {
			swap(size, v.size);
			swap(data, v.data);
		}

		template <typename TB>
		Vector(const VecExpr<TB>& v) : Vector(v.Size()) {
			*this = v;
		}

		~Vector() { delete[] data; }

		using BASE::operator=;
		Vector& operator=(const Vector& other) {
			for (size_t i = 0; i < size; i++)
			data[i] = other(i);
			return *this;
		}

		Vector& operator=(Vector&& other) {
			swap(size, other.size);
			swap(data, other.data);
			return *this;
			}
	};


	template<typename ...Args>
	ostream& operator<<(ostream& os, const VectorView<Args...>& v) {
		if (v.Size() > 0)
			os << v(0);
		for (size_t i = 1; i < v.Size(); ++i)
			os << ", " << v(i);
		return os;
	}
}