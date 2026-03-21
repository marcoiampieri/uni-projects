#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <sstream>

void prepare_dataset() {
    std::ifstream file("/home/marco/root/macros/hands_on_4/atlas-higgs-challenge-2014-v2.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file!" << std::endl;
        return;
    }

    // Create output ROOT files
    TFile *sigFile = new TFile("atlas-higgs-challenge-2014-v2-sig.root", "RECREATE");
    TTree *sigTree = new TTree("tree", "Signal Data");

    TFile *bkgFile = new TFile("atlas-higgs-challenge-2014-v2-bkg.root", "RECREATE");
    TTree *bkgTree = new TTree("tree", "Background Data");

    // Define variables
    float mass_MMC, transverse_met_lep, mass_vis, pt_h, tau_pt, weight;
    std::string label;

    // Define branches
    sigTree->Branch("DER_mass_MMC", &mass_MMC);
    sigTree->Branch("DER_mass_transverse_met_lep", &transverse_met_lep);
    sigTree->Branch("DER_mass_vis", &mass_vis);
    sigTree->Branch("DER_pt_h", &pt_h);
    sigTree->Branch("PRI_tau_pt", &tau_pt);
    sigTree->Branch("Weight", &weight);

    bkgTree->Branch("DER_mass_MMC", &mass_MMC);
    bkgTree->Branch("DER_mass_transverse_met_lep", &transverse_met_lep);
    bkgTree->Branch("DER_mass_vis", &mass_vis);
    bkgTree->Branch("DER_pt_h", &pt_h);
    bkgTree->Branch("PRI_tau_pt", &tau_pt);
    bkgTree->Branch("Weight", &weight);

    // Read CSV line by line
    std::string line;
    getline(file, line); // Skip header
    std::cout << "CSV Header: " << line << std::endl;

    int sigCount = 0, bkgCount = 0;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        getline(ss, token, ','); // Skip EventId
        getline(ss, token, ','); mass_MMC = std::stof(token);
        getline(ss, token, ','); transverse_met_lep = std::stof(token);
        getline(ss, token, ','); mass_vis = std::stof(token);
        getline(ss, token, ','); pt_h = std::stof(token);

        // Skip columns 6-14
        for (int i = 5; i <= 14; i++) getline(ss, token, ',');
        tau_pt = std::stof(token); // Column 15 (tau_pt)

        // Skip columns 16-32
        for (int i = 15; i <= 31; i++) getline(ss, token, ',');
        weight = std::stof(token); // Column 32 (Weight)
        getline(ss, token, ','); // Column 34 -> This is now the correct Label column
        label = token;


        // Trim whitespace from label
        label.erase(0, label.find_first_not_of(" \t\r\n"));
        label.erase(label.find_last_not_of(" \t\r\n") + 1);
        
        // Debug: Print first few lines
        if (sigCount + bkgCount < 10) {
            std::cout << "Read: mass_MMC=" << mass_MMC
                      << ", transverse_met_lep=" << transverse_met_lep
                      << ", mass_vis=" << mass_vis
                      << ", pt_h=" << pt_h
                      << ", tau_pt=" << tau_pt
                      << ", weight=" << weight
                      << ", label=" << label << std::endl;
        }

        if (label == "s") {
            sigTree->Fill();
            sigCount++;
        } else if (label == "b") {
            bkgTree->Fill();
            bkgCount++;
        } else {
            std::cerr << "Warning: Unknown label '" << label << "' in CSV!" << std::endl;
        }
    }

    file.close();

    // Ensure data was written
    
    if (sigCount == 0 || bkgCount == 0) {
        std::cerr << "Warning: One of the trees has 0 entries!" << std::endl;
    } else {
        std::cout << "Signal events: " << sigCount << ", Background events: " << bkgCount << std::endl;
    }

    // Write trees to file
    sigFile->Write();
    sigFile->Close();

    bkgFile->Write();
    bkgFile->Close();

    std::cout << "Dataset prepared successfully!" << std::endl;
}
