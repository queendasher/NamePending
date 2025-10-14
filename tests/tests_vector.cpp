#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <vector.hpp>
using namespace ASC_bla;

/*
uint32_t factorial(uint32_t number)
{
  return number <= 1 ? number : factorial(number - 1) * number;
}
*/

TEST_CASE( "bla-tests", "[Vector]" ) {
  Vector<double> x(5);
  REQUIRE(x.size() == 5);
}
