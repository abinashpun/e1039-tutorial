/*============================================================================= 
  Author: Abinash Pun
  Jan 2, 2020
  ============================================================================*/
#include "ana_tutorial.h"

/// Fun4All includes
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4VtxPoint.h>
#include <E906LegacyGen/SQDimuonTruthInfoContainer.h>

/// Tracking includes
#include <ktracker/SRecEvent.h>

/// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

/// C++ includes
#include <cstring>
#include <cmath>
#include <cfloat>
#include <iostream>

#define LogError(exp)		std::cout<<"ERROR: "  <<__FILE__<<": "<<__LINE__<<": "<< exp << std::endl

using namespace std;

//
/// Constructor of the module
//
ana_tutorial::ana_tutorial(const std::string &name)
  : SubsysReco("ANA_TUTORIAL")
{
  outfilename = name;
  ResetVars();
  InitTree(); 
  //add other initializers here 
}


//
/// Distructor of the module
//
ana_tutorial::~ana_tutorial()
{
  if(truth_tree) delete truth_tree;
  if(track_tree) delete track_tree;
}


//
/// Initialize the process
//
int ana_tutorial::Init(PHCompositeNode *topNode)
{
  

  return Fun4AllReturnCodes::EVENT_OK;

}

//
/// Initialize the run
//
int ana_tutorial::InitRun(PHCompositeNode *topNode)
{
  file = new TFile(outfilename.c_str(), "RECREATE");

  int ret = GetNodes(topNode);
  if(ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  return Fun4AllReturnCodes::EVENT_OK;
}

//
///  Main function which is called in each event (main play ground to extract what you want and implement your cuts etc. here)
//
int ana_tutorial::process_event(PHCompositeNode *topNode)
{
 
  TruthEval(topNode); //get truth info
  TrkRecoEval(topNode); //get track info

  return Fun4AllReturnCodes::EVENT_OK;
}


//
/// called after the end of an event
//
int ana_tutorial::ResetEvent(PHCompositeNode *topNode)
{
return Fun4AllReturnCodes::EVENT_OK;
}


//
/// called after the end of Run
//
int ana_tutorial::EndRun(const int runnumber)
{
return Fun4AllReturnCodes::EVENT_OK;
}


//
/// End the module (write files here)
//
int ana_tutorial::End(PHCompositeNode *topNode)
{
  std::cout << " DONE PROCESSING " << endl;
  file->cd();
  // PHTFileServer::get().cd(_out_name.c_str());
  truth_tree->Write();
  track_tree->Write();

  file->Write();
  file->Close();
  return 0;
}


//
/// Function to get nodes required for analysis
//
 int ana_tutorial::GetNodes(PHCompositeNode* topNode)
{

  _truth = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  if (!_truth) {
    LogError("!_truth");
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  
  _recEvent = findNode::getClass<SRecEvent>(topNode, "SRecEvent");
  if (!_recEvent) {
    LogError("!_recEvent");
    //return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}


//
/// Function to collect the truth information from event generator
//

int ana_tutorial::TruthEval(PHCompositeNode* topNode)
{
 
  _dimuoninfo = findNode::getClass<SQDimuonTruthInfoContainer>(topNode, "DimuonInfo");

  if (!_dimuoninfo) {
    LogError("! _dimuon event weight not assigned, it will be set to default value of unity");
  }

  //check for nan 
  //if(_dimuoninfo->get_Dimuon_xs()<=0)  return Fun4AllReturnCodes::ABORTEVENT;
  dimuon_xs = _dimuoninfo->get_Dimuon_xs(); //Get event weight   
  if(std::isnan(dimuon_xs)) return Fun4AllReturnCodes::ABORTEVENT;
 

  if(_truth) { 
    /// Loop over the primary truth particles
    for(auto iter=_truth->GetPrimaryParticleRange().first;
    	iter!=_truth->GetPrimaryParticleRange().second;
    	++iter) {
     

      /// Get this truth particle
      PHG4Particle * par= iter->second;
      
      //get vtx, momentum etc. of the truth particle 
      int vtx_id =  par->get_vtx_id();
      PHG4VtxPoint* vtx = _truth->GetVtx(vtx_id);
  
      truth_pid = par->get_pid();
      truth_vtxx = vtx->get_x();
      truth_vtxy = vtx->get_y();
      truth_vtxz = vtx->get_z();
      truth_px = par->get_px();
      truth_py = par->get_py();
      truth_pz = par->get_pz();
      truth_e = par->get_e();
      cout<<"energy"<<truth_e<<endl;
      truth_tree->Fill(); // Fill the truth tree
    }
     
  }
    return 0;
}

//
/// Function to get the tracks reconstructed from tracker
//

int ana_tutorial::TrkRecoEval(PHCompositeNode *topNode)
{
  if(!_recEvent) {
    LogInfo("!_recEvent");
    return Fun4AllReturnCodes::ABORTRUN;
  }

   cout<<"Inside TrkRecoEval"<<endl;

  krecstat = _recEvent->getRecStatus(); //Reconstruction Status

  /// Loop over the tracks
  for(int itrack=0; itrack<_recEvent->getNTracks(); ++itrack){
    SRecTrack recTrack = _recEvent->getTrack(itrack);

    /// Get the reconstructed track info  
    
    TVector3 rec_vtx = recTrack.getTargetPos();
    rectrack_vx  = rec_vtx.X();
    rectrack_vy  = rec_vtx.Y();
    rectrack_vz  = rec_vtx.Z();

    TVector3 rec_mom = recTrack.getTargetMom();
    rectrack_px  = rec_mom.Px();
    rectrack_py  = rec_mom.Py();
    rectrack_pz  = rec_mom.Pz();
    
    track_tree->Fill();// Fill track Tree

  }
 
  return 0;
}
//
/// Function to reset or initialize the member variables in this class
//
void ana_tutorial::ResetVars()
{
  truth_vtxx = 0.0;
  truth_vtxy = 0.0;
  truth_vtxz = 0.0;
  truth_pid = 0;
  truth_px = 0.0;
  truth_py = 0.0;
  truth_pz = 0.0;
  truth_e = 0.0;
 
  dimuon_xs = 1.;

  rectrack_vx = 0.0;
  rectrack_vy = 0.0;
  rectrack_vz = 0.0;
  rectrack_px = 0.0;
  rectrack_py = 0.0;
  rectrack_pz = 0.0;

}


//
/// Function to initialize tree with respective branch assigment
//
void ana_tutorial::InitTree()
{
  truth_tree = new TTree("truthtree","a tree with all truth information from generator");
  truth_tree->Branch("truth_vtxx", &truth_vtxx, "truth_vtxx/F");
  truth_tree->Branch("truth_vtxy", &truth_vtxy, "truth_vtxy/F");
  truth_tree->Branch("truth_vtxz", &truth_vtxz, "truth_vtxz/F");
  truth_tree->Branch("truth_px", &truth_px, "truth_px/F");
  truth_tree->Branch("truth_py", &truth_py, "truth_py/F");
  truth_tree->Branch("truth_pz", &truth_pz, "truth_pz/F");
  truth_tree->Branch("truth_e", &truth_e, "truth_e/F");
  truth_tree->Branch("truth_pid", &truth_pid, "truth_pid/I");
  truth_tree->Branch("dimuon_xs", &dimuon_xs, "dimuon_xs/F");


  track_tree = new TTree("tracktree","a tree with all track information from generator");
  track_tree->Branch("krecstat", &krecstat, "krecstat/I");
  track_tree->Branch("rectrack_vx", &rectrack_vx, "rectrack_vx/F");
  track_tree->Branch("rectrack_vy", &rectrack_vy, "rectrack_vy/F");
  track_tree->Branch("rectrack_vz", &rectrack_vz, "rectrack_vz/F");
  track_tree->Branch("rectrack_px", &rectrack_px, "rectrack_px/F");
  track_tree->Branch("rectrack_py", &rectrack_py, "recthtrack_py/F");
  track_tree->Branch("rectrack_pz", &rectrack_pz, "rectrack_pz/F");

}

