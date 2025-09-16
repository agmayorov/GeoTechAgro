#include "Action.hh"


Action::Action(Geometry *geom, const G4String &confCRY) : fGeometry(geom), fConfigCRY(confCRY) {}

void Action::BuildForMaster() const {
    RunAct* runAct = new RunAct();
    SetUserAction(runAct);
}

void Action::Build() const {
    RunAct* runAct = new RunAct();
    SetUserAction(runAct);

    PrimaryPartCRY* primary = new PrimaryPartCRY(fGeometry, fConfigCRY);
    SetUserAction(primary);

//    EventAct* event = new EventAct();
//    SetUserAction(event);

    StepAct* stepAct = new StepAct(fGeometry->detectorType);
    SetUserAction(stepAct);
}
