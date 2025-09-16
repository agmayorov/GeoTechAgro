# GeoTechAgro

Simulation of realistic soil and atmosphere in GEANT4

## How to run

Clone the repository to any directory and open that folder in the terminal. Run commands below:

```
mkdir build && cd build
cmake ..
make -j($nproc)
./GeoTechAgro -m ../run.mac
```


### Cli commands

- `-i`, `-input` &#8594; Specify a macro file path.
- `-d`, `-detector` &#8594; Specify a type of detector to use. DEFAULT MilliNM. AVAILABLE: MilliNM, NMEpithermal, NMThermal, NMFast.
- `-t`, `-threads` &#8594; Specify number of threads to use. DEFAULT maximum number of available threads.
- `-a`, `-atmosphere` &#8594; Specify number of layers in atmosphere. DEFAULT 10.
- `-f`, `-filepath` &#8594; Specify a soil composition file path.
- `-s`, `-size` &#8594; Specify geometry size in meters. DEFAULT 10m.
- `-m`, `-moisture` &#8594; Specify soil moisture percentage. DEFAULT 0%.
- `-h`, `-humidity` &#8594; Specify relative humidity percentage. DEFAULT 0%.
- `-v`, `-vegetation` &#8594; Specify vegetation height in cm. DEFAULT 0cm.
- `-temp`, `-temperature` &#8594; Specify temperature in &deg;C. DEFAULT 20&deg;C.
- `-cry` &#8594; Specify a CRY library configuration file path.
- `-noUI` &#8594; Disable GUI.


### Example of usage
```bash
./GeoTechAgro -i ../run.mac -s 100 -f ../soil_example.in -a 5 -t 20 -h 30 -m 5 -v 40 -d NMFast -temp 15 -cry ../cry_config.in -noUI
```

### How to install CRY Library

Download source code of [CRY Library](https://nuclear.llnl.gov/simulation/cry_v1.7.tar.gz) and unpack archive. Open that folder in terminal. Run command
```make```. Then add this environmental variables:
```aiignore
CRYHOME=/path/to/CRY
CRYDATAPATH=/path/to/CRY/data
```
