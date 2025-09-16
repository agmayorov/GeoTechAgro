#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(const G4String &name) : G4VSensitiveDetector(name) {
    collectionName.insert("HitsCollection");
    fHCID = -1;
}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent *hce) {
    fHitsCollection = new SDHitCollection(SensitiveDetectorName, "HitsCollection");

    if (fHCID < 0)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);

    hce->AddHitsCollection(fHCID, fHitsCollection);

    fIndexByRoot.clear();
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
    auto analysisManager = G4AnalysisManager::Instance();

    for (G4int i = 0; i < fHitsCollection->GetSize(); ++i) {
        auto hit = (*fHitsCollection)[i];
        analysisManager->FillNtupleDColumn(0, 0, hit->GetEdep() / keV);
        analysisManager->AddNtupleRow();
    }
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    auto *track = step->GetTrack();
    auto *tag = static_cast<TrackTag *>(track->GetUserInformation());
    if (!(tag && tag->from_neutron && tag->root_id > 0)) return false;

    const G4int root = tag->root_id;

    SDHit *hit = nullptr;
    auto it = fIndexByRoot.find(root);
    if (it == fIndexByRoot.end()) {
        hit = new SDHit();
        hit->SetRootId(root);
        fHitsCollection->insert(hit);
        const G4int idx = fHitsCollection->GetSize() - 1;
        fIndexByRoot.emplace(root, idx);
    } else {
        hit = (*fHitsCollection)[it->second];
    }

    hit->AddEdep(edep);
    return true;
}