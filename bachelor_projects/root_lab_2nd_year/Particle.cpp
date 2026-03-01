#include "Particle.hpp"

ParticleType* Particle::ParticleTypes[MaxNumParticleTypes];
int Particle::NParticleTypes{0};

//Costruttore di default
Particle::Particle() :
    Px{0},
    Py{0},
    Pz{0},
    Index{7} {}

//Costruttore
Particle::Particle(double Px_, double Py_, double Pz_,const char*name_) :
    Px{Px_}, 
    Py{Py_}, 
    Pz{Pz_},
    Index{CheckName(name_)} {}

int Particle::FindParticle(const char*name_) {
    
    for (int i = 0; i < NParticleTypes ; i++ ) {
       if (ParticleTypes[i]->GetName() == name_) {
           return i;
       }
    }
    
    return -1;
}

int Particle::CheckName(const char* name_) {
    if ((FindParticle(name_) == -1)) {
       cout << "Error: No particle with name " << name_ << " found." << endl;
    }
    
    return FindParticle(name_);
    
}

void Particle::AddParticleType(const char* name_, double mass_, int charge_, double width_) {
  if (FindParticle(name_) != -1) {
    throw runtime_error{"Warning: particle name already in ParticleTypes array."};

  }
  if (NParticleTypes == 10) {
    std::cout << "Warning: Particles array full." << endl;
  }

  if (width_ == 0) {
    ParticleTypes[NParticleTypes++] = new ParticleType(name_, mass_, charge_);
  }

  if (width_ != 0) {
  ParticleTypes[NParticleTypes++] = new ResonanceType(name_, mass_, charge_, width_);
  }
}

void Particle::Print() {
  std::cout << "Particle type index: " << Index                                   << endl
            << "Particle name: "       << ParticleTypes[Index]->GetName()        << endl
            << "Particle momentum: ("  << Px << ", " << Py << ", " << Pz << ")" << endl;
}

void Particle::TypesArrayPrint() {
    for (int i=0; i<NParticleTypes;i++) {
      ParticleTypes[i]->Print();
    }
}

bool Particle::SetIndex(int Index_) {
  if (Index_ < 0 || Index >= MaxNumParticleTypes || Index >= NParticleTypes) {
    std::cout << "Invalid particle index" << endl;
    return false;
  }

  Index = Index_;
  return true;
}
bool Particle::SetIndex(const char* name_) {
  return SetIndex(FindParticle(name_));
}

void Particle::SetP(double Px_,double Py_,double Pz_) {
  Px = Px_;
  Py = Py_;
  Pz = Pz_;
}

int Particle::GetIndex() {
    return Index;
}

double Particle::GetPx() {
    return Px;
}
double Particle::GetPy() {
    return Py;
}
double Particle::GetPz() {
    return Pz;
}

double Particle::GetMass()  {
  return ParticleTypes[Index]->GetMass();
}

int Particle::GetCharge()  {
  return ParticleTypes[Index]->GetCharge();
}

double Particle::Momentum2()  {
  return Px * Px + Py * Py + Pz * Pz;
}

double Particle::Momentum()  {
  return sqrt(Momentum2());
}

double Particle::Energy() {
    return sqrt(GetMass() * GetMass() + Momentum2());
}

double Particle::InvariantMass(Particle *Particle)  {
  const double EnergySumSquared = pow(Energy() + Particle->Energy(), 2);
  const double MomentumSumxSquared = pow(GetPx() + Particle->GetPx(), 2);
  const double MomentumSumySquared = pow(GetPy() + Particle->GetPy(), 2);
  const double MomentumSumzSquared = pow(GetPz() + Particle->GetPz(), 2);
  const double MomentumSumSquared = MomentumSumxSquared + MomentumSumySquared + MomentumSumzSquared;

  return sqrt(EnergySumSquared - MomentumSumSquared);
}


int Particle::Decay2body(Particle *dau1,Particle *dau2)  {
  if(GetMass() == 0.0){
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  
  double massMot = GetMass();
  double massDau1 = dau1->GetMass();
  double massDau2 = dau2->GetMass();

  if(Index > -1){ // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;
    
    double invnum = 1./RAND_MAX;
    do {
      x1 = 2.0 * rand()*invnum - 1.0;
      x2 = 2.0 * rand()*invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += ParticleTypes[Index]->GetWidth() * y1;

  }

  if(massMot < massDau1 + massDau2){
    printf("Decayment cannot be performed because mass is too low in this channel\n");
    return 2;
  }
  
  double pout = sqrt((massMot*massMot - (massDau1+massDau2)*(massDau1+massDau2))*(massMot*massMot - (massDau1-massDau2)*(massDau1-massDau2)))/massMot*0.5;

  double norm = 2*M_PI/RAND_MAX;

  double phi = rand()*norm;
  double theta = rand()*norm*0.5 - M_PI/2.;
  dau1->SetP(pout*sin(theta)*cos(phi),pout*sin(theta)*sin(phi),pout*cos(theta));
  dau2->SetP(-pout*sin(theta)*cos(phi),-pout*sin(theta)*sin(phi),-pout*cos(theta));

  double energy = sqrt(Px*Px + Py*Py + Pz*Pz + massMot*massMot);

  double bx = Px/energy;
  double by = Py/energy;
  double bz = Pz/energy;

  dau1->Boost(bx,by,bz);
  dau2->Boost(bx,by,bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz)
{

  double energy = Energy();

  //Boost this Lorentz vector
  double b2 = bx*bx + by*by + bz*bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx*Px + by*Py + bz*Pz;
  double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

  Px += gamma2*bp*bx + gamma*bx*energy;
  Py += gamma2*bp*by + gamma*by*energy;
  Pz += gamma2*bp*bz + gamma*bz*energy;
}