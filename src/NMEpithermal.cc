#include "NMEpithermal.hh"

NMEpithermal::NMEpithermal(G4NistManager *nistMan, G4Material *wMat, G4LogicalVolume *wLV, G4double sPosZ,
                           G4double sDepth) {
    nist = nistMan;
    worldMat = wMat;
    worldLV = wLV;

    detContainerHalfSize = G4ThreeVector(5 * cm, 40 * cm, 5 * cm);
    detContainerPos = G4ThreeVector(0, 0, sPosZ + sDepth / 2 + detectorPosZ + detContainerHalfSize.z());
    
    detectorType = "NMEpithermal";

    init();
}

void NMEpithermal::Construct() {
    G4Box *box1_rot = new G4Box("box1_rot", 5 * cm, 5 * cm, 40 * cm);

    G4Tubs *solidHelium = new G4Tubs("solidHelium", 0, 24.5 * mm, 39.95 * cm, 0, 360 * deg);
    logicSD = new G4LogicalVolume(solidHelium, He3Mat, "logicSD");
    G4RotationMatrix *rotCylinder = new G4RotationMatrix();
    rotCylinder->rotateX(90 * deg);
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicSD, "physHelium", detectorContainerLV, false, 0);

    logicSD->SetVisAttributes(visWhite);

    G4Tubs *outerMetal = new G4Tubs("outerMetal", 0, 25 * mm, 40 * cm, 0, 360 * deg);
    G4SubtractionSolid *subtractionMetal = new G4SubtractionSolid("subtractionMetal", outerMetal, solidHelium);
    G4LogicalVolume *logicMetal = new G4LogicalVolume(subtractionMetal, stainlessSteelMat, "logicMetal");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicMetal, "physMetal", detectorContainerLV, false, 0);

    logicMetal->SetVisAttributes(visBlue);

    G4SubtractionSolid *subtractionPoly = new G4SubtractionSolid("subtractionPoly", box1_rot, outerMetal);
    G4LogicalVolume *logicPoly = new G4LogicalVolume(subtractionPoly, polyMat, "logicPoly");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicPoly, "physPoly", detectorContainerLV, false, 0);

    logicPoly->SetVisAttributes(visCyan);
}

