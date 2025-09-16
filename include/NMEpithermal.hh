#ifndef SOILSIMULATION_NMEPITHERMAL_HH
#define SOILSIMULATION_NMEPITHERMAL_HH

#include "Detector.hh"


class NMEpithermal : public Detector {
public:
    NMEpithermal(G4NistManager *, G4Material *, G4LogicalVolume *, G4double, G4double);

    void Construct() override;
};


#endif //SOILSIMULATION_NMEPITHERMAL_HH
