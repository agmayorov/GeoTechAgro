#ifndef PrimaryPartCRY_h
#define PrimaryPartCRY_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "Geometry.hh"
#include <G4PrimaryParticle.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4ThreeVector.hh>
#include <memory>
#include <string>
#include <unordered_map>

#include "G4ParticleGun.hh"
#include "G4Neutron.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"

#include "Geometry.hh"

class PrimaryPartCRY : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryPartCRY(Geometry *, const G4String &);
    ~PrimaryPartCRY() = default;

    void GeneratePrimaries(G4Event *) override;
    G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
    G4ParticleGun* fParticleGun = nullptr;
    Geometry* fGeometry = nullptr;

    double L_SCALE = 1.0;
    double T_SCALE = 1.0;
    double E_SCALE = 1.0;

    std::unique_ptr<CRYSetup> fSetup;
    std::unique_ptr<CRYGenerator> fGen;
};

#endif
