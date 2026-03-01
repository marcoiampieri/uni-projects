#include "ResonanceType.hpp"
ResonanceType::ResonanceType (const char* name_,double mass_, int charge_, double width_) :
ParticleType(name_,mass_,charge_), 
width{width_} {}


void ResonanceType::Print() {
    ParticleType::Print();
    cout << "Resonance width : "<< width << '\n';
}
double ResonanceType::GetWidth() {
    return width;
}