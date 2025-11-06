#include "PrimaryPartCRY.hh"

static constexpr G4double CRY_E_TO_G4 = MeV;
static constexpr G4double CRY_L_TO_G4 = m;
static constexpr G4double CRY_T_TO_G4 = s;

static G4ParticleDefinition *FindPDGfromCRY(const std::string &name) {
    std::string g4name = name;
    if (name == "muon+") g4name = "mu+";
    else if (name == "muon-") g4name = "mu-";
    else if (name == "electron") g4name = "e-";
    else if (name == "positron") g4name = "e+";
    else if (name == "photon" || name == "gamma") g4name = "gamma";
    return G4ParticleTable::GetParticleTable()->FindParticle(g4name);
}

PrimaryPartCRY::PrimaryPartCRY(Geometry *geom, const G4String &cryConfig) : fGeometry(geom) {
    std::string dataPath;
    if (const char *env = std::getenv("CRYHOME")) {
        dataPath = std::string(env) + "/data";
    } else {
        G4Exception("PrimaryGeneratorCRY", "NoCRYHOME", FatalException,
                    "CRYHOME is not set and cryDataPath is empty");
    }

    fSetup = std::make_unique<CRYSetup>(cryConfig, dataPath);
    fGen = std::make_unique<CRYGenerator>(fSetup.get());

}

void PrimaryPartCRY::GeneratePrimaries(G4Event *event) {
    std::vector<CRYParticle *> shower;
    fGen->genEvent(&shower);

    auto* analysisManager = G4AnalysisManager::Instance();

    for (CRYParticle *p: shower) {
        const int pdg = p->PDGid();
        G4ParticleDefinition *def = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
        if (!def) {
            delete p;
            continue;
        }
    if (p->PDGid() == 2112) {   // 2112 = neutron
            analysisManager->FillNtupleDColumn(1, 0, p->ke());
            analysisManager->AddNtupleRow(1);
        }

        if (p->PDGid() == 2212) {   // 2212 = proton
            analysisManager->FillNtupleDColumn(2, 0, p->ke());
            analysisManager->AddNtupleRow(2);
        }

        const G4double x = p->x();
        const G4double y = p->y();
        const G4double z = p->z();

        const G4double ux = p->u();
        const G4double uy = p->v();
        const G4double uz = p->w();

        const G4double ke = p->ke();
        const G4double tt = p->t();

        G4ThreeVector worldPos(x * CRY_L_TO_G4, y * CRY_L_TO_G4, z * CRY_L_TO_G4);

        //worldPos.setZ((fGeometry->atmosphereHeight - fGeometry->soilDepth - fGeometry->vegetation) / 2
                      //+ 1 * m);

        worldPos.setZ(-48*m);

        G4ThreeVector dir(ux, uy, uz);
        if (dir.z() > 0.0) {
            dir = -dir;
        }

        //G4ThreeVector dir(0, 0, -1);

        const G4double t0 = tt * CRY_T_TO_G4;
        const G4double Ekin = ke * CRY_E_TO_G4;

        auto *vtx = new G4PrimaryVertex(worldPos, t0);
        auto *prim = new G4PrimaryParticle(def);
        prim->SetKineticEnergy(Ekin);
        prim->SetMomentumDirection(dir);

        vtx->SetPrimary(prim);
        event->AddPrimaryVertex(vtx);

        delete p;
    }
}
