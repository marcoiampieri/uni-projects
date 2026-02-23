#include <chrono>
#include <thread>

#include "pandemic.hpp"
#include "setting_functions.hpp"

using namespace pandemic;

void print(ostream& os, World const& world) {
  const auto N = world.Get_side();

  os << '+' << string(N, '-') << "+\n";
  for (int r = 0; r != N; ++r) {
    os << '|';
    for (int c = 0; c != N; ++c) {
      if (world.person(r, c) == Person::Susceptible) {
        os << '@';
      }

      if (world.person(r, c) == Person::Infected) {
        os << '*';
      }

      if (world.person(r, c) == Person::Removed) {
        os << ' ';
      }
    }
    os << "|\n";
  }
  os << '+' << string(N, '-') << "+\n";
}

int main() {
  const int Population = Set_Population();
  const int world_size = rint(sqrt(Population));
  int Infected = Set_Infected();
  if (Infected > Population) {
    Infected = Population;
  }
  const int Duration = Set_pandemic_duration();
  const double beta = Set_beta();
  const double gamma = Set_gamma();
  const bool lockdown = Lockdown();

  World world(world_size, beta, gamma);

  default_random_engine eng{random_device{}()};
  uniform_int_distribution<int> dist{0, world_size - 1};

  for (int i = 0; i != Infected; ++i) {
    auto r = dist(eng);
    auto c = dist(eng);
    for (; world.person(r, c) == Person::Infected; r = dist(eng), c = dist(eng))
      ;
    world.person(r, c) = Person::Infected;
  }

  cout << "\n"
       << "Giorno 0 (inizio) : "
       << "\n";
  cout << "Numero di suscettibili : " << count_Susceptibles(world) << "\n";
  cout << "Numero di infetti : " << count_Infected(world) << "\n";
  cout << "Numero di rimossi : " << count_Removed(world) << "\n";
  print(cout, world);
  this_thread::sleep_for(chrono::milliseconds(2000));

  for (int i = 0; i != Duration; ++i) {
    cout << "\n"
         << "Giorno " << i + 1 << " :"
         << "\n";
    bool ld = Check_lockdown_requirements(lockdown, count_Infected(world),
                                          Population);
    world = spread(world, ld);
    cout << "Numero di suscettibili : " << count_Susceptibles(world) << "\n";
    cout << "Numero di infetti : " << count_Infected(world) << "\n";
    cout << "Numero di rimossi : " << count_Removed(world) << "\n";

    print(cout, world);
    this_thread::sleep_for(chrono::milliseconds(2000));
  }
}