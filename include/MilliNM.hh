#ifndef SOILSIMULATION_MILLINM_HH
#define SOILSIMULATION_MILLINM_HH

#include "Detector.hh"
#include "BrickParameterisation.hh"


class MilliNM : public Detector {
public:
    MilliNM(G4NistManager *, G4Material *, G4LogicalVolume *, G4double, G4double);

    void Construct() override;
};


#endif //SOILSIMULATION_MILLINM_HH
