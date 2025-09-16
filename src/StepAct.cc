#include "StepAct.hh"


StepAct::StepAct(G4String detType) : detectorType(std::move(detType)) {}

void StepAct::RecordHit(G4int ntupleId) {
    auto *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(ntupleId, 0, 1);
    analysisManager->AddNtupleRow(ntupleId);
}


void StepAct::UserSteppingAction(const G4Step *step) {
    const G4Track *track = step->GetTrack();
    const bool isNeutron = (track->GetParticleDefinition() == G4Neutron::Definition());

    auto* tag = static_cast<TrackTag*>(track->GetUserInformation());
    if (!tag) {
        G4int root = isNeutron ? track->GetTrackID() : -1;
        tag = new TrackTag(isNeutron, root);
        track->SetUserInformation(tag);
    } else if (isNeutron && tag->root_id < 0) {
        tag->from_neutron = true;
        tag->root_id = track->GetTrackID();
    }

    if (const auto* secs = step->GetSecondaryInCurrentStep()) {
        const bool fromN = tag->from_neutron || isNeutron;
        const G4int root = tag->from_neutron ? tag->root_id : (isNeutron ? track->GetTrackID() : -1);
        for (const auto* sec : *secs) {
            auto* child = const_cast<G4Track*>(sec);
            child->SetUserInformation(new TrackTag(fromN, root));
        }
    }
    if (isNeutron) return;

    G4int trackID = track->GetTrackID();
    if (trackID != 1) return;

    G4StepPoint *prePoint = step->GetPreStepPoint();
    G4StepPoint *postPoint = step->GetPostStepPoint();
    if (!prePoint || !postPoint) return;

    G4VPhysicalVolume *volume1 = prePoint->GetPhysicalVolume();
    G4VPhysicalVolume *volume2 = postPoint->GetPhysicalVolume();
    if (!volume1 || !volume2) return;

    const G4String &name1 = volume1->GetName();
    const G4String &name2 = volume2->GetName();

    if (atmLayerNames.count(name1)) {
        if ((detectorType == "MilliNM" && (name2 == "physPoly1" || name2 == "physPoly3" || name2 == "physPoly4")) ||
            (detectorType == "NMFast" && name2 == "physPoly1" && (std::abs(prePoint->GetPosition().z()) >= 17.5 * cm)) ||
            (detectorType == "NMEpithermal" && name2 == "physPoly") ||
            (detectorType == "NMThermal" && name2 == "physMetal")) {
            RecordHit(1);
        }
    }
}