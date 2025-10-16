#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/vector.hpp"
using namespace Mathlib;



TEST_CASE( "construction, copy, assignment, destruction" ) {
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
}

TEST_CASE( "vector-test 2", "[vector add and scale test]" ) {

}