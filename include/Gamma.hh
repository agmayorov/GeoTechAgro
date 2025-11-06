#ifndef SOILSIMULATION_GAMMA_HH
#define SOILSIMULATION_GAMMA_HH

#include "Detector.hh"

class Gamma : public Detector {
public:
    Gamma(G4NistManager *, G4Material *, G4LogicalVolume *, G4double, G4double);

    void Construct() override;

};


#endif