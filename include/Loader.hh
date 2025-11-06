#ifndef FINAL_LOADER_HH
#define FINAL_LOADER_HH

#include <G4GlobalConfig.hh>
#include <G4StepLimiter.hh>
#include <G4UserLimits.hh>
#include <G4UImanager.hh>
#include <G4VisManager.hh>
#include <G4ParallelWorldPhysics.hh>
#include <G4ParallelWorldScoringProcess.hh>
#include <G4PhysicsListHelper.hh>
#include <G4ParticleTable.hh>
#include <G4UImanager.hh>
#include <QGSP_BERT_HP.hh>
#include <G4StepLimiterPhysics.hh>

#include <iostream>
#include <iomanip>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Geometry.hh"
#include "Action.hh"
#include <G4Types.hh>
#include <globals.hh>

#ifdef G4MULTITHREADED
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif

class Loader {
private:
    G4String soilFile;
    G4String macroFile;
    int numThreads;
    bool useUI;
    G4int numLayers;
    G4double soilSize;
    G4double airHumidity;
    G4double soilMoisture;
    G4double temperature;
    G4double vegetation;
    G4String detectorType;

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager;
#else
    G4RunManager* runManager;
#endif

    G4VisManager *visManager;
public:
    Loader(int argc, char **argv);
    ~Loader();

private:
    G4String configPathCry;

    static G4String trim(const G4String &);
    G4String LoadConfigCRY();
};

#endif //FINAL_LOADER_HH
