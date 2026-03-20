#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <RooCBShape.h>
#include <RooPolynomial.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooWorkspace.h>
#include <RooStats/ProfileLikelihoodCalculator.h>
#include <RooStats/FeldmanCousins.h>
#include <RooStats/HypoTestInverter.h>
#include <RooStats/ModelConfig.h>
#include <TSystem.h>
#include <TApplication.h>


using namespace RooFit;
using namespace RooStats;

// Function to import data from higgs_4l.dat
RooDataSet* loadData(RooRealVar& x) {
    std::ifstream file("/home/marco/root/macros/hands_on_3/higgs_4l.dat");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file higgs_4l.dat!" << std::endl;
        return nullptr;
    }

    RooDataSet* data = new RooDataSet("data", "Higgs 4l data", RooArgSet(x));
    double value;
    while (file >> value) {
        x.setVal(value);
        data->add(RooArgSet(x));
    }
    file.close();
    return data;
}

// Main function
void higgs_mass_analysis() {
    // Define observable: Higgs 4-lepton invariant mass (110-135 GeV)
    RooRealVar x("x", "Invariant Mass", 110, 135, "GeV");

    // Load dataset
    gROOT->SetBatch(kTRUE);
    RooDataSet* data = loadData(x);
    data->SetName("data");
    if (!data) return;
    std::cout << "Number of events in dataset: " << data->numEntries() << std::endl;

    // Define Crystal Ball signal model (Higgs peak)
    RooRealVar mass("mass", "Higgs Mass", 125, 110, 150);
    RooRealVar width("width", "Higgs Width", 4.1 / 2.35);
    width.setConstant(true);
    RooRealVar alpha("alpha", "CB Alpha", 0.6);
    alpha.setConstant(true);
    RooRealVar n("n", "CB n", 20);
    n.setConstant(true);
    RooCBShape smodel("smodel", "Signal Model", x, mass, width, alpha, n);

    // Define Background Model: 2nd-degree polynomial
    RooRealVar a1("a1", "Background a1", -160, -200, -100);
    RooRealVar a2("a2", "Background a2", 2.7, 2, 4);
    RooPolynomial bmodel("bmodel", "Background Model", x, RooArgList(a1, a2));

    // Define expected yields
    RooRealVar nsignal("nsignal", "Signal Yield", 50, 0, 500);
    RooRealVar nbackground("nbackground", "Background Yield", 70, 0, 500);

    // Composite Extended Likelihood Model
    RooAddPdf model("model", "Signal + Background", RooArgList(smodel, bmodel), RooArgList(nsignal, nbackground));

    // Fit the model to data
    RooFitResult* fitResult = model.fitTo(*data, Save(), Strategy(2), PrintLevel(-1));
    if (!fitResult) {
        std::cerr << "ERROR: Fit failed!" << std::endl;
        return;
    }

    // Plot data and fit
    TCanvas c1("c1", "Higgs Mass Fit", 800, 600);
    RooPlot* frame = x.frame();
    data->plotOn(frame);
    model.plotOn(frame, LineColor(kRed));
    model.plotOn(frame, Components(bmodel), LineStyle(kDashed), LineColor(kBlue));
    frame->Draw();
    c1.SaveAs("higgs_mass_fit.png");

    // Create and save workspace
    RooWorkspace w("w");
    w.import(*data);
    w.import(model);
    w.import(nsignal);
    w.import(nbackground);
    w.import(a1);
    w.import(a2);
    w.defineSet("nuisParams", "nbackground,a1,a2");
    w.defineSet("nuisParams2", "nbackground,a1,a2,nsignal,width");
    w.var("mass")->setConstant(false);

    // ModelConfig for Hypothesis Test (nsignal as parameter of interest)
    ModelConfig mc("ModelConfig", &w);
    mc.SetPdf(model);
    mc.SetParametersOfInterest(*w.var("nsignal"));
    mc.SetObservables(x);
    mc.SetNuisanceParameters(*w.set("nuisParams"));
    mc.SetSnapshot(nsignal);
    w.import(mc);

    // ModelConfig for Mass Interval Calculation (mass as POI)
    ModelConfig mc_mass("ModelConfig_mass", &w);
    mc_mass.SetPdf(model);
    mc_mass.SetParametersOfInterest(*w.var("mass"));
    mc_mass.SetNuisanceParameters(*w.set("nuisParams2"));
    w.import(mc_mass);

    w.writeToFile("higgs_workspace.root");

    // PART 2: Compute Profile Likelihood Interval
    ProfileLikelihoodCalculator plc(*data, mc_mass);
    plc.SetConfidenceLevel(0.68);
    LikelihoodInterval* interval = plc.GetInterval();
    std::cout << "Profile Likelihood Interval: [" << interval->LowerLimit(*w.var("mass")) << ", "
              << interval->UpperLimit(*w.var("mass")) << "]" << std::endl;

    // PART 3: Compute Feldman-Cousins Interval
    FeldmanCousins fc(*data, mc_mass);
    fc.SetConfidenceLevel(0.90);
    PointSetInterval* fc_interval = fc.GetInterval();
    std::cout << "Feldman-Cousins Interval: [" << fc_interval->LowerLimit(*w.var("mass")) << ", "
              << fc_interval->UpperLimit(*w.var("mass")) << "]" << std::endl;

    // PART 4: Hypothesis Test for Significance
    // Hypothesis test: Compute significance of Higgs signal (Profile Likelihood)
    ProfileLikelihoodCalculator plc_significance(*data, mc);
    plc_significance.SetConfidenceLevel(0.95);
    LikelihoodInterval* plc_interval = plc_significance.GetInterval();
    
    // Compute observed significance
    double significance = RooStats::SignificanceToPValue(plc_interval->UpperLimit(*w.var("nsignal")));
    std::cout << "Higgs significance (p-value): " << significance << std::endl;


    // PART 5: Compute p-value vs Signal Mass 
    //         (Doesn't produce the correct plot, couldn't find the mistake)
    std::vector<double> massValues = {120, 122, 124, 126, 128};
    std::vector<double> pValues;
    for (double m : massValues) {
        w.var("mass")->setVal(m);
    
        ProfileLikelihoodCalculator plc_mass(*data, mc_mass);
        plc_mass.SetConfidenceLevel(0.68);
        LikelihoodInterval* interval_mass = plc_mass.GetInterval();

        // Print the computed interval
        double upperLimit = interval_mass->UpperLimit(*w.var("mass"));
        double lowerLimit = interval_mass->LowerLimit(*w.var("mass"));
    
        std::cout << "Mass " << m << " GeV: Lower = " << lowerLimit
          << ", Upper = " << upperLimit << std::endl;
    
        double p0 = RooStats::SignificanceToPValue(interval_mass->UpperLimit(*w.var("mass")));
        pValues.push_back(p0);
        std::cout << "Mass " << m << " GeV: p-value = " << p0 << std::endl;
    }

    // Plot p-value vs mass
    TCanvas c2("c2", "Significance vs Mass", 800, 600);
    TGraph* graph = new TGraph(massValues.size(), &massValues[0], &pValues[0]);
    graph->SetTitle("p-value vs Higgs Mass;Mass (GeV);p-value");
    graph->SetMarkerStyle(20);
    graph->Draw("APL");
    c2.SaveAs("pvalue_vs_mass.png");

    std::cout << "Analysis completed!" << std::endl;
    gApplication->Terminate(0);
}
