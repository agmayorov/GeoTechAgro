{
    TFile f("GeoTechAgro.root");
    
    TNtuple* ntupleEdep = (TNtuple*)f.Get("EnergyDeposit");
    
    if (!ntupleEdep) {
        std::cout << "Error: Could not load NTuples!" << std::endl;
        return;
    }

    double minEnergy = 150.;

    ntupleEdep->Draw(">>eventList", Form("Edep > %f", minEnergy), "goff");
    TEventList* eventList = (TEventList*)gDirectory->Get("eventList");
    double registeredNeutronCount = eventList->GetN();
    std::cout << "Number of registered events: " << registeredNeutronCount << std::endl;
    
    delete eventList;
}
