#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "PrimaryPartCRY.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

#include "G4AnalysisManager.hh"
#include "AnalysisManager.hh"
#include "G4Threading.hh"

#include <iomanip>

class RunAct : public G4UserRunAction {
public:
    RunAct();
    ~RunAct() override;

public:
    void BeginOfRunAction(const G4Run *) override;
    void EndOfRunAction(const G4Run *) override;


private:
    AnalysisManager *fAnalysisManager;
    std::string fileName;
};

#endif
