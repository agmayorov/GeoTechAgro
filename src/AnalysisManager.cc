#include "AnalysisManager.hh"

#include "G4UnitsTable.hh"

AnalysisManager::AnalysisManager(const std::string &fName) : fFileName(fName) {
    Book();
}


void AnalysisManager::Book() {
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName(fFileName);
    analysisManager->SetVerboseLevel(0);
    analysisManager->SetNtupleActivation(true);

#ifdef G4MULTITHREADED
    analysisManager->SetNtupleMerging(true);
#endif
    analysisManager->CreateNtuple("EnergyDeposit", "Total energy deposited");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("Initial", "Neutron initial energies");
    analysisManager->CreateNtupleDColumn("Ekin");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("Initial proton", "Proton initial energies");
    analysisManager->CreateNtupleDColumn("EkinProt");
    analysisManager->FinishNtuple();
}