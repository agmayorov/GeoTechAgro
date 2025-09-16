#ifndef SOILSIMULATION_NMTHERMAL_HH
#define SOILSIMULATION_NMTHERMAL_HH

#include "Detector.hh"


class NMThermal : public Detector {
public:
    NMThermal(G4NistManager *, G4Material *, G4LogicalVolume *, G4double, G4double);

    void Construct() override;

};


#endif //SOILSIMULATION_NMTHERMAL_HH
