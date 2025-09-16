#include "SDHit.hh"


G4ThreadLocal G4Allocator<SDHit> *SDHitAllocator = nullptr;


void SDHit::AddEdep(G4double val) {
    fEdep += val;
}


G4double SDHit::GetEdep() const {
    return fEdep;
}

void SDHit::SetRootId(G4int id) {
    fRootId = id;
}

G4int SDHit::GetRootId() const {
    return fRootId;
}