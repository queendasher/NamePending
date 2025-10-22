#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/vector.hpp"
using namespace Mathlib;



TEST_CASE( "construction, copy, move, assignment" ) {
	Vector<int> v1(5);          // Construct a vector of size 5
	for (size_t i = 0; i < v1.Size(); ++i)
		v1(i) = static_cast<int>(i + 1); // Initialize v1 with values 1, 2, 3, 4, 5

	Vector<int> v2 = v1;        // Copy constructor
	REQUIRE(v2.Size() == 5);
	for (size_t i = 0; i < v2.Size(); ++i)
		REQUIRE(v2(i) == static_cast<int>(i + 1)); // Check copied values

	Vector<int> v3(3);         // Another vector of size 3
	v3 = v1;                    // Copy assignment
	REQUIRE(v3.Size() == 5);
	for (size_t i = 0; i < v3.Size(); ++i)
		REQUIRE(v3(i) == static_cast<int>(i + 1)); // Check assigned values

	Vector<int> v4 = std::move(v1); // Move constructor
	REQUIRE(v4.Size() == 5);
	for (size_t i = 0; i < v4.Size(); ++i)
		REQUIRE(v4(i) == static_cast<int>(i + 1)); // Check moved values

	Vector<int> v5(2);         // Another vector of size 2
	v5 = std::move(v2);        // Move assignment
	REQUIRE(v5.Size() == 5);
	for (size_t i = 0; i < v5.Size(); ++i)
		REQUIRE(v5(i) == static_cast<int>(i + 1)); // Check moved values

	// Ensure moved-from vectors are in a valid state (size should be 0)
	REQUIRE(v1.Size() == 0);
	REQUIRE(v2.Size() == 0);

	// Self-assignment (copy)
	v3 = v3;
	REQUIRE(v3.Size() == 5);
	for (size_t i = 0; i < v3.Size(); ++i)
		REQUIRE(v3(i) == static_cast<int>(i + 1)); // Check values remain unchanged

	// Self-assignment (move)
	v4 = std::move(v4);
	REQUIRE(v4.Size() == 5);
	for (size_t i = 0; i < v4.Size(); ++i)
		REQUIRE(v4(i) == static_cast<int>(i + 1)); // Check values remain unchanged

	// Scalar assignment
	v5 = 42;
	REQUIRE(v5.Size() == 5);
	for (size_t i = 0; i < v5.Size(); ++i)
		REQUIRE(v5(i) == 42); // All elements should be 42
}



TEST_CASE( "expression templates" ) {
	Vector<int> v1(5);
	for (size_t i = 0; i < v1.Size(); ++i)
		v1(i) = static_cast<int>(i); // v1 = [0, 1, 2, 3, 4]

	Vector<int> v2(5);
	for (size_t i = 0; i < v2.Size(); ++i)
		v2(i) = static_cast<int>(i + 10); // v2 = [10, 11, 12, 13, 14]

	Vector<int> v3(5);
	v3 = v1 + v2; // Test vector addition
	REQUIRE(v3.Size() == 5);
	for (size_t i = 0; i < v3.Size(); ++i)
		REQUIRE(v3(i) == static_cast<int>(2 * i + 10)); // v3 should be [10, 12, 14, 16, 18]

	Vector<int> v4(5);
	v4 = v2 - v1; // Test vector subtraction
	REQUIRE(v4.Size() == 5);
	for (size_t i = 0; i < v4.Size(); ++i)
		REQUIRE(v4(i) == static_cast<int>(10)); // v4 should be [10, 10, 10, 10, 10]

	Vector<int> v5(5);
	v5 = 2 * v1; // Test scalar multiplication
	REQUIRE(v5.Size() == 5);
	for (size_t i = 0; i < v5.Size(); ++i)
		REQUIRE(v5(i) == static_cast<int>(2 * i)); // v5 should be [0, 2, 4, 6, 8]

	Vector<int> v6(5);
	v6 = -v1; // Test unary negation
	REQUIRE(v6.Size() == 5);
	for (size_t i = 0; i < v6.Size(); ++i)
		REQUIRE(v6(i) == static_cast<int>(-i)); // v6 should be [0, -1, -2, -3, -4]

	int dot_product = Dot(v1, v2); // Test dot product
	REQUIRE(dot_product == 130); // 0*10 + 1*11 + 2*12 + 3*13 + 4*14 = 130

	Vector<int> v7(5);
	v7 = VecMul(v1, v2); // Test element-wise multiplication
	REQUIRE(v7.Size() == 5);
	for (size_t i = 0; i < v7.Size(); ++i)
		REQUIRE(v7(i) == static_cast<int>(i * (i + 10))); // v7 should be [0, 11, 24, 39, 56]

	Vector<int> v8(5);
	v8 = 3 * VecMul((v1 + v2), (v2 - v1)) + (-v1); // Test combined expression
	REQUIRE(v8.Size() == 5);
	for (size_t i = 0; i < v8.Size(); ++i)
		REQUIRE(v8(i) == static_cast<int>(300 + i * 59)); // v8 should be [300, 359, 418, 477, 536]
}



TEST_CASE( "views" ) {
	Vector<int> v1(10);
	for (size_t i = 0; i < v1.Size(); ++i)
		v1(i) = static_cast<int>(i); // v1 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	auto subview = v1.Range(2, 7); // subview = [2, 3, 4, 5, 6]
	REQUIRE(subview.Size() == 5);
	for (size_t i = 0; i < subview.Size(); ++i)
		REQUIRE(subview(i) == static_cast<int>(i + 2));

	auto slice = v1.Slice(1, 2); // slice = [1, 3, 5, 7, 9]
	REQUIRE(slice.Size() == 5);
	for (size_t i = 0; i < slice.Size(); ++i)
		REQUIRE(slice(i) == static_cast<int>(1 + i * 2));

	Vector<int> v2(5);
	v2 = subview + slice; // v2 should be [3, 6, 9, 12, 15]
	REQUIRE(v2.Size() == 5);
	for (size_t i = 0; i < v2.Size(); ++i)
		REQUIRE(v2(i) == static_cast<int>(3 + i * 3));

	// Modify original vector and check views reflect changes
	for (size_t i = 0; i < v1.Size(); ++i)
		v1(i) = static_cast<int>(i * 10); // v1 = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90]
	REQUIRE(subview(0) == 20);
	REQUIRE(slice(0) == 10);
}



TEST_CASE( "error handling" ) {
	Vector<int> v1(5);
	Vector<int> v2(3);

	// Bad malloc
	REQUIRE_THROWS_AS(Vector<int>(SIZE_MAX), std::runtime_error);

	// Mismatched sizes for addition
	REQUIRE_THROWS_AS(v1 + v2, std::runtime_error);

	// Mismatched sizes for subtraction
	REQUIRE_THROWS_AS(v1 - v2, std::runtime_error);

	// Mismatched sizes for element-wise multiplication
	REQUIRE_THROWS_AS(VecMul(v1, v2), std::runtime_error);

	// Mismatched sizes for dot product
	REQUIRE_THROWS_AS(Dot(v1, v2), std::runtime_error);

	// Out of range access
	REQUIRE_THROWS_AS(v1(5), std::out_of_range);
	REQUIRE_THROWS_AS(v1(100), std::out_of_range);
	REQUIRE_THROWS_AS(v1(-1), std::out_of_range);

	// Invalid range for subview
	REQUIRE_THROWS_AS(v1.Range(4, 2), std::out_of_range);
	REQUIRE_THROWS_AS(v1.Range(0, 6), std::out_of_range);

	// Invalid slice
	REQUIRE_THROWS_AS(v1.Slice(5, 2), std::out_of_range);
	REQUIRE_THROWS_AS(v1.Slice(2, 0), std::invalid_argument);
}