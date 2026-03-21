#include <ROOT/RDataFrame.hxx>  // ROOT's RDataFrame for handling CSV data
#include <TFile.h>              // For saving ROOT files
#include <TTree.h>              // For creating TTrees
#include <iostream>             // Standard I/O
#include "TSystem.h"
//  allows to analyse data stored in TTrees (CSVs) with a high level interface.

void prepare_dataset()
{
  gSystem->ChangeDirectory("/home/marco/root/macros/hands_on_4/");
  auto df = ROOT::RDF::MakeCsvDataFrame("atlas-higgs-challenge-2014-v2.csv");

  // Get the number of entries

  auto nevents = *df.Count(); 
  std::cout << "n. dati " << nevents << "\n"; 
  // auto h1 = df.Histo1D("totale_casi");

  // Returns the names of the available columns.

  auto colNames = df.GetColumnNames();
  for (auto &&colName : colNames)  {
    std::cout << colName << ", " ;  
  }
  std::cout << '\n';

  // auto colNames = df.GetColumnNames();
  // for (auto &&colName : colNames)  {
  //   std::cout << "  loader.AddVariable(\"" << colName << "\", 'F');  \t// "
  //     << colName << " (" << df.GetColumnType(colName) << ")\n";
  // }

  // Save selected columns to disk, in a new TTree in new file.
  df.Filter("Label == \"s\" ")
    .Snapshot("tree", "atlas-higgs-challenge-2014-v2-sig.root");  

  df.Filter("Label == \"b\" ")
    .Snapshot("tree", "atlas-higgs-challenge-2014-v2-bkg.root");

}
