#ifndef SOILSIMULATION_DETECTOR_HH
#define SOILSIMULATION_DETECTOR_HH

#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4SolidStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>


class Detector {
public:
    G4NistManager *nist;
    G4Material *worldMat;
    G4LogicalVolume *worldLV;

    G4String detectorType;
    G4LogicalVolume* logicHelium;

    G4ThreeVector detContainerPos;
    G4ThreeVector detContainerHalfSize;
    G4Box *detContainer;
    G4LogicalVolume *detectorContainerLV;

    G4double detectorPosZ = 2 * m;

    G4Material *stainlessSteelMat;
    G4Material *polyMat;
    G4Material *He3Mat;
    G4Isotope *He3;
    G4Element *elHe3;
    G4Material *leadMat;

    G4VisAttributes* visCyan;
    G4VisAttributes *visBlue;
    G4VisAttributes *visGrey;
    G4VisAttributes* visWhite;

    Detector() = default;
    ~Detector() = default;

    void init();

    virtual void DefineMaterials();
    virtual void DefineVisual();

    virtual void ConstructContainer();
    virtual void Construct() = 0;

    G4String GetDetectorType() const;
    G4ThreeVector GetDetectorPos() const;
    G4ThreeVector GetDetectorHalfSize() const;
    G4LogicalVolume *GetSensitiveLV() const;
};


#endif //SOILSIMULATION_DETECTOR_HH
