## Fun4All Tutorial at the E1039 Collaboration Meeting - 2018-11

### Log in to seaquestgpvm01

```bash
kinit -f <usr>@FNAL.GOV
```
type your password, then

```bash
ssh -Y <usr>@spinquestgpvm01.fnal.gov
```

### Source the setup macro and git clone the tutorial repository

If you have a GitHub account and have your public key uploaded to GitHub:

(Refer [Connecting to GitHub with SSH](https://help.github.com/articles/connecting-to-github-with-ssh/)
and [this E1039 wiki page](https://github.com/E1039-Collaboration/e1039-wiki/wiki/Work-with-GitHub))

```bash
source /e906/app/software/osg/software/e1039/this-e1039.sh
git clone https://github.com/E1039-Collaboration/e1039-tutorial.git
```

### Run the simulation

```bash
cd <path-to-e1039-tutorial>/cm1811
root -l Fun4Tutorial.C
```

### (Optional) Run it with GEANT4 event display
```bash
root -l int_run.C
```

![g4-evt-disp](https://github.com/E1039-Collaboration/e1039-tutorial/blob/master/cm1811/pic/g4-evt-disp.png)

### Inspect the '[Fun4Tutorial.C](https://github.com/E1039-Collaboration/e1039-tutorial/blob/master/cm1811/Fun4Tutorial.C)' file

### Experiment 1 - Enable the output manager and inspect the output DST file, 'DST.root'

Get a interactive ROOT session
```bash
root -l
```

Within the ROOT session
```C++
gSystem->Load("libg4detectors.so")
TFile *f = TFile::Open("DST.root")
TTree *T = (TTree*) f->Get("T")
T->Show(0)
```

You should get something like this:
```bash
======> EVENT:0
 DST.PHHepMCGenEventMap = (PHHepMCGenEventMap*)0x2570fe0
 DST.PHHepMCGenEventMap.fUniqueID = 0
 DST.PHHepMCGenEventMap.fBits = 50331648
 DST.PHHepMCGenEventMap._map = (map<int,PHHepMCGenEvent*>*)0x2570ff8
 DST.G4HIT_Sensor = (PHG4HitContainer*)0x26f3bc0
 DST.G4HIT_Sensor.fUniqueID = 0
 DST.G4HIT_Sensor.fBits = 50331648
 DST.G4HIT_Sensor.id = -884413610
 DST.G4HIT_Sensor.hitmap = (map<ULong64_t,PHG4Hit*>*)0x26f3be0
 DST.G4HIT_Sensor.layers = (set<unsigned int>*)0x26f3c10
 DST.G4TruthInfo = (PHG4TruthInfoContainer*)0x273f6a0
 DST.G4TruthInfo.fUniqueID = 0
 DST.G4TruthInfo.fBits = 50331648
 DST.G4TruthInfo.particlemap = (map<int,PHG4Particle*>*)0x273f6b8
 DST.G4TruthInfo.vtxmap = (map<int,PHG4VtxPoint*>*)0x273f6e8
 DST.G4TruthInfo.showermap = (map<int,PHG4Shower*>*)0x273f718
 DST.G4TruthInfo.particle_embed_flags = (map<int,int>*)0x273f7f0
 DST.G4TruthInfo.particle_embed_flags.first = 1, 2
 DST.G4TruthInfo.particle_embed_flags.second = 1, 1
 DST.G4TruthInfo.vertex_embed_flags = (map<int,int>*)0x273f850
 DST.G4TruthInfo.vertex_embed_flags.first = 1
 DST.G4TruthInfo.vertex_embed_flags.second = 1
```


### Experiment 2 - Export the geometry file and inspect the geometry using 'disp_geom.C'

Modify 'Fun4Tutorial.C' to enable geometry exportation, then
```bash
root -l Fun4Tutorial.C
```

You should see a root file 'geom.root' generated. Then using the 'disp_geom.C' to inspect the geometry
```bash
root -l disp_geom.C
```

![root-gl-viewer](https://github.com/E1039-Collaboration/e1039-tutorial/blob/master/cm1811/pic/root-gl-viewer.png)

### Experiment 3 - Compile an analysis module and run it

```bash
mkdir build
mkdir install

cd <path-to-build>
cmake -DCMAKE_INSTALL_PREFIX=<path-to-install> ../SimpleAna/
make
make install
```
A dynamic library, 'libsimple_ana.so' should be generated and installed at <path-to-install/lib> and header at <path-to-install/include>


 - Add `<path-to-install>/lib` to your `LD_LIBRARY_PATH`
 - Add `<path-to-install>/include` to your `ROOT_INCLUDE_PATH`
```bash
export LD_LIBRARY_PATH=<path-to-install>/lib/:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=<path-to-install>/include/:$ROOT_INCLUDE_PATH
```

Edit the Experiment 3 part to enable the SimpleAna module, run it again.
Check the output 'eval.root'

### Inspect the '[SimpleAna](https://github.com/E1039-Collaboration/e1039-tutorial/tree/master/cm1811/SimpleAna)' module





