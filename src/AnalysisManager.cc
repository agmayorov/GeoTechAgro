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

    analysisManager->CreateNtuple("NeutronCount", "Neutron events");
    analysisManager->CreateNtupleIColumn("Neutron");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("InitialCRY", "CRY neutron initial energies");
    analysisManager->CreateNtupleDColumn("Ekin");
    analysisManager->FinishNtuple();
}