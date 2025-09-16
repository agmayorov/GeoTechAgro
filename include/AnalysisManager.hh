#ifndef AnalysisManager_h
#define AnalysisManager_h 1

#include "G4AnalysisManager.hh"
#include "globals.hh"

class AnalysisManager {
public:
    AnalysisManager(const std::string &);
    ~AnalysisManager() = default;


private:
    G4String fFileName = "SoilSimulation";

    void Book();
};


#endif
