#ifndef SETTING_HPP
#define SETTING_HPP

#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int Set_Population();
int Set_Infected();
int Set_pandemic_duration();
double Set_beta();
double Set_gamma();
bool Chek_countermeasures();
double Set_countermeasures_effectiveness();
bool Chek_treatments();
double Set_treatments_effectiveness();
bool Lockdown();
bool Check_lockdown_requirements(bool lockdown, int a, int b);

#endif