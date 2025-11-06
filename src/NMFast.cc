#include "NMFast.hh"

NMFast::NMFast(G4NistManager *nistMan, G4Material *wMat, G4LogicalVolume *wLV, G4double sPosZ, G4double sDepth) {
    nist = nistMan;
    worldMat = wMat;
    worldLV = wLV;

    detContainerHalfSize = G4ThreeVector(17.5 * cm, 50 * cm, 17.5 * cm);
    detContainerPos = G4ThreeVector(0, 0, sPosZ + sDepth / 2 + detectorPosZ + detContainerHalfSize.z());

    detectorType = "NMFast";

    init();
}


void NMFast::Construct() {
    G4Box *box1_rot = new G4Box("box1_rot", 5 * cm, 5 * cm, 40 * cm);
    G4Box *box2 = new G4Box("box2", 10 * cm, 40 * cm, 10 * cm);
    G4Box *box3 = new G4Box("box3", 17.5 * cm, 50 * cm, 17.5 * cm);

    G4Tubs *solidHelium = new G4Tubs("solidHelium", 0, 24.5 * mm, 39.95 * cm, 0, 360 * deg);
    logicSD = new G4LogicalVolume(solidHelium, He3Mat, "logicSD");
    G4RotationMatrix *rotCylinder = new G4RotationMatrix();
    rotCylinder->rotateX(90 * deg);
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicSD, "physHelium",
                      detectorContainerLV, false, 0);

    logicSD->SetVisAttributes(visWhite);

    G4Tubs *outerMetal = new G4Tubs("outerMetal", 0, 25 * mm, 40 * cm, 0, 360 * deg);
    G4SubtractionSolid *subtractionMetal = new G4SubtractionSolid("subtractionMetal", outerMetal, solidHelium);
    G4LogicalVolume *logicMetal = new G4LogicalVolume(subtractionMetal, stainlessSteelMat, "logicMetal");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicMetal, "physMetal",
                      detectorContainerLV, false, 0);

    logicMetal->SetVisAttributes(visBlue);

    G4SubtractionSolid *subtractionPoly1 = new G4SubtractionSolid("subtractionPoly1", box3, box2);
    G4LogicalVolume *logicPoly1 = new G4LogicalVolume(subtractionPoly1, polyMat, "logicPoly1");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicPoly1, "physPoly1", detectorContainerLV,
                      false, 0);

    G4SubtractionSolid *subtractionPoly2 = new G4SubtractionSolid("subtractionPoly2", box1_rot, outerMetal);
    G4LogicalVolume *logicPoly2 = new G4LogicalVolume(subtractionPoly2, polyMat, "logicPoly2");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicPoly2, "physPoly2",
                      detectorContainerLV, false, 0);

    logicPoly1->SetVisAttributes(visCyan);
    logicPoly2->SetVisAttributes(visCyan);
}

