#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "ResonanceType.hpp"

class Particle {
    public :
    int Decay2body(Particle *dau1,Particle *dau2) ;

    Particle();
    Particle(double xpulse_, double ypulse_ , double zpulse_ , const char*name_);
    int CheckName(const char* name_);
    
    static void  AddParticleType(const char* name_,double mass_,int charge_,double width_ = 0);

    void Print();
    void TypesArrayPrint();
    
    int GetIndex();
    double GetPx();
    double GetPy();
    double GetPz();

    double GetMass();
    int GetCharge();
    double Momentum();
    double Momentum2();
    double Energy();
    double InvariantMass(Particle *Particle);

    bool SetIndex(const char* name_);
    bool SetIndex(int Index_);
    void SetP(double Px_,double Py_,double Pz_);

    private :

    void Boost(double bx, double by, double bz);
    static int FindParticle(const char*name_);
    static const int MaxNumParticleTypes = 10;
    static int NParticleTypes;
    static ParticleType* ParticleTypes[MaxNumParticleTypes];
    double Px{0};
    double Py{0};
    double Pz{0};
    int Index;
};
#endif