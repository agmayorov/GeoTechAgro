#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4AnalysisManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


class EventAct : public G4UserEventAction {
public:
    EventAct() = default;
    ~EventAct() override = default;

public:
    void BeginOfEventAction(const G4Event *) override;
    void EndOfEventAction(const G4Event *) override;

    void AddEdep(G4double);
    void AddEflow(G4double);
    void AddLog(const G4String &, G4double, G4double);
    void AddAlphaEnergy(G4double);
    void AddNeutronEnergy(G4double);

private:
    G4double fTotalEnergyDeposit = 0.;
    G4double fTotalEnergyFlow = 0.;
};

#endif
