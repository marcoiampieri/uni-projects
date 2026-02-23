#include "pandemic.hpp"
#include <algorithm>
using namespace pandemic;

World::World(int n,int Infected, double beta_, double gamma_)
    : side(n),
      grid(side * side, Person::Susceptible),
      beta(beta_),
      gamma(gamma_) {
        std::fill_n(grid.begin(), Infected, Person::Infected);
        std::random_shuffle(grid.begin(), grid.end(),std::default_random_engine {random_device{}()});
      }

int World::Get_side() const { return side; }

double World::Get_beta() const { return beta; }

double World::Get_gamma() const { return gamma; }

Person const& World::person(int r, int c) const noexcept {
  auto const i = (r + side) % side;
  auto const j = (c + side) % side;
  assert(i >= 0 && i < side && j >= 0 && j < side);
  auto const index = i * side + j;
  assert(index >= 0 && index < static_cast<int>(grid.size()));
  return grid[index];
}

Person& World::person(int r, int c) noexcept {
  auto const i = (r + side) % side;
  auto const j = (c + side) % side;
  assert(i >= 0 && i < side && j >= 0 && j < side);
  auto const index = i * side + j;
  assert(index >= 0 && index < static_cast<int>(grid.size()));
  return grid[index];
}
