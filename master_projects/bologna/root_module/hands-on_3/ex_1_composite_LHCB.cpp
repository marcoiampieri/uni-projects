#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "RooArgSet.h"
#include "RooFit.h"
#include "TSystem.h"
#include "RooHist.h"

void rare_b0_decay() {
    // Step 1: Load the dataset
    int nbins = 25;
    double xmin = 5080;
    double xmax = 5080 + 19 * nbins;
    
    //gSystem->ChangeDirectory("/home/marco/root/macros/hands_on_3/");
    RooRealVar x{"x", "m(p-pbar)", xmin, xmax, "Mev/c^2"};
    x.setBins(nbins);
    auto data = *RooDataSet::read("rarest_b0_decay.dat", x, "D");
  
    RooRealVar c{"c", "exponential par", -0.0001, -1, -0.000001};
    RooExponential b{"bkg", "Comb. bkg.", x, c};
  
  
    RooRealVar m0{"m0", "B0 mass", 5279, 5100, 5600};
    RooRealVar s0{"s0", "B0 width", 10, 2, 50};
    RooGaussian g0{"g0", "B0 peak", x, m0, s0};
  
    RooRealVar m1{"m1", "B0s mass", 5380, 5100, 5600};
    RooRealVar s1{"s1", "B0s width", 10, 2, 50};
    RooGaussian g1{"g1", "B0s peak", x, m1, s0};
  
    RooRealVar f0{"f0", "fraction of B0", 0.2, 0.0001, 1};
    RooRealVar f1{"f1", "fraction of Bs", 0.2, 0.0001, 1};
  
    RooAddPdf model{"model", "model", RooArgList{g0, g1, b}, RooArgList{f0, f1}};
  
    auto fit_res = model.fitTo(data, RooFit::Save());
  
    TCanvas c1;
    gStyle->SetPalette(1);
    auto frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, RooFit::Components("bkg"), RooFit::LineColor(kGreen));
    model.plotOn(frame, RooFit::Components("g0"), RooFit::LineColor(kRed));
    model.plotOn(frame, RooFit::Components("g1"), RooFit::LineColor(kGray));
  
    frame->Draw();
    c1.SaveAs("model.png");

    // Step 5: Residual and Pull Histograms
    auto resid = frame->residHist();
    auto pull = frame->pullHist();

    // Draw the residuals
    TCanvas c2;
    resid->Draw();
    c2.SaveAs("residuals.png");

    // Draw the pulls
    TCanvas c3;
    pull->Draw();
    c3.SaveAs("pulls.png");

    // Step 6: Correlation Matrix
    TCanvas c4;
    gStyle->SetPalette(1);
    RooFitResult* fit_results = model.fitTo(data, RooFit::Save(true));
    fit_results->correlationHist()->Draw("colz");
    c4.SaveAs("corr_mat.png");

    // Part 2: Simultaneous Fit
    //         I think the code should be correct, but I've got some problems
    //         with my ROOT installation, since the RooFit::Cut() function
    //         doesn't work, even if all the necessary libraries are installed

    // Define the control region model
    RooRealVar x_ctl("x_ctl", "Invariant Mass (Control Region)", 4000, 5000);
    RooExponential model_ctl("model_ctl", "Control Region Model", x_ctl, c);

    // Generate 10000 events for the control region
    RooDataSet data_ctl("data_ctl", "Control Region Data", RooArgSet(x_ctl));
    model_ctl.generateEvent(10000);

    // Define category for distinguishing physics and control samples
    RooCategory sample("sample", "sample");
    sample.defineType("physics");
    sample.defineType("control");

    // Create a combined dataset
    RooDataSet combData("combData", "Combined Data", RooArgSet(x, x_ctl), RooFit::Index(sample),
                        RooFit::Import("physics", data), RooFit::Import("control", data_ctl));

    // Construct the simultaneous PDF
    RooSimultaneous simPdf("simPdf", "Simultaneous Model", sample);
    simPdf.addPdf(model, "physics");
    simPdf.addPdf(model_ctl, "control");

    // Perform the simultaneous fit
    simPdf.fitTo(combData, RooFit::Save(true));

    // Plot the physics region
    RooPlot* frame_physics = x.frame();
    combData.plotOn(frame_physics, RooFit::Cut("sample=='physics'"));
    simPdf.plotOn(frame_physics, RooFit::Slice(sample, "physics"));
    TCanvas c5;
    frame_physics->Draw();
    c5.SaveAs("physics.png");

    // Plot the control region
    RooPlot* frame_control = x.frame();
    combData.plotOn(frame_control, RooFit::Cut("sample=='control'"));
    simPdf.plotOn(frame_control, RooFit::Slice(sample, "control"));
    TCanvas c6;
    frame_control->Draw();
    c6.SaveAs("control.png");
}
