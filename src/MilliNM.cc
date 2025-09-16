#include "MilliNM.hh"

MilliNM::MilliNM(G4NistManager *nistMan, G4Material *wMat, G4LogicalVolume *wLV, G4double sPosZ, G4double sDepth) {
    nist = nistMan;
    worldMat = wMat;
    worldLV = wLV;

    detContainerHalfSize = G4ThreeVector(17.5 * cm, 50 * cm, 17.5 * cm);
    detContainerPos = G4ThreeVector(0, 0, sPosZ + sDepth / 2 + detectorPosZ);

    detectorType = "MilliNM";

    init();
}

void MilliNM::Construct() {
    G4Box *box1 = new G4Box("box1", 5 * cm, 40 * cm, 5 * cm);
    G4Box *box1_rot = new G4Box("box1_rot", 5 * cm, 5 * cm, 40 * cm);
    G4Box *box2 = new G4Box("box2", 10 * cm, 40 * cm, 10 * cm);
    G4Box *box3 = new G4Box("box3", 17.5 * cm, 40 * cm, 17.5 * cm);

    G4SubtractionSolid *subtractionContainer = new G4SubtractionSolid("subtractionContainer", box2, box1);
    G4LogicalVolume *logicContainer = new G4LogicalVolume(subtractionContainer, worldMat, "logicContainer");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicContainer, "physContainer",
                      detectorContainerLV, false, 0);
    logicContainer->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4double brickLength = 20. * cm;
    G4double brickWidth = 10. * cm;
    G4double brickHeight = 5. * cm;

    G4Box *solidPbBrick = new G4Box("solidPbBrick", brickLength / 2, brickWidth / 2, brickHeight / 2);
    G4LogicalVolume *logicPbBrick = new G4LogicalVolume(solidPbBrick, leadMat, "logicPbBrick");

    logicPbBrick->SetVisAttributes(visGrey);

    new G4PVParameterised("BoxWalls", logicPbBrick, logicContainer, kUndefined, 24, new BrickParameterisation(), true);

    G4Tubs *solidHelium = new G4Tubs("solidHelium", 0, 24.5 * mm, 39.95 * cm, 0, 360 * deg);
    logicHelium = new G4LogicalVolume(solidHelium, He3Mat, "logicHelium");
    G4RotationMatrix *rotCylinder = new G4RotationMatrix();
    rotCylinder->rotateX(90 * deg);
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicHelium, "physHelium",
                      detectorContainerLV, false, 0);

    logicHelium->SetVisAttributes(visWhite);

    G4Tubs *outerMetal = new G4Tubs("outerMetal", 0, 25 * mm, 40 * cm, 0, 360 * deg);
    G4SubtractionSolid *subtractionMetal = new G4SubtractionSolid("subtractionMetal", outerMetal, solidHelium);
    G4LogicalVolume *logicMetal = new G4LogicalVolume(subtractionMetal, stainlessSteelMat, "logicMetal");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicMetal, "physMetal",
                      detectorContainerLV, false, 0);

    logicMetal->SetVisAttributes(visBlue);

    G4SubtractionSolid *subtractionPoly1 = new G4SubtractionSolid("subtractionPoly1", box3, box2);
    G4LogicalVolume *logicPoly1 = new G4LogicalVolume(subtractionPoly1, polyMat, "logicPoly1");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicPoly1, "physPoly1", detectorContainerLV,
                      false, 0);

    G4SubtractionSolid *subtractionPoly2 = new G4SubtractionSolid("subtractionPoly2", box1_rot, outerMetal);
    G4LogicalVolume *logicPoly2 = new G4LogicalVolume(subtractionPoly2, polyMat, "logicPoly2");
    new G4PVPlacement(rotCylinder, G4ThreeVector(0, 0, 0), logicPoly2, "physPoly2",
                      detectorContainerLV, false, 0);

    G4Box *polyPlast = new G4Box("polyPlast", 17.5 * cm, 5 * cm, 17.5 * cm);
    G4LogicalVolume *logicPoly3 = new G4LogicalVolume(polyPlast, polyMat, "logicPoly2");
    new G4PVPlacement(nullptr, G4ThreeVector(0, -45 * cm, 0), logicPoly3, "physPoly3", detectorContainerLV, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 45 * cm, 0), logicPoly3, "physPoly4", detectorContainerLV, false, 1);

    logicPoly1->SetVisAttributes(visCyan);
    logicPoly2->SetVisAttributes(visCyan);
    logicPoly3->SetVisAttributes(visCyan);
}

