#ifndef PANDEMIC_WORLD_HPP
#define PANDEMIC_WORLD_HPP

#include <cassert>
#include <random>
#include <vector>

using namespace std;

namespace pandemic {

enum class Person : char { Susceptible, Infected, Removed };

class World {
  using Grid = vector<Person>;
  int side;
  Grid grid;
  double beta;
  double gamma;

 public:
  World(int n, double beta_, double gamma_);
  int Get_side() const;
  double Get_beta() const;
  double Get_gamma() const;
  Person const& person(int r, int c) const noexcept;
  Person& person(int r, int c) noexcept;
  friend bool operator==(World const& l, World const& r) {
    return l.grid == r.grid;
  }
  friend bool operator!=(World const& l, World const& r) {
   return !(l == r);

  }
  

};

inline bool infection(World const& world, int r, int c) {
  bool result = false;        
  default_random_engine eng{random_device{}()};
  uniform_real_distribution<double> dist{0, 1};
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if ((world.person(r + i, c + j) == pandemic::Person::Infected) &&
          (world.person(r, c) == pandemic::Person::Susceptible)) {

        auto chance = dist(eng);
        if (chance < world.Get_beta()) {
          result = true;
        }
      }
    }
  }
  return result;
}

inline bool removal(World const& world, int r, int c) {
  bool result = false;
  default_random_engine eng{random_device{}()};
  uniform_real_distribution<double> dist{0, 1};
  if (world.person(r, c) == pandemic::Person::Infected) {
    
    auto chance = dist(eng);
    if (chance < world.Get_gamma()) {
      result = true;
    }
  }

  return result;
}

inline World spread(World const& current, bool ld) {
  int const N = current.Get_side();

  World next(N, current.Get_beta(), current.Get_gamma());

  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      bool c = infection(current, i, j);
      bool r = removal(current, i, j);

      if ((c == true) && (current.person(i, j) == Person::Susceptible) && (ld == false)) {
        next.person(i, j) = Person::Infected;
      }

      if ((r == true) && (current.person(i, j) == Person::Infected)) {
        next.person(i, j) = Person::Removed;
      }

      if (((c == false) && (current.person(i, j) == Person::Susceptible)) ||
          (current.person(i, j) == Person::Removed) ||
          ((r == false) && current.person(i, j) == Person::Infected)) {
        next.person(i, j) = current.person(i, j);
      }
    }
  }

  return next;
}

inline int count_Susceptibles(World const& current) {
  int const N = current.Get_side();
  int S = 0;
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      if (current.person(i, j) == Person::Susceptible) {
        ++S;
      }
    }
  }
  return S;
}

inline int count_Infected(World const& current) {
  int const N = current.Get_side();
  int I = 0;
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      if (current.person(i, j) == Person::Infected) {
        ++I;
      }
    }
  }
  return I;
}

inline int count_Removed(World const& current) {
  int const N = current.Get_side();
  int R = 0;
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      if (current.person(i, j) == Person::Removed) {
        ++R;
      }
    }
  }
  return R;
}
}  // namespace pandemic

#endif