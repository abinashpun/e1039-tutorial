## Analysis Tutorial

A simple example of building an analysis module in Fun4All framework. This shows how to extract physics information
from the NodeTree, and produce a root file that can be used for further analysis. In this package, truth particle information 
and tracking information are extracted into ROOT Tree format.

## login, setup and checkout the code

init kerberos key, `<usr>` below means your user name.
```
kinit -f <usr>@FNAL.GOV
<type your password>
```

login to a submitter node
```
ssh -Y <usr>@spinquestgpvm01.fnal.gov
```
## Building

The analysis code is located in `e1039-tutorial/AnaTutorial/src`.
The following commands set up the shell environment and compile the code;
```
cd e1039-tutorial/AnaTutorial
source setup.sh
cd build
cmake-ana-tutorial
make install
```
You are expected to later look into the script and the analysis code to make your own analysis logic.

### Running the analysis macro

The running macro is located in `e1039-tutorial/AnaTutorial/macro`.
Copy the macro `Fun4Sim_ana_tutorial.C` to the default area of simulation macro i.e. `e1039-analysis/SimChainDev`

```
cp Fun4Sim_ana_tutorial.C path_to/e1039-analysis/SimChainDev/
cd e1039-analysis/SimChainDev
root -b -q Fun4Sim_ana_tutorial.C
```
