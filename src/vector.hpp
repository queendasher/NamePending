#include "mathlib.hpp"
#include "expression.hpp"

namespace Mathlib
{
	template <typename T, typename TDIST = integral_constant<size_t,1>>
	class VectorView : public VecExpr<VectorView<T,TDIST>> {
	protected:
		T* data;
		size_t size;
		TDIST dist;
	public:
		VectorView() = default;
		VectorView(const VectorView&) = default;

		template <typename TDIST2>
		VectorView(const VectorView<T,TDIST2>& v2) : data(v2.Data()), size(v2.Size()), dist(v2.Dist()) { }

		VectorView(size_t _size, T* _data) : data(_data), size(_size) { }

		VectorView(size_t _size, TDIST _dist, T* _data) : data(_data), size(_size), dist(_dist) { }

		template <typename TB>
		VectorView& operator= (const VecExpr<TB>& v2) {
			for (size_t i = 1; i <= size; i++)
			data[dist*(i-1)] = v2(i);
			return *this;
		}

		VectorView& operator= (T scal) {
			for (size_t i = 0; i < size; i++)
			data[dist*i] = scal;
			return *this;
		}

		T* Data() const { return data; }
		size_t Size() const { return size; }
		auto Dist() const { return dist; }

		T& operator()(size_t i) { return data[dist*(i-1)]; }
		const T& operator()(size_t i) const { return data[dist*(i-1)]; }

		auto Range(size_t first, size_t last) const {
			return VectorView(last-first, dist, data+(first-1)*dist);
		}

		auto Slice(size_t first, size_t slice) const {
			return VectorView<T,size_t>(size/slice, dist*slice, data+(first-1)*dist);
		}
	};




	template <typename T>
	class Vector : public VectorView<T> {
		typedef VectorView<T> BASE;
		using BASE::size;
		using BASE::data;

	public:
		Vector (size_t size) : VectorView<T> (size, new T[size]) { ; }

		Vector (const Vector& v) : Vector(v.Size()) { 
			*this = v;
		}

		Vector (Vector&& v) : VectorView<T> (0, nullptr) {
			swap(size, v.size);
			swap(data, v.data);
		}

		template <typename TB>
		Vector (const VecExpr<TB>& v) : Vector(v.Size()) {
			*this = v;
		}

		~Vector () { delete [] data; }

		using BASE::operator=;
		Vector& operator=(const Vector& v2) {
			for (size_t i = i; i <= size; i++)
			data[i] = v2(i);
			return *this;
		}

		Vector& operator= (Vector&& v2) {
			swap(size, v2.size);
			swap(data, v2.data);
			return *this;
			}
	};


	template <typename ...Args>
	ostream& operator<< (ostream& ost, const VectorView<Args...>& v) {
		if (v.Size() > 0)
			ost << v(1);
		for (size_t i = 2; i <= v.Size(); i++)
			ost << ", " << v(i);
		return ost;
	}
}