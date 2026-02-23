#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "pandemic.hpp"

#include "doctest.h"

TEST_CASE("Nessun infetto") {
  pandemic::World start(2, 0.5, 0.5);
  start.person(1, 1) = pandemic::Person::Infected;
  CHECK(start.person(1, 1) == pandemic::Person::Infected);
  start.person(1, 1) = pandemic::Person::Susceptible;
  CHECK(start.person(1, 1) == pandemic::Person::Susceptible);
  start.person(1, 2) = pandemic::Person::Susceptible;
  start.person(2, 1) = pandemic::Person::Susceptible;
  start.person(2, 2) = pandemic::Person::Susceptible;

  auto const next = pandemic::spread(start,false);
  CHECK(next == start);
}

TEST_CASE("Parametri impostati a 1") {
  pandemic::World start(5, 1, 1);
  start.person(3, 3) = pandemic::Person::Infected;

  auto next = spread(start,false);

  pandemic::World expected(5, 1, 1);
  expected.person(3, 3) = pandemic::Person::Removed;
  expected.person(2, 2) = pandemic::Person::Infected;
  expected.person(2, 3) = pandemic::Person::Infected;
  expected.person(2, 4) = pandemic::Person::Infected;
  expected.person(3, 2) = pandemic::Person::Infected;
  expected.person(3, 4) = pandemic::Person::Infected;
  expected.person(4, 2) = pandemic::Person::Infected;
  expected.person(4, 3) = pandemic::Person::Infected;
  expected.person(4, 4) = pandemic::Person::Infected;

  CHECK(next == expected);
}

TEST_CASE("Grid shape test") {
  pandemic::World start(3, 1, 1);
  //start.person(4, 4) = pandemic::Person::Infected;
  start.person(5, 5) = pandemic::Person::Infected;
  //start.person(6, 6) = pandemic::Person::Infected;
  start.person(0, 0) = pandemic::Person::Infected;
  pandemic::World expected(3, 1, 1);
  expected.person(1,1) = pandemic::Person::Infected;
  expected.person(2,2) = pandemic::Person::Infected;
  //expected.person(3,3) = pandemic::Person::Infected;

  CHECK(expected == start);
}
