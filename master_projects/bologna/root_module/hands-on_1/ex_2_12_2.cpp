#include "TFile.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooBreitWigner.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "RooDataSet.h"
#include "TLegend.h" 
#include "RooAbsData.h"
#include "TH1F.h"
using namespace RooFit;

void B0sFit() {
    //gSystem->ChangeDirectory("/home/marco/root/macros/hands_on_1/");
    
    TFile *file = TFile::Open("B0sInvariantMass.root");
    TH1F *hist = (TH1F *)file->Get("massaB0");

    RooRealVar mass("mass", "B0s Invariant Mass (GeV/c^2)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    RooDataHist data("mass","DataSet from massaB0",mass,hist);

    // Breit-Wigner Model
    RooRealVar meanBW("meanBW", "Mean of Breit-Wigner", 5.366, 5.0, 6.0);
    RooRealVar widthBW("widthBW", "Width of Breit-Wigner", 0.01, 0.001, 0.1);
    RooBreitWigner bw("bw", "Breit-Wigner PDF", mass, meanBW, widthBW);

    // Breit-Wigner fit to dataset
    bw.fitTo(data);

    // Gaussiano model
    RooRealVar meanG("meanG", "Mean of Gaussian", 5.366, 5.0, 6.0);
    RooRealVar sigmaG("sigmaG", "Sigma of Gaussian", 0.01, 0.001, 0.1);
    RooGaussian gauss("gauss", "Gaussian PDF", mass, meanG, sigmaG);

    // Gaussian fit to dataset
    gauss.fitTo(data);

    // Create plot for model and data
    RooPlot* frame = mass.frame();
    RooAbsData::setDefaultStorageType(RooAbsData::Tree);
    data.plotOn(frame);
    bw.plotOn(frame, LineColor(kRed), LineStyle(kDashed));
    gauss.plotOn(frame, LineColor(kBlue));

    
     // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9); // Coordinates (x1, y1, x2, y2)
    legend->SetBorderSize(1); 
    legend->SetFillStyle(0);  
    legend->SetTextSize(0.03); 

    legend->AddEntry(frame->findObject("h_data_mass"), "Data", "P"); // 'P' = points
    legend->AddEntry(frame->findObject("bw_Norm[mass]"), "Breit-Wigner", "L"); // 'L' = line
    legend->AddEntry(frame->findObject("gauss_Norm[mass]"), "Gaussian", "L");


    // Draw on canvas
    TCanvas* c1 = new TCanvas("c1", "Fit Comparison", 800, 600);
    c1->SetGrid(0, 0); 
    frame->Draw();
    legend->Draw();

    // Save the canvas
    c1->SaveAs("B0sFitComparison.png");

    // Comparing with the PDG's value for the mass
    double PDG_mass = 5.36689; 
    std::cout << "PDG mass: " << PDG_mass << " GeV/c^2" << std::endl;
    std::cout << "Breit-Wigner fitted mean: " << meanBW.getVal() << " ± " << meanBW.getError() << " GeV/c^2" << std::endl;
    std::cout << "Gaussian fitted mean: " << meanG.getVal() << " ± " << meanG.getError() << " GeV/c^2" << std::endl;

    //file->Close();
}
