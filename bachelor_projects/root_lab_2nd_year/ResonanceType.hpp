#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"


class ResonanceType : public ParticleType {
    public :
    ResonanceType (const char* name_,double mass_, int charge_, double width_);
    double GetWidth();
    void Print();

    private :

    const double width;

};
#endif