## Tutorial at E1039 11/2018 Collaboration Meeting

### Log in to seaquestgpvm01

```bash
kinit -f <usr>@FNAL.GOV
```
type your password, then

```bash
ssh -AKX -l krbtgt/FNAL.GOV@FNAL.GOV <usr>@seaquestgpvm01.fnal.gov
```

### source the setup macro

```bash
source /e906/app/users/yuhw/setup.sh
```

### git clone the tutorial repository

If you have a GitHub account and have your publick key uploaded to GitHub.

Refer [Connecting to GitHub with SSH](https://help.github.com/articles/connecting-to-github-with-ssh/)
and [this E1039 wiki page](https://github.com/E1039-Collaboration/e1039-wiki/wiki/Work-with-GitHub)
```bash
git clone git@github.com:E1039-Collaboration/e1039-tutorial.git
```

If not try
```bash
source /grid/fermiapp/products/uboone/setup_uboone.sh
setup git
source /e906/app/users/yuhw/setup.sh
git clone https://github.com/E1039-Collaboration/e1039-tutorial.git
```

### Run the simulation

```bash
root -l Fun4Tutorial.C
```

### (Optional) Run it with GEANT4 event display
```bash
root -l int_run.C
```

### Check the 'Fun4Tutorial.C' file

### Experiment 1 - Enable the output manager and inspect the output DST file

### Experiment 2 - Export the geometry file and inspect the geometry using 'disp_geom.C'

### Experiment 3 - Compile a analysis module and run it

```bash
mkdir build
mkdir install
cmake -DCMAKE_INSTALL_PREFIX=<path-to-install> ../SimpleAna/
make
make install
```
A dynamic library, 'libsimple_ana.so' should be generated and installed at <path-to-install/lib>


Add <path-to-install>/lib to your LD_LIBRARY_PATH
```bash
export LD_LIBRARY_PATH=<path-to-install>/lib/:$LD_LIBRARY_PATH
```

Edit the Experiment 3 part to enable the SimpleAna module, run it again.
Check the output 'eval.root'

### Inspect the 'SimpleAna' module





