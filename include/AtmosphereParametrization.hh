#ifndef AtmosphereParameterisation_h
#define AtmosphereParameterisation_h 1

#include "G4VNestedParameterisation.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Box.hh"

class AtmosphereParameterisation : public G4VNestedParameterisation {
public:
    AtmosphereParameterisation(G4int, G4double, G4double, G4double, G4double, G4double, G4double,
                               std::vector<G4Material *>);

    void ComputeTransformation(G4int, G4VPhysicalVolume *) const override;

    void ComputeDimensions(G4Box &box, G4int, const G4VPhysicalVolume *) const override;

    G4Material *ComputeMaterial(G4VPhysicalVolume *, const G4int, const G4VTouchable *) override;

    G4int GetNumberOfMaterials() const override;

    G4Material *GetMaterial(G4int) const override;

private:
    G4int fNumLayers;
    G4double fLayerHeight;
    G4double fTemperature;
    G4double fHumidity;
    G4double fZOffset;
    G4double fYSize;
    G4double fXSize;

    std::vector<G4Material *> fMaterials;
};

#endif
