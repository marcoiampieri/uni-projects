#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "RooExponential.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "RooFit.h"
#include "RooDataHist.h"
#include "RooExtendPdf.h"
using namespace RooFit;
 
void roofit_empty_en()
{
  // Declare variables 
  RooRealVar x("x", "x", -20, 20);
  x.setBins(40);
  
  RooRealVar mean("mean", "Mean of Gaussian", 0);
  RooRealVar sigma("sigma", "Width of Gaussian", 3);
  RooGaussian sig("sig", "Signal Gaussian PDF", x, mean, sigma);

  // Build gaussian pdf in terms of x,mean and sigma
  RooRealVar tau("tau", "Decay constant", 10, 0.1, 50);
  RooFormulaVar minus_one_over_tau("minus_one_over_tau", "-1./tau", RooArgList(tau));
  RooExponential bkg("bkg", "Exponential Background PDF", x, minus_one_over_tau);
  RooRealVar fsig("fsig", "Signal fraction", 0.5, 0.0, 1.0);
  RooAddPdf model("model", "Signal + Background Model", RooArgList(sig, bkg),
                   RooArgList(fsig));


  // Construct plot frame in 'x'
  RooPlot* frame1 = x.frame(); 

  // Plot gauss in frame (i.e. in x)
  model.plotOn(frame1, LineColor(kBlue), LineStyle(kSolid));
  model.plotOn(frame1, Components(sig), LineColor(kRed), LineStyle(kDashed));
  model.plotOn(frame1, Components(bkg), LineColor(kGreen), LineStyle(kDotted));


  // Generate a dataset of 1000 events in x from gauss
  RooDataHist* data = model.generateBinned(x, 1000);
 

  // Fit pdf to data
  model.fitTo(*data, RooFit::SumW2Error(kTRUE));
   
  // Draw all frames on a canvas
  frame1->SetTitle("Composite Model: Signal + Background");
  data->plotOn(frame1);
  model.plotOn(frame1, LineColor(kBlue), LineStyle(kSolid));
  model.plotOn(frame1, Components(sig), LineColor(kRed), LineStyle(kDashed));
  model.plotOn(frame1, Components(bkg), LineColor(kGreen), LineStyle(kDotted));
  
  TCanvas* c = new TCanvas("c", "Composite Model", 800, 600);
  frame1->Draw();

  c->SaveAs("composite_model_signal_background.png");
}
 
void extended_ML_fit() {
  // Define observable
  RooRealVar x("x", "Observable", -10, 10);

  // Signal: Gaussian
  RooRealVar mean("mean", "Mean of Gaussian", 0);
  RooRealVar sigma("sigma", "Width of Gaussian", 3);
  RooGaussian sig("sig", "Signal Gaussian", x, mean, sigma);

  // Background: Exponential
  RooRealVar tau("tau", "Decay parameter", 0,-0.1, -10);
  RooExponential bkg("bkg", "Exponential Background", x, tau);

  // Event counts for extended fit
  RooRealVar Nsig("Nsig", "Number of signal events", 200, 0, 10000);
  RooRealVar Nbkg("Nbkg", "Number of background events", 800, 0, 10000);

  // Create extended PDFs
  RooExtendPdf esig("esig", "Extended Signal PDF", sig, Nsig);
  RooExtendPdf ebkg("ebkg", "Extended Background PDF", bkg, Nbkg);

  // Combined model
  RooAddPdf model("model", "Signal + Background", RooArgList(esig, ebkg));

  // Generate binned dataset
  RooDataHist* data = model.generateBinned(x, 1000);

  // Fit model to data
  model.fitTo(*data);

  // Plot results
  TCanvas* c1 = new TCanvas("c1", "Extended ML Fit", 800, 600);
  RooPlot* frame = x.frame();
  data->plotOn(frame);
  model.plotOn(frame);
  model.plotOn(frame, RooFit::Components("esig"), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed));
  model.plotOn(frame, RooFit::Components("ebkg"), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue));
  
  frame->Draw();
  c1->SaveAs("ExtendedMLFit.png");
}
