#include "Gamma.hh"

Gamma::Gamma(G4NistManager *nistMan, G4Material *wMat, G4LogicalVolume *wLV, G4double sPosZ,
                           G4double sDepth) {
    nist = nistMan;
    worldMat = wMat;
    worldLV = wLV;

    detContainerHalfSize = G4ThreeVector(10 * cm, 10 * cm, 10 * cm);
    detContainerPos = G4ThreeVector(0, 0, sPosZ + sDepth / 2 + detectorPosZ);

    detectorType = "Gamma";

    init();
}

void Gamma::Construct() {
	G4double contSize = 0.2*m;
	G4double contThickness = 1.*cm;

    G4Box *innerPlastic = new G4Box("innerPlastic", contSize/2 - contThickness, contSize/2 - contThickness, contSize/2 - contThickness);
	G4Box *outerPlastic = new G4Box("outerPlastic", contSize/2, contSize/2, contSize/2);
	G4SubtractionSolid *subtraction = new G4SubtractionSolid("subtraction", outerPlastic, innerPlastic);
	G4ThreeVector contPosition = G4ThreeVector(0, 0, contSize/2);
	G4LogicalVolume *logicContainer = new G4LogicalVolume(subtraction, polyMat, "logicContainer");
	new G4PVPlacement(0, contPosition, logicContainer, "physContainer", detectorContainerLV, false, 0);

    logicContainer->SetVisAttributes(visWhite);
		
	G4double xCsI = 5.0*cm;
	G4double yCsI = 5.0*cm;
	G4double zCsI = 5.0*cm;
	G4Box *solidCsI = new G4Box("solidCsI", xCsI/2, yCsI/2, zCsI/2);
	logicSD = new G4LogicalVolume(solidCsI, detMat, "logicSD");
	G4ThreeVector CsIPosition = G4ThreeVector(0, 0, contThickness+zCsI/2);
	new G4PVPlacement(0, CsIPosition, logicSD, "physCsI", detectorContainerLV, false, 0);

    logicSD->SetVisAttributes(visCyan);
}