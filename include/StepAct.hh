#ifndef StepAct_h
#define StepAct_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Neutron.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include <unordered_set>


class TrackTag : public G4VUserTrackInformation {
public:
    TrackTag(bool fromNeutron=false, G4int rootNeutronId=-1)
        : G4VUserTrackInformation("TrackTag"),
          from_neutron(fromNeutron), root_id(rootNeutronId) {}

    bool  from_neutron;
    G4int root_id;
};


class StepAct : public G4UserSteppingAction {
public:
    StepAct(G4String );
    ~StepAct() = default;

    void UserSteppingAction(const G4Step *) override;
    void RecordHit(G4int);

private:
    G4String detectorType;
    const std::unordered_set<std::string> atmLayerNames = {
        "WorldPVPL", "TopAtmLayerPVPL", "BottomAtmLayerPVPL",
        "LeftAtmLayerPVPL", "RightAtmLayerPVPL",
        "FrontAtmLayerPVPL", "BackAtmLayerPVPL"
    };
};

#endif
