### Inspect trk_eval.root generated by `TrkEval`

open ntuple-like root file
```bash
root -l trk_eval.root
```

Draw DP hits as "*" using wire pos and truth_z for Entry 3
```C++
Truth->Draw("pos:truth_z","detectorID>54&&Entry$==3","*")
```

### Do same thing using "DST.root" generated by `Fun4AllDstOutputManager`


open DST file
```bash
root -l DST.root
```

load library and set alias to shorten cmd
```C++
gSystem->Load("libinterface_main.so");
gSystem->Load("libktracker.so");
T->SetAlias("sqevent","DST.SQEvent");
T->SetAlias("sqhit","DST.SQHitVector._vector");
```

Draw DP hits as "*" using wire pos and truth_z for Entry 3
 - for DP hits, wire pos `SQHit::get_pos()` is along Y direction
 - DP detectors have `detector_id` from 55 to 62
```C++
T->Draw("sqhit.get_pos():sqhit.get_truth_z()","sqhit.get_detector_id()>54&&Entry$==3","*");
```
