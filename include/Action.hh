#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "EventAct.hh"
#include "PrimaryPartCRY.hh"
#include "RunAct.hh"
#include "Geometry.hh"

class Action : public G4VUserActionInitialization {
public:
    Action(Geometry *, const G4String &);
    ~Action() override = default;

    void BuildForMaster() const override;
    void Build() const override;

private:
    Geometry *fGeometry = nullptr;
    G4String fConfigCRY;
};

#endif