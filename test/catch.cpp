#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "smooth.h"
#include <cmath>

TEST_CASE( "Smooth model can be instantiated and configured", "[Smooth]" ) {

    SECTION( "Smooth can be constructed" ) {
        Smooth smooth;
        REQUIRE (smooth.Size() == 100);
        REQUIRE (smooth.Field().size() == 100);
        REQUIRE (smooth.Field()[0].size() == 100);
    }
}

TEST_CASE( "Smooth mathematical functions are correct","[Smooth]") {
  Smooth smooth;
  SECTION( "Disk support function is correct") {
      REQUIRE (smooth.Disk(500) == 0.0);
      REQUIRE (smooth.Disk(21.6) == 0.0);
      REQUIRE (smooth.Disk(21.4) > 0.0);
      REQUIRE (smooth.Disk(21.4) < 1.0);
      REQUIRE (smooth.Disk(20.6) > 0.0);
      REQUIRE (smooth.Disk(20.6) < 1.0);
      REQUIRE (smooth.Disk(20.4) == 1.0);
      REQUIRE (smooth.Disk(19.0) == 1.0);
      REQUIRE (smooth.Disk(21.0) ==0.5);
    }
  SECTION ("Ring support function is correct") {
      REQUIRE (smooth.Ring(22) == 1.0);
      REQUIRE (smooth.Ring(21.6) == 1.0);
      REQUIRE (smooth.Ring(21.4) > 0.0);
      REQUIRE (smooth.Ring(21.4) < 1.0);
      REQUIRE (smooth.Ring(20.6) > 0.0);
      REQUIRE (smooth.Ring(20.6) < 1.0);
      REQUIRE (smooth.Ring(20.4) == 0.0);
      REQUIRE (smooth.Ring(21.0) ==0.5);
      REQUIRE (smooth.Ring(64.0) == 0.0);
      REQUIRE (smooth.Ring(63.6) == 0.0);
      REQUIRE (smooth.Ring(63.4) > 0.0);
      REQUIRE (smooth.Ring(63.4) < 1.0);
      REQUIRE (smooth.Ring(62.6) > 0.0);
      REQUIRE (smooth.Ring(62.6) < 1.0);
      REQUIRE (smooth.Ring(62.4) == 1.0);
      REQUIRE (smooth.Ring(63.0) == 0.5);
  }
  SECTION ("Sigmoid function is correct") {
    double e=std::exp(1.0);
    REQUIRE(Smooth::Sigmoid(1.0,1.0,4.0) == 0.5);
    REQUIRE(std::abs(Smooth::Sigmoid(0.0,1.0,4.0)-e/(1+e))<0.0001);
  }
  SECTION ("Wraparound Distance is correct") {
    REQUIRE(smooth.TorusDifference(95,5) == 10);
    REQUIRE(smooth.TorusDifference(5,96) == 9);
    REQUIRE(smooth.TorusDifference(5,10) == 5);
    REQUIRE(smooth.Radius(10,10,13,14)==5.0);
  }
  SECTION ("Disk Normalisation is correct") {
    REQUIRE(std::abs(smooth.NormalisationDisk()-1385.7827)<0.1);
  }
}
