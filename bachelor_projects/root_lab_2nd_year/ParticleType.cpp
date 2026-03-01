#include "ParticleType.hpp"
ParticleType::ParticleType (const char* name_,double mass_, int charge_) :
     name{name_},
     mass{mass_},
     charge{charge_} {}
     
void ParticleType::Print() {
   cout << "Particle type : " << name << '\n';
   cout << "Particle mass : " << mass << '\n';
   cout << "Particle charge : " << charge << '\n';
}

const char* ParticleType::GetName() {
    return name;
}
double ParticleType::GetMass() {
    return mass;
}
int ParticleType::GetCharge() {
    return charge;
}

double ParticleType::GetWidth() {
    return 0;
}
   
