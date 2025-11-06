#include "Detector.hh"


void Detector::init() {
    DefineMaterials();
    DefineVisual();
    ConstructContainer();
}


void Detector::ConstructContainer() {
    detContainer = new G4Box("DetectorContainer", detContainerHalfSize.x(), detContainerHalfSize.y(),
                             detContainerHalfSize.z());
    detectorContainerLV = new G4LogicalVolume(detContainer, worldMat, "DetectorContainerLV");
    detectorContainerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
    new G4PVPlacement(nullptr, detContainerPos, detectorContainerLV, "DetectorContainerPVPL", worldLV, false, 0, true);
}


void Detector::DefineMaterials() {
    leadMat = nist->FindOrBuildMaterial("G4_Pb");

    G4double molarHe = 3.016 * g / mole;
    He3 = new G4Isotope("He3", 2, 3, molarHe);
    elHe3 = new G4Element("Helium3", "He3", 1);
    elHe3->AddIsotope(He3, 1.0);

    G4double pressureHe = 1.5 * atmosphere;
    G4double temperatureHe = 293.15 * kelvin;
    G4double densityHe = (pressureHe * molarHe) / (k_Boltzmann * Avogadro * temperatureHe);
    He3Mat = new G4Material("He3Mat", densityHe, 1, kStateGas, temperatureHe, pressureHe);
    He3Mat->AddElement(elHe3, 1.0);

    G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
    G4Material *Cr = nist->FindOrBuildMaterial("G4_Cr");
    G4Material *Ni = nist->FindOrBuildMaterial("G4_Ni");
    G4Material *Mn = nist->FindOrBuildMaterial("G4_Mn");
    G4Material *Si = nist->FindOrBuildMaterial("G4_Si");

    stainlessSteelMat = new G4Material("stainlessSteelMat", 7.9 * g / cm3, 5);
    stainlessSteelMat->AddMaterial(Fe, 0.70);
    stainlessSteelMat->AddMaterial(Cr, 0.18);
    stainlessSteelMat->AddMaterial(Ni, 0.08);
    stainlessSteelMat->AddMaterial(Mn, 0.02);
    stainlessSteelMat->AddMaterial(Si, 0.02);

    polyMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    //detMat = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
	detMat = CreateCsITl();
}


G4ThreeVector Detector::GetDetectorPos() const {
    return detContainerPos;
}


G4ThreeVector Detector::GetDetectorHalfSize() const {
    return detContainerHalfSize;
}


G4String Detector::GetDetectorType() const {
    return detectorType;
}

G4LogicalVolume *Detector::GetSensitiveLV() const {
    return logicSD;
}

void Detector::DefineVisual() {
    visWhite = new G4VisAttributes(G4Color(1.0, 1.0, 1.0));
    visBlue = new G4VisAttributes(G4Color(0.0, 0.0, 1.0));
    visCyan = new G4VisAttributes(G4Color(0.0, 1.0, 1.0));
    visGrey = new G4VisAttributes(G4Color(0.5, 0.5, 0.5));
}
