#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "../src/vector.hpp"
#include "../src/matrix.hpp"
using namespace Mathlib;
using namespace Catch;

TEST_CASE( "vector performance" ) {
	const size_t N = 1e7;
	Vector<double> v1(N);
	Vector<double> v2(N);

	// Initialize vectors
	for (size_t i = 0; i < N; ++i) {
		v1(i) = static_cast<double>(i) * 0.5;
		v2(i) = static_cast<double>(i) * 2.0;
	}

	BENCHMARK("Vector addition") {
		Vector<double> v3 = v1 + v2;
		return v3;
	};

	BENCHMARK("Scalar multiplication") {
		Vector<double> v5 = 3.14 * v1;
		return v5;
	};

	BENCHMARK("Dot product") {
		double dot_product = Dot(v1, v2);
		return dot_product;
	};
}