#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
#include <iostream>
//#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;
class ParticleType {
    public:
    
    ParticleType(const char* name_,double mass_, int charge_);
    const char* GetName() ;
    double GetMass() ;
    int GetCharge() ;
    virtual void Print();
    virtual double GetWidth();
    
    private :
    const char* name;
    const double mass;
    const int charge;
};
#endif