#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "StepAct.hh"
#include "unordered_map"

#include "SDHit.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String&);
    ~SensitiveDetector();

private:
    SDHitCollection *fHitsCollection = nullptr;
    G4int fHCID = -1;
    std::unordered_map<G4int, G4int> fIndexByRoot;

    void Initialize(G4HCofThisEvent *) override;
    void EndOfEvent(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};


#endif