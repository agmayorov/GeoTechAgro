#ifndef SOILSIMULATION_NMFAST_HH
#define SOILSIMULATION_NMFAST_HH

#include "Detector.hh"


class NMFast : public Detector {
public:
    NMFast(G4NistManager *, G4Material *, G4LogicalVolume *, G4double, G4double);

    void Construct() override;
};


#endif //SOILSIMULATION_NMFAST_HH
