#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <vector.hpp>
using namespace ASC_bla;


TEST_CASE( "bla-tests", "[Vector]" ) {
  Vector<double> x(5);
  Vector<double> y = x;
  REQUIRE(y.size() == 5);
}

TEST_CASE( "bla-tests", "[vector size check]" ) {
  Vector<double> x(5);
  x = 3;
  Vector<double> y = x + x + (-2) * x + x;
  REQUIRE(y(0) == 3); 
}