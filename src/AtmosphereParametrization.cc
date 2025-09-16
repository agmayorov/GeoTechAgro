#include <utility>

#include "AtmosphereParametrization.hh"


AtmosphereParameterisation::AtmosphereParameterisation(G4int numLayers, G4double layerHeight, G4double temperature,
                                                       G4double humidity, G4double zOffset, G4double xSize,
                                                       G4double ySize, std::vector<G4Material *> materials) :
    fNumLayers(numLayers),
    fLayerHeight(layerHeight),
    fTemperature(temperature),
    fHumidity(humidity),
    fZOffset(zOffset),
    fXSize(xSize),
    fYSize(ySize),
    fMaterials(std::move(materials)) {}


void AtmosphereParameterisation::ComputeTransformation(G4int copyNo, G4VPhysicalVolume *physVol) const {
    G4double z = fZOffset + fLayerHeight * (copyNo + 0.5);
    physVol->SetTranslation(G4ThreeVector(0, 0, z));
    physVol->SetRotation(nullptr);
}


void AtmosphereParameterisation::ComputeDimensions(G4Box &box, G4int, const G4VPhysicalVolume *) const {
    box.SetXHalfLength(fXSize / 2);
    box.SetYHalfLength(fYSize / 2);
    box.SetZHalfLength(fLayerHeight / 2);
}


G4Material *AtmosphereParameterisation::ComputeMaterial(G4VPhysicalVolume *, const G4int copyNo, const G4VTouchable *) {
    return fMaterials.at(copyNo);
}

G4int AtmosphereParameterisation::GetNumberOfMaterials() const {
    return fNumLayers;
}

G4Material *AtmosphereParameterisation::GetMaterial(G4int idx) const {
    return fMaterials.at(idx);
}
