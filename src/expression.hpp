#ifndef FILE_EXPRESSION
#define FILE_EXPRESSION

namespace Mathlib
{
	template <typename E>
	class VecExpr {
	public:
		auto Downcast() const { return static_cast<const E&>(*this); }
		size_t Size() const { return Downcast().Size(); }
		auto operator()(size_t i) const { return Downcast()(i); }
	};
	


	template<typename E1, typename E2>
	class VecExprSum : public VecExpr<VecExprSum<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		VecExprSum(E1 _a, E2 _b) : a(_a), b(_b) { 
			if (a.Size() != b.Size()) throw runtime_error("Vector sizes do not match in sum.");
		}

		auto operator()(size_t i) const { return a(i) + b(i); }
		size_t Size() const { return a.Size(); }      
	};
	
	template <typename E1, typename E2>
	auto operator+(const VecExpr<E1>& a, const VecExpr<E2>& b) {
		return VecExprSum(a.Downcast(), b.Downcast());
	}



	
	template<typename ESCAL, typename EV>
	class VecExprScale : public VecExpr<VecExprScale<ESCAL, EV>> {
		ESCAL scal; // scalar
		EV vec; // vector

	public:
		VecExprScale(ESCAL _scal, EV _vec) : scal(_scal), vec(_vec) { }

		auto operator()(size_t i) const { return scal * vec(i); }
		size_t Size() const { return vec.Size(); }      
	};
	
	template <typename E>
	auto operator*(double scal, const VecExpr<E>& v) {
		return VecExprScale(scal, v.Downcast());
	}


	
	template <typename E>
	ostream& operator<<(ostream& os, const VecExpr<E>& v) {
		if (v.Size() > 0)
			os << v(0);
		for (size_t i = 1; i < v.Size(); ++i)
			os << ", " << v(i);
		return os;
	}
}
 
#endif
