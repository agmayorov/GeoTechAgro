#include "RunAct.hh"


RunAct::RunAct() : fileName("GeoTechAgro.root") {
    fAnalysisManager = new AnalysisManager(fileName);
}

RunAct::~RunAct() {
    delete fAnalysisManager;
}

void RunAct::BeginOfRunAction(const G4Run *) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile(fileName);
}

void RunAct::EndOfRunAction(const G4Run *) {
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
