#ifndef BRICKPARAMETERISATION_HH
#define BRICKPARAMETERISATION_HH

#include "G4VPVParameterisation.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class BrickParameterisation : public G4VPVParameterisation {
public:
    BrickParameterisation();
    virtual ~BrickParameterisation() override;

    virtual void ComputeTransformation(G4int copyNo, G4VPhysicalVolume* physVol) const override;
};

#endif