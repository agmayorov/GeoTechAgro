#include "EventAct.hh"


void EventAct::BeginOfEventAction(const G4Event *) {
    fTotalEnergyDeposit = 0.;
    fTotalEnergyFlow = 0.;
}


void EventAct::AddEdep(G4double Edep) {
    fTotalEnergyDeposit += Edep;
}


void EventAct::AddEflow(G4double Eflow) {
    fTotalEnergyFlow += Eflow;
}


void EventAct::AddLog(const G4String &name, G4double energy, G4double z) {
    G4AnalysisManager::Instance()->FillNtupleSColumn(0, name);
    G4AnalysisManager::Instance()->FillNtupleDColumn(1, energy / MeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(2, z / mm);
    G4AnalysisManager::Instance()->AddNtupleRow();
}


void EventAct::AddAlphaEnergy(G4double energy) {
    auto *analysis = G4AnalysisManager::Instance();
    if (analysis && analysis->IsActive()) {
        analysis->FillNtupleDColumn(0, energy);
        analysis->AddNtupleRow();
    }
}


void EventAct::AddNeutronEnergy(G4double energy) {
    auto *analysis = G4AnalysisManager::Instance();
    if (analysis && analysis->IsActive()) {
        analysis->FillNtupleDColumn(0, energy);
        analysis->AddNtupleRow();
    }
}


void EventAct::EndOfEventAction(const G4Event *) {}
