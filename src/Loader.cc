#include "Loader.hh"


Loader::Loader(int argc, char **argv) {
    numThreads = G4Threading::G4GetNumberOfCores();
    soilFile = "../soil_example.in";
    numLayers = 10;
    useUI = true;
    soilSize = 10 * m;
    macroFile = "run.mac";
    airHumidity = 0;
    soilMoisture = 0;
    temperature = 20;
    vegetation = 0 * cm;
    detectorType = "MilliNM";
    configPathCry = "../cry_setup.in";

    for (int i = 0; i < argc; i++) {
        std::string input(argv[i]);
        if (input == "-f" || input == "-filepath") {
            soilFile = argv[i + 1];
        } else if (input == "-i" || input == "-input") {
            macroFile = argv[i + 1];
            useUI = false;
        } else if (input == "-s" || input == "-size") {
            soilSize = std::stod(argv[i + 1]) * m;
        } else if (input == "-t" || input == "-threads") {
            numThreads = std::stoi(argv[i + 1]);
        } else if (input == "-noUI") {
            useUI = false;
        } else if (input == "-a" || input == "-atmosphere") {
            numLayers = std::stoi(argv[i + 1]);
        } else if (input == "-h" || input == "-humidity") {
            airHumidity = std::stod(argv[i + 1]) / 100.;
        } else if (input == "-m" || input == "-moisture") {
            soilMoisture = std::stod(argv[i + 1]) / 100.;
        } else if (input == "-temp" || input == "-temperature") {
            temperature = std::stod(argv[i + 1]);
        } else if (input == "-v" || input == "-vegetation") {
            vegetation = std::stod(argv[i + 1]) * cm;
        } else if (input == "-d" || input == "-detector") {
            detectorType = argv[i + 1];
        } else if (input == "-cry") {
            configPathCry = argv[i + 1];
        }
    }
    G4String configCRY = LoadConfigCRY();

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    CLHEP::HepRandom::setTheSeed(time(NULL));

#ifdef G4MULTITHREADED
    runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(runManager->GetNumberOfThreads());
    runManager->SetNumberOfThreads(numThreads);
#else
    runManager = new G4RunManager;
#endif
    Geometry *realWorld = new Geometry(soilFile, numLayers, soilSize, airHumidity, soilMoisture, temperature,
                                       vegetation, detectorType);
    runManager->SetUserInitialization(realWorld);
    G4VModularPhysicsList *physicsList = new QGSP_BIC_AllHP;
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new Action(realWorld, configCRY));

    visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (!useUI) {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macroFile);
    } else {
        G4UIExecutive *ui = new G4UIExecutive(argc, argv, "qt");
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
}

Loader::~Loader() {
    delete runManager;
    delete visManager;
}


G4String Loader::trim(const G4String &str) {
    size_t l = str.find_first_not_of(" \t\r\n");
    if (l == std::string::npos) return "";
    size_t r = str.find_last_not_of(" \t\r\n");
    return str.substr(l, r - l + 1);
}

G4String Loader::LoadConfigCRY() {
    std::ifstream in(configPathCry);
    if (!in) {
        throw std::runtime_error("Cannot open config: " + configPathCry);
    }

    G4String cryConfig = "";
    for (std::string line; std::getline(in, line);) {
        G4String str = trim(line);
        if (str.empty() || str[0] == '#' || str.rfind("//", 0) == 0) {
            continue;
        }
        cryConfig = cryConfig + str + " ";
    }
    return cryConfig;
}
