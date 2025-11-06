#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include <G4UserLimits.hh>
#include <G4SystemOfUnits.hh>
#include <G4SolidStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4GeometryManager.hh>
#include <G4UImanager.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>
#include <G4PhysicalConstants.hh>

#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

#include "Detector.hh"
#include "MilliNM.hh"
#include "NMThermal.hh"
#include "NMEpithermal.hh"
#include "NMFast.hh"
#include "Gamma.hh"

#include "AtmosphereParametrization.hh"
#include <utility>


typedef std::vector<std::pair<std::string, double>> dict;

class G4VPhysicalVolume;

class G4LogicalVolume;

class Geometry : public G4VUserDetectorConstruction {
public:
    G4NistManager *nist;
    G4Material *worldMat;
    G4double worldSizeXY;
    G4double worldSizeZ;
    G4Box *worldBox;
    G4LogicalVolume *worldLV;
    G4VPhysicalVolume *worldPVP;

    G4double simulationSizeZ;

    G4double soilSize;
    G4double soilDepth;
    G4double atmosphereSize;
    G4double atmosphereHeight;

    G4VisAttributes* soilVisAttr = nullptr;
    G4VisAttributes* airVisAttr = nullptr;
    G4VisAttributes* vegetationVisAttr = nullptr;

    G4Box *soilBox;
    G4LogicalVolume *soilLV;
    G4ThreeVector soilPos;

    G4String detectorType;
    G4ThreeVector detContainerPos;
    G4ThreeVector detContainerHalfSize;

    G4double vegetation;
    G4Box *vegetationBox;
    G4LogicalVolume *vegetationLV;
    G4ThreeVector vegetationPos;

    G4Material *drySoilMat;
    G4Material *soilMat;

    G4Material *dryVegetationMat;
    G4Material *vegetationMat;

    Geometry(const G4String &, G4int, G4double, G4double, G4double, G4double, G4double, G4String);

    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;

private:
    G4RotationMatrix *zeroRot;


    G4int numLayers;
    dict materialComposition;

    G4double soilMoisture;
    G4double airHumidity;
    G4double temperature;

    G4Material *water;

    G4LogicalVolume* logicSD;
    Detector *detector;
    G4ThreeVector detectorPos;
    G4ThreeVector detectorHalfSize;

    std::vector<G4Material*> atmosphereMaterials;
    G4double layerHeight;

    void ComputeAtmosphereMaterials();

    void ReadMaterialFile(const std::string &);
    void DefineSoilMaterial();

    void ConstructSoil();
    void ConstructAtmosphere();
    void ConstructAtmBlock(const std::string &, const G4ThreeVector &, const G4ThreeVector &);
    void ConstructVegetation();
    void ConstructDetector();
};

#endif //GEOMETRY_HH
