#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "TSystem.h"
using namespace RooFit;


void macro() {
    int nbins = 25;
    double xmin = 5080;
    double xmax = 5080 +19 * nbins;

    RooRealVar x{"x","m(p-bar)",xmin,xmax,"Mev/c^2"};
    x.setBins(nbins);
    //std::cout << "Current directory: " << gSystem->WorkingDirectory() << std::endl;
    gSystem->ChangeDirectory("/home/marco/root/macros/hands_on_1/");
    auto data = *RooDataSet::read("rarest_b0_decay.dat",x,"D");

    RooRealVar c {"c","exponential par",-0.0001,-1,-0.000001};
    RooExponential b {"bkg","Comb. bkg",x,c};

    RooRealVar m0{"m0","B0 mass",5279,5220,5320};
    RooRealVar s0{"s0","B0 width",10,2,50};
    RooGaussian g0{"g0","B0 peak",x,m0,s0};

    RooRealVar m1{"m1","B0s mass",5380,5320,5420};
    RooGaussian g1{"g1","B0s peak",x,m1,s0};

    RooRealVar n0 {"N0","n events of B0",0.2,0.0001,1};
    RooRealVar n1 {"N1","n events of Bs",800,0.0001,1};
    RooRealVar nbck {"NB","n events of bck",20,0.0001,1};

    RooAddPdf model{"model","model",RooArgList{g0,g1,b},
                                    RooArgList{n0,n1,nbck}};

    model.fitTo(data);
    auto xframe = x.frame();

    data.plotOn(xframe);
    model.plotOn(xframe);
    model.plotOn(xframe, RooFit::Components("bkg"),RooFit::LineColor(kGreen));
    model.plotOn(xframe, RooFit::Components("g0"),RooFit::LineColor(kRed));
    model.plotOn(xframe, RooFit::Components("g1"),RooFit::LineColor(kGray));
    xframe->Draw();
}

   