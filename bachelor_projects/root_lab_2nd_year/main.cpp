#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include "Particle.hpp"
#include <cmath>
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TF1.h"
#include "TStyle.h"

R__LOAD_LIBRARY( ParticleType_cpp.so )
R__LOAD_LIBRARY( ResonanceType_cpp.so )
R__LOAD_LIBRARY( Particle_cpp.so )

int main() {
    
    //Riempimento array dei tipi di particelle
    Particle::AddParticleType("Pion+",0.13957,+1,0);
    Particle::AddParticleType("Pion-",0.13957,-1,0);
    Particle::AddParticleType("Kaon+",0.49367,+1,0);
    Particle::AddParticleType("Kaon-",0.49367,-1,0);
    Particle::AddParticleType("Proton+",0.93827 ,+1,0);
    Particle::AddParticleType("Proton-",0.93827 ,-1,0);
    Particle::AddParticleType("Kaon*",0.89166 ,0,0.05);
    Particle::AddParticleType("NullParticle",0,0,0);

    //Creazione istogrammi
    TH1F *hTyp = new TH1F ("N. of particles generated","Types of particles generated",7,0,7);
    TH1F *hAzAng = new TH1F ("Azimutal angles parameters","Azimutal angles distribution",1000,0,2*M_PI);
    TH1F *hPolAng = new TH1F ("Polar angles parameters","Polar angles distribution",1000,0,M_PI);
    TH1F *hMom = new TH1F ("Momentum parameters","Particles' momentum distribution",1000,0,10);
    TH1F *hTMom = new TH1F ("Transverse momentum parameters","Particles' transverse momentum distribution",500,0,10);
    TH1F *hEn = new TH1F ("Energy parameters","Particles' energy distribution",1000,0,10);
    TH1F *hInvm1 = new TH1F ("Invariant mass parameters","Invariant mass between particles with opposite charge",1500,0,7);
    TH1F *hInvm2 = new TH1F ("Invariant mass parameters","Invariant mass between particles with same charge",1500,0,7);
    TH1F *hInvm3 = new TH1F ("Invariant mass parameters","Invariant mass between Pions/Kaons with opposite charge",1500,0,7);
    TH1F *hInvm4 = new TH1F ("Invariant mass parameters","Invariant mass between Pions/Kaons with same charge",1500,0,7);
    TH1F *hInvm5 = new TH1F ("Invariant mass parameters","Invariant mass between particles decayed from the same K*",1000,0,2);
    TH1F *hInvm6 = new TH1F ("Invariant mass parameters","Invariant mass obtained from the difference between combinations of same and opposite charges (Pions and Kaons)",1500,0,7);
    TH1F *hInvm7 = new TH1F ("Invariant mass parameters","Invariant mass obtained from the difference between combinations of same and opposite charges (all particles)",1500,0,7);

    //10^5 Generazioni
    for (int j=0;j<100000;j++) { 
        
        int i = 0;
        Particle* Particles[120];

        //100 Generazioni dei vari tipi di particelle
        for (int k=0;k<100;k++) { 
           double x = gRandom->Rndm();
           double phi = gRandom->Uniform(0,2*M_PI);
           double theta = gRandom->Uniform(0,M_PI);
           double p = gRandom->Exp(1.0);
           double px = p * sin(theta)*cos(phi);
           double py = p * sin(theta)*sin(phi);
           double pz = p * cos(theta);
            

            hAzAng->Fill(phi); //Angoli azimutali
            hPolAng->Fill(theta); //Angoli polari
            hMom->Fill(p); //Quantità di moto
            hTMom->Fill(sqrt(px*px+py*py)); //Quantità di moto trasversa
            

            Particles[k] = new Particle;


            if (x<0.4) { //Pioni+
                hTyp->Fill(0); //Istogramma dei tipi di particelle generati
                Particles[k]->SetIndex("Pion+");
                Particles[k]->SetP(px,py,pz);
                }
            else if (x<0.8){ //Pioni-
                hTyp->Fill(1);
                Particles[k]->SetIndex("Pion-");
                Particles[k]->SetP(px,py,pz);
                }
            else if (x<0.85){ //Kaoni+
                hTyp->Fill(2);
                Particles[k]->SetIndex("Kaon+");
                Particles[k]->SetP(px,py,pz);
                }
            else if (x<0.90){ //Kaoni-
                hTyp->Fill(3);
                Particles[k]->SetIndex("Kaon-");
                Particles[k]->SetP(px,py,pz);
                }
            else if (x<0.945){ //Protoni+
                hTyp->Fill(4);
                Particles[k]->SetIndex("Proton+");
                Particles[k]->SetP(px,py,pz);
                }
            else if (x<0.99){ //Protoni-
                hTyp->Fill(5);
                Particles[k]->SetIndex("Proton-");
                Particles[k]->SetP(px,py,pz);
                }
            else {            //Kaoni*
                hTyp->Fill(6);
                Particles[k]->SetIndex("Kaon*");
                Particles[k]->SetP(px,py,pz);
                double decay = gRandom->Rndm();               
                
                 if (decay<0.5) { //Decadimento in Pi+ e K-
                    Particles[100+i] = new Particle;
                    Particles[100+i]->SetIndex("Pion+"); 

                    Particles[101+i] = new Particle;
                    Particles[101+i]->SetIndex("Kaon-");             

                    Particles[k]->Decay2body(Particles[100+i],Particles[101+i]);

                    i += 2;
                   
                 }
                 
                 else { //Decadimento in Pi- e K+
                    Particles[100+i] = new Particle;
                    Particles[100+i]->SetIndex("Pion-"); 

                    Particles[101+i] = new Particle;
                    Particles[101+i]->SetIndex("Kaon+"); 

                    Particles[k]->Decay2body(Particles[100+i],Particles[101+i]);
                    
                    i += 2;
                 }
                }
            
            hEn->Fill(Particles[k]->Energy()); // Energia 
        }
        
        //Massa invariante : tutte le particelle con carica discorde
        for (int j=0; j<100+i;j++) { 
                for (int l=j+1; l<100+i;l++) {
                   if ( (Particles[l]->GetCharge() != Particles[j]->GetCharge()) && (Particles[l]->GetIndex() != 6) && (Particles[j]->GetIndex() != 6) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm1->Fill(InvM);
                    }
                }
        } 
        
        //Massa invariante : tutte le particelle con carica concorde
        for (int j=0; j<100+i;j++) { 
                for (int l=j+1; l<100+i;l++) {
                   if ( (Particles[l]->GetCharge() == Particles[j]->GetCharge()) && (Particles[l]->GetIndex() != 6) && (Particles[j]->GetIndex() != 6) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm2->Fill(InvM);
                    }
                }
        }
        
        //Massa invariante : tutte le particelle Pione/Kaone con carica discorde
        for (int j=0; j<100+i;j++) { 
                for (int l=j+1; l<100+i;l++) {
                   if ( (Particles[l]->GetIndex() == 0) && (Particles[j]->GetIndex() == 3) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm3->Fill(InvM);
                    }
                
                   if ( (Particles[l]->GetIndex() == 3) && (Particles[j]->GetIndex() == 0) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm3->Fill(InvM);
                    }

                   if ( (Particles[l]->GetIndex() == 1) && (Particles[j]->GetIndex() == 2) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm3->Fill(InvM);
                    }
                
                   if ( (Particles[l]->GetIndex() == 2) && (Particles[j]->GetIndex() == 1) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm3->Fill(InvM);
                    }
                }
        }

        //Massa invariante : tutte le particelle Pione/Kaone con carica concorde
        for (int j=0; j<100+i;j++) { 
                for (int l=j+1; l<100+i;l++) {
                   if ( (Particles[l]->GetIndex() == 0) && (Particles[j]->GetIndex() == 2) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm4->Fill(InvM);
                    }
                
                   if ( (Particles[l]->GetIndex() == 2) && (Particles[j]->GetIndex() == 0) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm4->Fill(InvM);
                    }

                   if ( (Particles[l]->GetIndex() == 1) && (Particles[j]->GetIndex() == 3) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm4->Fill(InvM);
                    }
                
                   if ( (Particles[l]->GetIndex() == 3) && (Particles[j]->GetIndex() == 1) ){  
                       double InvM = Particles[j]->InvariantMass(Particles[l]);
                        hInvm4->Fill(InvM);
                    }
                }
        }
        
        //Massa invariante : particelle provenienti dal decadimento di una K*
        for (int j=100; j<100+i;j+=2) { 
            int l = j+1;
            if (l < 100+i ) {
            double InvM = Particles[j]->InvariantMass(Particles[l]);
            hInvm5->Fill(InvM);    
            }  
        }
        
     
    }
    
    //Riempimento istogrammi

    //Canvas contenente gli istogrammi inerenti le proprietà delle particelle
    TCanvas *properties = new TCanvas("Particles' properties","PARTICLES' PROPERTIES");
    properties->Divide(2,2);
    
    //Istogramma 1.1 : distribuzione abbondanza delle particelle
    properties->cd(1);
    hTyp->Draw();
    hTyp->GetXaxis() ->SetTitle("Types of particles generated (pi+,pi-,k+,k-,p+,p-,k*)");
    hTyp->GetYaxis() ->SetTitleOffset(1.3);
    hTyp->GetYaxis() ->SetTitle("N. of particles");
    hTyp->SetFillColor(kBlue);
    hTyp->SetLineColor(kBlack);
    cout << "--------------------------------------------------------------------------------------" << endl;
    cout << "N. of entries : " << hTyp->GetEntries() << endl;
    cout << "N. of positive Pions generated : " << hTyp->GetBinContent(1) << endl;
    cout << "Error on the n. of positive Pions generated : " << hTyp->GetBinError(1) << endl;
    cout << "N. of negative Pions generated : " << hTyp->GetBinContent(2) << endl;
    cout << "Error on the n. of negative Pions generated : " << hTyp->GetBinError(2) << endl;
    cout << "N. of positive Kaons generated : " << hTyp->GetBinContent(3) << endl;
    cout << "Error on the n. of positive Kaons generated : " << hTyp->GetBinError(3) << endl;
    cout << "N. of negative Kaons generated : " << hTyp->GetBinContent(4) << endl;
    cout << "Error on the n. of negative Kaons generated : " << hTyp->GetBinError(4) << endl;
    cout << "N. of positive Protons generated : " << hTyp->GetBinContent(5) << endl;
    cout << "Error on the n. of positive Protons generated : " << hTyp->GetBinError(5) << endl;
    cout << "N. of negative Protons generated : " << hTyp->GetBinContent(6) << endl;
    cout << "Error on the n. of negative Protons generated : " << hTyp->GetBinError(6) << endl;
    cout << "N. of resonance Kaons generated : " << hTyp->GetBinContent(7) << endl;
    cout << "Error on the n. of resonance Kaons generated : " << hTyp->GetBinError(7) << endl;

    //Istogramma 1.2 : distribuzione della quantità di moto delle particelle
    properties->cd(2);
    hMom->Draw();
    hMom->GetXaxis() ->SetTitle("GeV");
    hMom->GetYaxis() ->SetTitleOffset(1.3);
    hMom->GetYaxis() ->SetTitle("N. of occurences");
    hMom->SetFillColor(kBlue);
    hMom->Fit("expo");
    TF1 *f = hMom->GetFunction("expo");
    gStyle->SetOptFit(111);
    cout << "Slope : " << f->GetParameter(1) << " +/- " << f->GetParError(1) << endl;  

    //Istogramma 1.3 : distribuzione degli angoli azimutali
    properties->cd(3);
    hAzAng->Draw();
    hAzAng->GetXaxis() ->SetTitle("Radians");
    hAzAng->GetYaxis() ->SetTitleOffset(1.3);
    hAzAng->GetYaxis() ->SetTitle("N. of occurences");
    hAzAng->SetFillColor(kBlue);
    hAzAng->Fit("pol0");
    gStyle->SetOptFit(111);

    //Istogramma 1.4 : distribuzione degli angoli polari
    properties->cd(4);
    hPolAng->Draw();
    hPolAng->GetXaxis() ->SetTitle("Radians");
    hPolAng->GetYaxis() ->SetTitleOffset(1.3);
    hPolAng->GetYaxis() ->SetTitle("N. of occurences");
    hPolAng->SetFillColor(kBlue);
    hPolAng->Fit("pol0");
    gStyle->SetOptFit(111);

    //Salvataggio canvas
    properties->Print("Particles' properties.root");
    properties->Print("Particles' properties.jpg");

 
    //Canvas contenente gli istogrammi inerenti le distribuzioni della massa invariante delle particelle
    TCanvas *invmass = new TCanvas("Invariant mass distributions","INVARIANT MASS DISTRIBUTIONS");
    invmass->Divide(1,3);


    //Istogramma 2.1 : distribuzione della massa invariante tra le particelle figlie di una K*
    invmass->cd(1);
    hInvm5->Draw();
    hInvm5->SetFillColor(kBlue);
    hInvm5->GetXaxis() ->SetTitle("GeV/c^2");
    hInvm5->GetYaxis() ->SetTitleOffset(1.3);
    hInvm5->GetYaxis() ->SetTitle("N. of occurences");
    hInvm5->Fit("gaus");
    gStyle->SetOptFit(111);
   
    //Istogramma 2.2 : distribuzione della massa invariante ottenuta dalla differenza
    //                 fra combinazioni di carica discorde e concorde (solo combinazioni pioni/kaoni)
    invmass->cd(2);
    hInvm6->Sumw2();
    hInvm6->Add(hInvm3,hInvm4,1,-1);   
    hInvm6->Draw();
    hInvm6->SetFillColor(kBlue);
    hInvm6->GetXaxis() ->SetTitle("GeV/c^2");
    hInvm6->GetYaxis() ->SetTitleOffset(1.3);
    hInvm6->GetYaxis() ->SetTitle("N. of occurences");
    hInvm6->Fit("gaus");
    gStyle->SetOptFit(111);
 
    //Istogramma 2.3 : distribuzione della massa invariante ottenuta dalla differenza
    //                 fra combinazioni di carica discorde e concorde (di tutte le paricelle)
    invmass->cd(3);
    hInvm7->Sumw2();
    hInvm7->Add(hInvm1,hInvm2,1,-1);
    hInvm7->Draw();
    hInvm7->SetFillColor(kBlue);
    hInvm7->GetXaxis() ->SetTitle("GeV/c^2");
    hInvm7->GetYaxis() ->SetTitleOffset(1.3);
    hInvm7->GetYaxis() ->SetTitle("N. of occurences");
    hInvm7->Fit("gaus");
    gStyle->SetOptFit(111);

    //Salvataggio canvas
    invmass->Print("Invariant mass distributions.root");
    invmass->Print("Invariant mass distributions.jpg");


}