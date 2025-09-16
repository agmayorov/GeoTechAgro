#ifndef SENSITIVEDETECTORHIT_HH
#define SENSITIVEDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class SDHit : public G4VHit {
public:
    SDHit() : fRootId(-1), fEdep(0.) {};
    ~SDHit() = default;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    void SetRootId(G4int id);
    G4int GetRootId() const;

    void AddEdep(G4double val);
    G4double GetEdep() const;

private:
    G4int    fRootId;
    G4double fEdep = 0.0;
};

using SDHitCollection = G4THitsCollection<SDHit>;

extern G4ThreadLocal G4Allocator<SDHit> *SDHitAllocator;

inline void *SDHit::operator new(size_t) {
    if (!SDHitAllocator) SDHitAllocator = new G4Allocator<SDHit>;
    return (void *) SDHitAllocator->MallocSingle();
}

inline void SDHit::operator delete(void *hit) {
    SDHitAllocator->FreeSingle((SDHit *) hit);
}

#endif