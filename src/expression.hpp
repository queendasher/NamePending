#ifndef FILE_EXPRESSION
#define FILE_EXPRESSION

namespace Mathlib
{
	template<typename E>
	class MatExpr {
	public:
		auto Downcast() const { return static_cast<const E&>(*this); }
		size_t Rows() const { return Downcast().Rows(); }
		size_t Cols() const { return Downcast().Cols(); }
		auto Row(size_t r) const { return Downcast().Row(r); }
		auto Col(size_t c) const { return Downcast().Col(c); }
		auto operator()(size_t r, size_t c) const { return Downcast()(r, c); }
	};	

	template<typename E>
	class VecExpr {
	public:
		auto Downcast() const { return static_cast<const E&>(*this); }
		size_t Size() const { return Downcast().Size(); }
		auto operator()(size_t i) const { return Downcast()(i); }
	};





	/*
	// Vector expressions
	*/


	// Addition
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


	// Subtraction
	template<typename E1, typename E2>
	class VecExprSub : public VecExpr<VecExprSub<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		VecExprSub(E1 _a, E2 _b) : a(_a), b(_b) {
			if (a.Size() != b.Size()) throw runtime_error("Vector sizes do not match in subtraction.");
		}

		auto operator()(size_t i) const { return a(i) - b(i); }
		size_t Size() const { return a.Size(); }
	};

	template <typename E1, typename E2>
	auto operator-(const VecExpr<E1>& a, const VecExpr<E2>& b) {
		return VecExprSub(a.Downcast(), b.Downcast());
	}


	// Unary negation
	template<typename E>
	class VecExprNeg : public VecExpr<VecExprNeg<E>> {
		E vec;

	public:
		VecExprNeg(E _vec) : vec(_vec) { }
		auto operator()(size_t i) const { return -vec(i); }
		size_t Size() const { return vec.Size(); }
	};

	template <typename E>
	auto operator-(const VecExpr<E>& v) {
		return VecExprNeg(v.Downcast());
	}


	// Elementwise multiplication
	template<typename E1, typename E2>
	class VecExprMul : public VecExpr<VecExprMul<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		VecExprMul(E1 _a, E2 _b) : a(_a), b(_b) {
			if (a.Size() != b.Size()) throw runtime_error("Vector sizes do not match in multiplication.");
		}

		auto operator()(size_t i) const { return a(i) * b(i); }
		size_t Size() const { return a.Size(); }
	};

	template <typename E1, typename E2>
	auto VecMul(const VecExpr<E1>& a, const VecExpr<E2>& b) {
		return VecExprMul(a.Downcast(), b.Downcast());
	}


	// Dot product
	template <typename E1, typename E2>
    auto Dot(const VecExpr<E1>& a, const VecExpr<E2>& b) {
        if (a.Size() != b.Size()) throw runtime_error("Vector sizes do not match for dot product");
        using SumType = decay_t<decltype(a(0) * b(0))>;
        SumType result(0);
        for (size_t i = 0; i < a.Size(); ++i) {
			result = result + a(i) * b(i);
		}
        return result;
    }


	// Matrix-vector multiplication
	template<typename EM, typename EV>
	class VecExprMulMatFromL : public VecExpr<VecExprMulMatFromL<EM, EV>> {
		EM mat; // left operand
		EV vec; // right operand

	public:
		VecExprMulMatFromL(EM _mat, EV _vec) : mat(_mat), vec(_vec) { }

		auto operator()(size_t i) const { return Dot(mat.Row(i), vec); }
		size_t Size() const { return mat.Rows(); }
	};

	template <typename E1, typename E2>
	auto operator*(const MatExpr<E1>& m, const VecExpr<E2>& v) {
		return VecExprMulMatFromL(m.Downcast(), v.Downcast());
	}


	// Scalar multiplication from left
	template<typename TSCAL, typename EV>
	class VecExprScaleL : public VecExpr<VecExprScaleL<TSCAL, EV>> {
		TSCAL scal; // scalar
		EV vec; // vector

	public:
		VecExprScaleL(TSCAL _scal, EV _vec) : scal(_scal), vec(_vec) { }

		auto operator()(size_t i) const { return scal * vec(i); }
		size_t Size() const { return vec.Size(); }      
	};

	template<typename TSCAL, typename EV>
	auto operator*(const TSCAL scal, const VecExpr<EV>& v) {
		return VecExprScaleL(scal, v.Downcast());
	}


	// Output
	template<typename E>
	ostream& operator<<(ostream& os, const VecExpr<E>& v) {
		if (v.Size() > 0)
			os << v(0);
		for (size_t i = 1; i < v.Size(); ++i)
			os << ", " << v(i);
		return os;
	}

	



	/*
	// Matrix expressions
	*/


	// Addition 
	template<typename E1, typename E2>
	class MatExprSum : public MatExpr<MatExprSum<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		MatExprSum(E1 _a, E2 _b) : a(_a), b(_b) { 
			if (a.Rows() != b.Rows() || a.Cols() != b.Cols()) 
				throw runtime_error("Matrix sizes do not match in sum.");
		}

		auto operator()(size_t r, size_t c) const { return a(r, c) + b(r, c); }
		size_t Rows() const { return a.Rows(); }
		size_t Cols() const { return a.Cols(); }
		auto Row(size_t r) const { return a.Row(r) + b.Row(r); }
		auto Col(size_t c) const { return a.Col(c) + b.Col(c); }
	};

	template <typename E1, typename E2>
	auto operator+(const MatExpr<E1>& a, const MatExpr<E2>& b) {
		return MatExprSum(a.Downcast(), b.Downcast());
	}


	// Subtraction
	template<typename E1, typename E2>
	class MatExprSub : public MatExpr<MatExprSub<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		MatExprSub(E1 _a, E2 _b) : a(_a), b(_b) { 
			if (a.Rows() != b.Rows() || a.Cols() != b.Cols()) 
				throw runtime_error("Matrix sizes do not match in subtraction.");
		}

		auto operator()(size_t r, size_t c) const { return a(r, c) - b(r, c); }
		size_t Rows() const { return a.Rows(); }
		size_t Cols() const { return a.Cols(); }
		auto Row(size_t r) const { return a.Row(r) - b.Row(r); }
		auto Col(size_t c) const { return a.Col(c) - b.Col(c); }
	};

	template <typename E1, typename E2>
	auto operator-(const MatExpr<E1>& a, const MatExpr<E2>& b) {
		return MatExprSub(a.Downcast(), b.Downcast());
	}


	// Unary negation
	template<typename E>
	class MatExprNeg : public MatExpr<MatExprNeg<E>> {
		E mat;

	public:
		MatExprNeg(E _mat) : mat(_mat) { }

		auto operator()(size_t r, size_t c) const { return -mat(r, c); }
		size_t Rows() const { return mat.Rows(); }
		size_t Cols() const { return mat.Cols(); }
		auto Row(size_t r) const { return -mat.Row(r); }
		auto Col(size_t c) const { return -mat.Col(c); }
	};

	template <typename E>
	auto operator-(const MatExpr<E>& m) {
		return MatExprNeg(m.Downcast());
	}


	// Elementwise multiplication
	template<typename E1, typename E2>
	class MatExprElemMul : public MatExpr<MatExprElemMul<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		MatExprElemMul(E1 _a, E2 _b) : a(_a), b(_b) { 
			if (a.Rows() != b.Rows() || a.Cols() != b.Cols()) 
				throw runtime_error("Matrix sizes do not match in elementwise multiplication.");
		}

		auto operator()(size_t r, size_t c) const { return a(r, c) * b(r, c); }
		size_t Rows() const { return a.Rows(); }
		size_t Cols() const { return a.Cols(); }
		auto Row(size_t r) const { return VecMul(a.Row(r), b.Row(r)); }
		auto Col(size_t c) const { return VecMul(a.Col(c), b.Col(c)); }
	};
	
	template <typename E1, typename E2>
	auto MatMul(const MatExpr<E1>& a, const MatExpr<E2>& b) {
		return MatExprMul(a.Downcast(), b.Downcast());
	}


	// Matrix multiplication
	template<typename E1, typename E2>
	class MatExprMul : public MatExpr<MatExprMul<E1, E2>> {
		E1 a; // left operand
		E2 b; // right operand

	public:
		MatExprMul(E1 _a, E2 _b) : a(_a), b(_b) { 
			if (a.Cols() != b.Rows()) 
				throw runtime_error("Matrix sizes do not match in multiplication.");
		}

		// This is actually horribly inefficient for chained multiplications.
		// If a (or b) is a MatExprMul, each a(r,k) is its own dot product which
		// recursively recomputes the dot products for all of its children.
		// For a chain of m multiplications, the cost of this implementation is O(n^m)
		// whereas the simpler approach with temporaries would be O(m*n^3).
		// Avoid inlining matrix multiplications, create temporaries instead.
		auto operator()(size_t r, size_t c) const {
			return Dot(a.Row(r), b.Col(c));
		}

		size_t Rows() const { return a.Rows(); }
		size_t Cols() const { return b.Cols(); }
		auto Row(size_t r) const { return a.Row(r) * b; }
		auto Col(size_t c) const { return a * b.Col(c); }

	};

	template <typename E1, typename E2>
	auto operator*(const MatExpr<E1>& a, const MatExpr<E2>& b) {
		return MatExprMul(a.Downcast(), b.Downcast());
	}


	// Scalar multiplication from left
	template<typename TSCAL, typename EM>
	class MatExprScaleL : public MatExpr<MatExprScaleL<TSCAL, EM>> {
		TSCAL scal; // scalar
		EM mat; // matrix

	public:
		MatExprScaleL(TSCAL _scal, EM _mat) : scal(_scal), mat(_mat) { }
		auto operator()(size_t r, size_t c) const { return scal * mat(r, c); }
		size_t Rows() const { return mat.Rows(); }
		size_t Cols() const { return mat.Cols(); }
		auto Row(size_t r) const { return scal * mat.Row(r); }
		auto Col(size_t c) const { return scal * mat.Col(c); }
	};

	template<typename TSCAL, typename E>
	auto operator*(const TSCAL scal, const MatExpr<E>& m) {
		return MatExprScaleL(scal, m.Downcast());
	}


	// Output
	template<typename E>
	ostream& operator<<(ostream& os, const MatExpr<E>& m) {
		for (size_t i = 0; i < m.Rows(); ++i) {
			for (size_t j = 0; j < m.Cols(); ++j)
				os << m(i, j) << " ";
			os << "\n";
		}
		return os;
	}
}
 
#endif
