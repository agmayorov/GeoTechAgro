#include "BrickParameterisation.hh"

BrickParameterisation::BrickParameterisation() {}

BrickParameterisation::~BrickParameterisation() {}

void BrickParameterisation::ComputeTransformation(G4int copyNo, G4VPhysicalVolume *physVol) const {
    G4double xPos = 0, yPos = 0, zPos = 0;
    G4RotationMatrix *noRot = new G4RotationMatrix();
    G4RotationMatrix *rot90 = new G4RotationMatrix();

    rot90->rotateY(90 * deg);
    rot90->rotateZ(90 * deg);

    G4RotationMatrix *rot;

    if (copyNo < 8) {
        yPos = -35 * cm + copyNo * 10 * cm;
        zPos = -7.5 * cm;
        rot = noRot;
    } else if (copyNo < 16) {
        yPos = -35 * cm + (copyNo - 8) * 10 * cm;
        zPos = 7.5 * cm;
        rot = noRot;
    } else if (copyNo < 20) {
        yPos = -30 * cm + (copyNo - 16) * 20 * cm;
        xPos = -7.5 * cm;
        rot = rot90;;
    } else {
        yPos = -30 * cm + (copyNo - 20) * 20 * cm;
        xPos = 7.5 * cm;
        rot = rot90;
    }

    physVol->SetTranslation(G4ThreeVector(xPos, yPos, zPos));
    physVol->SetRotation(rot);
}
