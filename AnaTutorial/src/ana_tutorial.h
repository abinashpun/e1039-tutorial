#ifndef __ANA_TUTORIAL_H__
#define __ANA_TUTORIAL_H__

#include <fun4all/SubsysReco.h>

//Declaration the class to use in analysis module
class TFile;
class TTree;
class TH1;
class TH2;
class PHCompositeNode;
class PHG4TruthInfoContainer;
class SQDimuonTruthInfoContainer;
class SRecEvent;

//Defination of this analysis module class
class ana_tutorial : public SubsysReco
{
 public:
  //Constructor
  ana_tutorial(const std::string &name = "ana_tutorial.root");

  //Destructor
  virtual ~ana_tutorial();

  //@
  //Basic Fun4All methods (SubsysReco functions) 

  int Init(PHCompositeNode *topNode); //called once you register the module with Fun4AllServer
  int InitRun(PHCompositeNode *topNode); //called before first event in new Run
  int process_event(PHCompositeNode *topNode); //called for every event (main function)
  int ResetEvent(PHCompositeNode *topNode); //clean up leftovers of the event
  int EndRun(const int runnumber); //called before InitRun
  int End(PHCompositeNode *topNode); //Last call
  //
  //@

  

 
 private:
  void ResetVars();
  void InitTree();

  ///get truth info
  int TruthEval(PHCompositeNode *topNode);
  ///get track info
  int TrkRecoEval(PHCompositeNode *topNode);
  ///get the required nodes
  int GetNodes(PHCompositeNode *topNode);

  PHG4TruthInfoContainer* _truth;
  SQDimuonTruthInfoContainer* _dimuoninfo;
  SRecEvent* _recEvent;
  
  std::string outfilename;

  //trees and files
  TFile *file;
  TTree *truth_tree;
  TTree *track_tree;

  //truth variables
  float truth_vtxx,truth_vtxy,truth_vtxz;
  float truth_px, truth_py, truth_pz, truth_e;
  float dimuon_xs;
  int truth_pid;

  //track variables
  int krecstat;
  float rectrack_px, rectrack_py, rectrack_pz;
  float rectrack_vx, rectrack_vy, rectrack_vz;

};

#endif  //
