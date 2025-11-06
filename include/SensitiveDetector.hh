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

#include <algorithm>

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String&, G4String);
    ~SensitiveDetector();

private:
    G4String detectorType;

    G4double fTotalEnergyDepositedGamma;

    std::vector<G4double> fTotalEnergyDeposited;
    G4int parentID;
    std::vector<G4int> registeredID;

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};


#endif