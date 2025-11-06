#include "Geometry.hh"

Geometry::Geometry(const G4String &soilFile,
                   const G4int nLayers,
                   const G4double sSize,
                   const G4double sMoisture,
                   const G4double aHumidity,
                   const G4double temp,
                   const G4double veg,
                   G4String detType) : numLayers(nLayers),
                                       soilSize(sSize),
                                       soilMoisture(sMoisture),
                                       airHumidity(aHumidity),
                                       temperature(temp),
                                       vegetation(veg),
                                       detectorType(std::move(detType)) {

    std::vector<G4String> detectorList = {"MilliNM", "NMFast", "NMEpithermal", "NMThermal", "Gamma"};
    if (std::find(detectorList.begin(), detectorList.end(), detectorType) == detectorList.end()) {
        G4Exception("Geometry::ConstructDetector", "DetectorType", FatalException,
                    ("Detector not found: " + detectorType +
                     ".\nAvailable detectors: MilliNM, NMFast, NMEpithermal, NMThermal, Gamma").c_str());
    }

    nist = G4NistManager::Instance();

    water = nist->FindOrBuildMaterial("G4_WATER");

    ReadMaterialFile(soilFile);
    DefineSoilMaterial();

    ComputeAtmosphereMaterials();

    soilDepth = 2 * m;
    atmosphereSize = soilSize;
    atmosphereHeight = 100 * m;

    layerHeight = atmosphereHeight / G4double(numLayers);

    simulationSizeZ = soilDepth + atmosphereHeight + vegetation;

    zeroRot = new G4RotationMatrix(0, 0, 0);
}


void Geometry::ReadMaterialFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        G4Exception("Geometry::ReadMaterialFile", "FileNotFound", FatalException,
                    ("Cannot open file: " + filename).c_str());
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string element;
        double percentage;
        if (!(iss >> element >> percentage)) {
            G4cerr << "Invalid line in material file: " << line << G4endl;
            continue;
        }
        materialComposition.emplace_back(element, percentage);
    }

    double count = 0;
    for (auto &material: materialComposition) {
        count += material.second;
    }

    if (round(count) != 100.) {
        G4Exception("Geometry::ReadMaterialFile", "WrongComposition", FatalException,
                    (("Wrong composition, correct materials ratio: " + std::to_string(count))).c_str());
        return;
    }
}


void Geometry::DefineSoilMaterial() {
    G4double drySoilDensity = 2.5 * g / cm3;

    G4int nElements = materialComposition.size();
    if (nElements == 0) {
        G4Exception("Geometry::DefineSoilMaterial", "EmptyComposition", FatalException,
                    "Material composition is empty.");
    }

    drySoilMat = new G4Material("DrySoil", drySoilDensity, nElements);

    for (const auto &[symbol, percent]: materialComposition) {
        G4Element *element = nist->FindOrBuildElement(symbol);
        if (!element) {
            G4Exception("Geometry::DefineSoilMaterial", "InvalidElement", JustWarning,
                        ("Element not found: " + symbol).c_str());
            continue;
        }
        drySoilMat->AddElement(element, percent / 100.0);
    }

    if (soilMoisture == 0.) {
        soilMat = drySoilMat;
    } else {
        G4double waterDensity = 1. * g / cm3;
        G4double wetSoilDensity = (1. + soilMoisture) / (soilMoisture / waterDensity + 1. / drySoilDensity);
        soilMat = new G4Material("Soil", wetSoilDensity, 2);
        soilMat->AddMaterial(water, soilMoisture / (1. + soilMoisture));
        soilMat->AddMaterial(drySoilMat, 1. / (1. + soilMoisture));
    }

    std::ofstream debug("soil_debug.log");
    debug << "Soil moisture: " << soilMoisture << std::endl;
    debug << "Soil density: " << soilMat->GetDensity() / (g/cm3) << " g/cm3" << std::endl;
    debug.close();
}


void Geometry::ComputeAtmosphereMaterials() {
    atmosphereMaterials.clear();

    G4Element *N = nist->FindOrBuildElement("N");
    G4Element *O = nist->FindOrBuildElement("O");
    G4Element *Ar = nist->FindOrBuildElement("Ar");

    for (int i = 0; i < numLayers; ++i) {
        G4double height = layerHeight * i;
        G4double T_K = temperature + 273.15 * kelvin;
        G4double P0 = 101325.0 * pascal;
        G4double gravity = 9.80665 * m / s / s;
        G4double M = 0.0289644 * kg / mole;
        G4double R = 8.3144598 * joule / (mole * kelvin);

        G4double P = P0 * std::exp(-gravity * M * height / (R * T_K));
        G4double P_sat = 610.94 * pascal * std::exp((17.625 * temperature) / (temperature + 243.04));
        G4double P_w = airHumidity * P_sat;
        G4double P_d = P - P_w;

        const G4double R_d = 287.058 * joule / (kg * kelvin);
        const G4double R_w = 461.495 * joule / (kg * kelvin);

        G4double w = 0.622 * P_w / P_d;
        G4double rho = (P_d / (R_d * T_K)) + (P_w / (R_w * T_K));

        G4Material *dryAirMat = new G4Material("DryAir_" + std::to_string(i), rho, 3);
        dryAirMat->AddElement(N, 0.78);
        dryAirMat->AddElement(O, 0.21);
        dryAirMat->AddElement(Ar, 0.01);

        G4Material *airMat = new G4Material("Air_" + std::to_string(i), rho, 2);
        airMat->AddMaterial(dryAirMat, 1 - w);
        airMat->AddMaterial(water, w);

        atmosphereMaterials.push_back(airMat);
    }
}


void Geometry::ConstructAtmBlock(const std::string &namePrefix,
                                 const G4ThreeVector &containerHalfSize,
                                 const G4ThreeVector &containerPosition) {

    G4Box *box = new G4Box(namePrefix + "Box", containerHalfSize.x(), containerHalfSize.y(), containerHalfSize.z());
    G4LogicalVolume *lv = new G4LogicalVolume(box, worldMat, namePrefix + "LV");
    new G4PVPlacement(zeroRot, containerPosition, lv, namePrefix + "PV", worldLV, false, 0, true);

    auto *param = new AtmosphereParameterisation(numLayers, layerHeight, temperature, airHumidity,
                                                 -atmosphereHeight / 2., 2 * containerHalfSize.x(),
                                                 2 * containerHalfSize.y(), atmosphereMaterials);

    G4Box *layerBox = new G4Box(namePrefix + "LayerBox", containerHalfSize.x(), containerHalfSize.y(), layerHeight / 2);
    G4LogicalVolume *layerLV = new G4LogicalVolume(layerBox, worldMat, namePrefix + "LayerLV");
    layerLV->SetVisAttributes(airVisAttr);

    new G4PVParameterised(namePrefix + "LayerPVPL", layerLV, lv, kZAxis, numLayers, param, true);
}


void Geometry::ConstructAtmosphere() {

    G4double zOffsetContainerInWorld = (soilDepth + vegetation) / 2.0;

    airVisAttr = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1));
    airVisAttr->SetForceSolid(true);

    // Left and Right
    G4double yLR = atmosphereSize / 2 - detectorHalfSize.y();
    G4ThreeVector halfSizeLR(atmosphereSize / 2, yLR / 2, atmosphereHeight / 2);

    ConstructAtmBlock("LeftAtm", halfSizeLR,
                      G4ThreeVector(0, -(atmosphereSize / 2 + detectorHalfSize.y()) / 2, zOffsetContainerInWorld));

    ConstructAtmBlock("RightAtm", halfSizeLR,
                      G4ThreeVector(0, (atmosphereSize / 2 + detectorHalfSize.y()) / 2, zOffsetContainerInWorld));

    // Front and Back
    G4double xFB = atmosphereSize / 2 - detectorHalfSize.x();
    G4double yFB = 2 * detectorHalfSize.y();
    G4ThreeVector halfSizeFB(xFB / 2, yFB / 2, atmosphereHeight / 2);

    ConstructAtmBlock("FrontAtm", halfSizeFB,
                      G4ThreeVector((atmosphereSize / 2 + detectorHalfSize.x()) / 2, 0, zOffsetContainerInWorld));

    ConstructAtmBlock("BackAtm", halfSizeFB,
                      G4ThreeVector(-(atmosphereSize / 2 + detectorHalfSize.x()) / 2, 0, zOffsetContainerInWorld));

    // Bottom
    G4double bottomAtmHeight = simulationSizeZ / 2 + detectorPos.z() - detectorHalfSize.z() - soilDepth - vegetation;
    G4int bottomNumLayers = (G4int) (bottomAtmHeight / layerHeight);
    G4double bottomExtraLayerHeigh = bottomAtmHeight - bottomNumLayers * layerHeight;

    for (int i = 0; i < bottomNumLayers; i++) {
        G4Box *layerBox = new G4Box("BottomAtmLayerBox_" + std::to_string(i), detectorHalfSize.x(),
                                    detectorHalfSize.y(), layerHeight / 2);
        G4LogicalVolume *layerLV = new G4LogicalVolume(layerBox, atmosphereMaterials[i],
                                                       "BottomAtmLayerLV" + std::to_string(i));
        G4ThreeVector layerPos = G4ThreeVector(0, 0, (soilDepth + vegetation - atmosphereHeight) / 2 +
                                                     layerHeight * (i + 0.5));
        new G4PVPlacement(zeroRot, layerPos, layerLV, "BottomAtmLayer", worldLV, false, i, true);

        layerLV->SetVisAttributes(airVisAttr);
    }
    if (bottomExtraLayerHeigh > 0) {
        G4Box *layerBox = new G4Box("BottomAtmLayerBox_" + std::to_string(bottomNumLayers), detectorHalfSize.x(),
                                    detectorHalfSize.y(), bottomExtraLayerHeigh / 2);
        G4LogicalVolume *layerLV = new G4LogicalVolume(layerBox, atmosphereMaterials[bottomNumLayers],
                                                       "BottomAtmLayerLV_" + std::to_string(bottomNumLayers));
        G4ThreeVector layerPos = G4ThreeVector(0, 0, (soilDepth + vegetation - atmosphereHeight) / 2 +
                                                     layerHeight * bottomNumLayers + bottomExtraLayerHeigh / 2);
        new G4PVPlacement(zeroRot, layerPos, layerLV, "BottomAtmLayerPVPL", worldLV, false, bottomNumLayers, true);

        layerLV->SetVisAttributes(airVisAttr);
    }

    // Top
    G4double topAtmHeight = atmosphereHeight - bottomAtmHeight - detectorHalfSize.z() * 2.;
    G4int topNumLayers = (G4int) (topAtmHeight / layerHeight);
    G4double topExtraLayerHeigh = topAtmHeight - topNumLayers * layerHeight;

    for (int i = numLayers - 1; i > numLayers - topNumLayers - 1; i--) {
        G4Box *layerBox = new G4Box("TopAtmLayerBox_" + std::to_string(i), detectorHalfSize.x(),
                                    detectorHalfSize.y(), layerHeight / 2);
        G4LogicalVolume *layerLV = new G4LogicalVolume(layerBox, atmosphereMaterials[i],
                                                       "TopAtmLayerLV" + std::to_string(i));
        G4ThreeVector layerPos = G4ThreeVector(0, 0, (soilDepth + vegetation - atmosphereHeight) / 2 +
                                                     layerHeight * (i + 0.5));
        new G4PVPlacement(zeroRot, layerPos, layerLV, "TopAtmLayer", worldLV, false, numLayers - i - 1, true);

        layerLV->SetVisAttributes(airVisAttr);
    }
    if (topExtraLayerHeigh > 0) {
        G4Box *layerBox = new G4Box("TopAtmLayerBox_" + std::to_string(numLayers - topNumLayers - 1),
                                    detectorHalfSize.x(), detectorHalfSize.y(), topExtraLayerHeigh / 2);
        G4LogicalVolume *layerLV = new G4LogicalVolume(layerBox, atmosphereMaterials[numLayers - topNumLayers - 1],
                                                       "TopAtmLayerLV_" + std::to_string(numLayers - topNumLayers));
        G4ThreeVector layerPos = G4ThreeVector(0, 0, (soilDepth + vegetation - atmosphereHeight) / 2 +
                                                     layerHeight * (numLayers - topNumLayers - 1) +
                                                     topExtraLayerHeigh / 2 + detectorHalfSize.z() * 2 +
                                                     bottomExtraLayerHeigh);
        new G4PVPlacement(zeroRot, layerPos, layerLV, "TopAtmLayerPVPL", worldLV, false, numLayers - topNumLayers - 1,
                          true);

        layerLV->SetVisAttributes(airVisAttr);
    }
}


void Geometry::ConstructVegetation() {
    if (vegetation == 0 * cm) {
        return;
    }

    dryVegetationMat = new G4Material("DryVegetation", 0.5 * g / cm3, 4);
    dryVegetationMat->AddElement(nist->FindOrBuildElement("C"), 0.485);
    dryVegetationMat->AddElement(nist->FindOrBuildElement("H"), 0.060);
    dryVegetationMat->AddElement(nist->FindOrBuildElement("O"), 0.400);
    dryVegetationMat->AddElement(nist->FindOrBuildElement("N"), 0.055);

    vegetationMat = new G4Material("Vegetation", 0.8 * g / cm3, 2);
    vegetationMat->AddMaterial(water, soilMoisture / (1. + soilMoisture));
    vegetationMat->AddMaterial(dryVegetationMat, 1. / (1. + soilMoisture));

    vegetationBox = new G4Box("Vegetation", 0.5 * soilSize, 0.5 * soilSize, 0.5 * vegetation);
    vegetationLV = new G4LogicalVolume(vegetationBox, vegetationMat, "VegetationLV");
    vegetationPos = G4ThreeVector(0., 0., -(atmosphereHeight - soilDepth) / 2.0);
    new G4PVPlacement(zeroRot, vegetationPos, vegetationLV, "VegetationPVPL", worldLV, false, 0, true);

    vegetationVisAttr = new G4VisAttributes(G4Colour(1., 1., 0., 0.5));
    vegetationVisAttr->SetForceSolid(true);
    vegetationLV->SetVisAttributes(vegetationVisAttr);
}


void Geometry::ConstructSoil() {
    soilBox = new G4Box("Soil", 0.5 * soilSize, 0.5 * soilSize, 0.5 * soilDepth);
    soilLV = new G4LogicalVolume(soilBox, soilMat, "SoilLV");
    soilPos = G4ThreeVector(0., 0., -(atmosphereHeight + vegetation) / 2.0);
    new G4PVPlacement(zeroRot, soilPos, soilLV, "SoilPVPL", worldLV, false, 0, true);

    soilVisAttr = new G4VisAttributes(G4Colour(0.545, 0.271, 0.075));
    soilVisAttr->SetForceSolid(true);
    soilLV->SetVisAttributes(soilVisAttr);
}


void Geometry::ConstructDetector() {
    if (detectorType == "MilliNM") {
        detector = new MilliNM(nist, worldMat, worldLV, soilPos.z(), soilDepth);
    } else if (detectorType == "NMFast") {
        detector = new NMFast(nist, worldMat, worldLV, soilPos.z(), soilDepth);
    } else if (detectorType == "NMEpithermal") {
        detector = new NMEpithermal(nist, worldMat, worldLV, soilPos.z(), soilDepth);
    } else if (detectorType == "NMThermal") {
        detector = new NMThermal(nist, worldMat, worldLV, soilPos.z(), soilDepth);
    } else if (detectorType == "Gamma") {
        detector = new Gamma(nist, worldMat, worldLV, soilPos.z(), soilDepth);
    }
    detector->Construct();
    detectorPos = detector->GetDetectorPos();
    detectorHalfSize = detector->GetDetectorHalfSize();
    logicSD = detector->GetSensitiveLV();
}


G4VPhysicalVolume *Geometry::Construct() {
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    worldSizeXY = 1.1 * soilSize;
    worldSizeZ = 1.1 * simulationSizeZ;
    worldBox = new G4Box("World", 0.5 * worldSizeXY, 0.5 * worldSizeXY, 0.5 * worldSizeZ);
    worldLV = new G4LogicalVolume(worldBox, worldMat, "WorldLV");
    worldPVP = new G4PVPlacement(zeroRot, G4ThreeVector(0, 0, 0), worldLV, "WorldPVPL", 0, false, 0, false);
    worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    ConstructSoil();
    ConstructVegetation();
    ConstructDetector();
    ConstructAtmosphere();

    return worldPVP;
}


void Geometry::ConstructSDandField() {
    SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector", detectorType);
    logicSD->SetSensitiveDetector(sensDet);
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(sensDet);
}