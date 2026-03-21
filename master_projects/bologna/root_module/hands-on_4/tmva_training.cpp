#include <TMVA/Factory.h>
#include <TMVA/DataLoader.h>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TSystem.h>

void tmva_training() {
    gSystem->ChangeDirectory("/home/marco/root/macros/hands_on_4/");
    // Create an output file for the results
    TFile* outputFile = TFile::Open("TMVA_output.root", "RECREATE");

    // Create TMVA factory
    TMVA::Factory factory("TMVAClassification", outputFile, 
                          "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification");

    // Create DataLoader
    TMVA::DataLoader dataloader("dataset");

    // Add input variables (excluding EventId, KaggleSet, KaggleWeight, Weight, and Label)
    dataloader.AddVariable("DER_mass_MMC", 'F');
    dataloader.AddVariable("DER_mass_transverse_met_lep", 'F');
    dataloader.AddVariable("DER_mass_vis", 'F');
    dataloader.AddVariable("DER_pt_h", 'F');
    dataloader.AddVariable("PRI_tau_pt", 'F');

    // Load signal and background datasets
    TFile* sigFile = TFile::Open("atlas-higgs-challenge-2014-v2-sig.root");
    if (!sigFile || sigFile->IsZombie()) {
        std::cerr << "Error: Could not open signal ROOT file!" << std::endl;
        return;
    }

    TTree* sigTree = (TTree*)sigFile->Get("tree");
    if (!sigTree) {
        std::cerr << "Error: No tree named 'tree' found in signal file!" << std::endl;
        sigFile->ls();  // List contents of the file
        return;
    }

    TFile* bkgFile = TFile::Open("atlas-higgs-challenge-2014-v2-bkg.root");
    if (!bkgFile || bkgFile->IsZombie()) {
        std::cerr << "Error: Could not open background ROOT file!" << std::endl;
        return;
    }

    TTree* bkgTree = (TTree*)bkgFile->Get("tree");
    if (!bkgTree) {
        std::cerr << "Error: No tree named 'tree' found in background file!" << std::endl;
        bkgFile->ls();  // List contents of the file
        return;
    }

    // Add signal and background trees
    dataloader.AddSignalTree(sigTree, 1.0);  // Weight 1.0
    dataloader.AddBackgroundTree(bkgTree, 1.0);

    // Define event weights
    dataloader.SetWeightExpression("Weight");

    // Split data into training/testing (10k signal, 20k background for training)
    dataloader.PrepareTrainingAndTestTree("", "SplitMode=Random:NormMode=NumEvents:NTrain_Signal=10000:NTrain_Background=20000:NTest_Signal=0:NTest_Background=0");

    // Book machine learning methods
    factory.BookMethod(&dataloader, TMVA::Types::kCuts, "Cuts", "!H:!V:FitMethod=MC");
    factory.BookMethod(&dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher");
    factory.BookMethod(&dataloader, TMVA::Types::kMLP, "MLP", "H:!V:HiddenLayers=N+5");
    factory.BookMethod(&dataloader, TMVA::Types::kBDT, "BDT", "NTrees=200:BoostType=AdaBoost");

    // Train and test the classifiers
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();

    // Save and close files
    outputFile->Close();
    delete sigFile;
    delete bkgFile;
    delete outputFile;
}
