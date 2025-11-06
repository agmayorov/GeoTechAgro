#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(const G4String &name, G4String detectorType) : G4VSensitiveDetector(name) {
}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent *hce) {
    if (detectorType == "Gamma") {
        fTotalEnergyDepositedGamma = 0.;
    }
    else {
        parentID = -1;
        registeredID.clear();
        fTotalEnergyDeposited.clear();
    }
    
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
    auto analysisManager = G4AnalysisManager::Instance();

    if (detectorType == "Gamma") {
        if (fTotalEnergyDepositedGamma > 0) {
            analysisManager->FillNtupleDColumn(0, 0, fTotalEnergyDepositedGamma/keV);
            analysisManager->AddNtupleRow();
        }
    }

    else {
        for (G4double num : fTotalEnergyDeposited) {
            analysisManager->FillNtupleDColumn(0, 0, num/keV);
            analysisManager->AddNtupleRow();
        }
    }
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
    if (detectorType == "Gamma") {
        G4double fEnergyDeposited = step->GetTotalEnergyDeposit();

        if (fEnergyDeposited > 0)
        {
            fTotalEnergyDepositedGamma += fEnergyDeposited;
        }
    }

    else {
        G4Track* track = step->GetTrack();

        G4int trackID = track->GetTrackID();
        G4int currentParentID = track->GetParentID();   

        G4double fEnergyDeposited = step->GetTotalEnergyDeposit();
        if (fEnergyDeposited <= 0) return true;

        registeredID.push_back(trackID);
    
        bool isNewParentID = false;
        bool isNotSecondaryElectron = true;

        if (currentParentID != parentID) {isNewParentID = true;}
        for (G4int num : registeredID) {
            if (num == currentParentID) {isNotSecondaryElectron = false;}
        }

        if (isNewParentID && isNotSecondaryElectron) {
            fTotalEnergyDeposited.push_back(fEnergyDeposited);
            parentID = currentParentID;
        }  

        else {
            if (!fTotalEnergyDeposited.empty()) {
                fTotalEnergyDeposited.back() += fEnergyDeposited;
            }
            else {
                fTotalEnergyDeposited.push_back(fEnergyDeposited);
            }
        }
    }   
    
    return true;
}