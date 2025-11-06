#include "CsITl.hh"

G4Material* CreateCsITl()
{
	auto nist = G4NistManager::Instance();

    G4Material* CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    const G4int nEntries = 2;
    G4double photonEnergy[nEntries] = {2.0*eV, 3.5*eV};

    G4double rIndex[nEntries] = {1.8, 1.8};
    G4double absorption[nEntries] = {40*cm, 40*cm};
    G4double scintEmission[nEntries] = {1.0, 1.0};

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",        photonEnergy, rIndex,       nEntries);
    mpt->AddProperty("ABSLENGTH",     photonEnergy, absorption,   nEntries);
    mpt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintEmission, nEntries);

    mpt->AddConstProperty("SCINTILLATIONYIELD", 54000./MeV);
    mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1000.*ns);
    mpt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);

    CsI->SetMaterialPropertiesTable(mpt);

    return CsI;
}
