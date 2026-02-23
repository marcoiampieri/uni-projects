#include "setting_functions.hpp"

int Set_Population() {
  double in;
  cout << "Dimensione (indicativa) della popolazione :" << endl;
  cin >> in;
  int N = rint(in);
  if ((N < 0) || (in != N)) {
    throw runtime_error{"Il numero di persone dev'essere naturale"};
  }
  return N;
}

int Set_Infected() {
  double in;
  cout << "Numero infetti :" << endl;
  cin >> in;
  int I = rint(in);
  if ((I < 0) || (in != I)) {
    throw runtime_error{"Il numero di infetti dev'essere naturale"};
  }
  return I;
}

int Set_pandemic_duration() {
  double in;
  cout << "Durata epidemia (giorni) :" << endl;
  cin >> in;
  int T = rint(in);
  if ((T < 0) || (in != T)) {
    throw runtime_error{"Il numero di giorni dev'essere naturale"};
  }
  return T;
}

bool Chek_countermeasures() {
  string ans;
  const string y = "sì";
  const string n = "no";
  cout << "Sono presenti misure per prevenire il contagio?" << '\n';
  cout << "(e.g. utilizzo obbligatorio di mascherine, utilizzo di "
          "disinfettanti, etc.)"
       << '\n';
  cout << "(sì/no) :" << endl;
  cin >> ans;
  if (ans == y) {
    return true;
  }
  if (ans == n) {
    return false;
  } else {
    throw runtime_error{"Input non valido"};
  }
}

double Set_countermeasures_effectiveness() {
  bool cm = Chek_countermeasures();

  double eff;

  if (cm == true) {
    cout << "Quanto sono efficaci queste misure?" << '\n';
    cout << "(Di quanto riducono la possibilità di contagio?)" << '\n';
    cout << "(range : 5%~50%) :" << endl;
    cin >> eff;
    if ((eff < 5) || (eff > 50)) {
      throw runtime_error{"Input non valido"};
    }
    return eff;
  }

  else {
    return 0;
  }
}

double Set_beta() {
  double beta;
  cout << "Indice di trasmissione (parametro beta) :" << endl;
  cin >> beta;
  if ((beta < 0) || (beta > 1)) {
    throw runtime_error{"Beta dev'essere compreso tra 0 e 1"};
  }
  beta = ((100 - Set_countermeasures_effectiveness()) / 100) * beta;
  return beta;
}

bool Chek_treatments() {
  string ans;
  const string y = "sì";
  const string n = "no";
  cout << "Sono presenti misure per trattare gli infetti?" << '\n';
  cout << "(e.g. farmaci specifici, posti in terapia intensiva, etc.)" << '\n';
  cout << "(sì/no) :" << endl;
  cin >> ans;
  if (ans == y) {
    return true;
  }
  if (ans == n) {
    return false;
  } else {
    throw runtime_error{"Input non valido"};
  }
}

double Set_treatments_effectiveness() {
  bool treat = Chek_treatments();

  double eff;

  if (treat == true) {
    cout << "Quanto sono efficaci queste misure?" << '\n';
    cout << "(Di quanto aumentano la possibilità di guarigione?)" << '\n';
    cout << "(range : 5%~50%) :" << endl;
    cin >> eff;
    if ((eff < 5) || (eff > 50)) {
      throw runtime_error{"Input non valido"};
    }
    return eff;
  }

  else {
    return 0;
  }
}

double Set_gamma() {
  double gamma;
  cout << "Indice di rimozione (parametro gamma) :" << endl;
  cin >> gamma;
  if ((gamma < 0) || (gamma > 1)) {
    throw runtime_error{"Gamma dev'essere compreso tra 0 e 1"};
  }
  gamma = ((100 + Set_treatments_effectiveness()) / 100) * gamma;
  return gamma;
}

bool Lockdown() {
  string ans;
  const string y = "sì";
  const string n = "no";
  cout << "Sono previsti lockdown?" << '\n';
  cout << "(la misura si attiverebbe per un rapporto infetti/popolazione > 10%)"
       << '\n';
  cout << "(sì/no) :" << endl;
  cin >> ans;
  if (ans == y) {
    return true;
  }
  if (ans == n) {
    return false;
  } else {
    throw runtime_error{"Input non valido"};
  }
}

bool Check_lockdown_requirements(bool lockdown, int a, int b) {
  bool ld;
  if (lockdown == true) {
    if (a > b / 10) {
      ld = true;
    } else {
      ld = false;
    }
  } else {
    ld = false;
  }
  return ld;
}
