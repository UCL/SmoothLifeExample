#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "smooth.h"

TEST_CASE( "Smooth model can be instantiated and configured", "[Smooth]" ) {

    SECTION( "Smooth can be constructed" ) {
        Smooth smooth;
        REQUIRE (smooth.Size() == 100);
    }
}
