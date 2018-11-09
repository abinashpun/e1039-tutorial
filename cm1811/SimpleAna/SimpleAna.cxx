/*
 * SimpleAna.C
 *
 *  Created on: Oct 29, 2017
 *      Author: yuhw@nmsu.edu
 */


#include "SimpleAna.h"

#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Hit.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>


#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#include <cstring>
#include <cmath>
#include <cfloat>
#include <stdexcept>
#include <limits>
#include <boost/lexical_cast.hpp>
#include <map>

using namespace std;

SimpleAna::SimpleAna(const std::string& name, const std::string &out) :
SubsysReco(name),
_out_name(out),
_event(0),
_g4truth_container(nullptr),
_g4hit_sensor(nullptr)
{
	InitEvalTree();
	ResetEvalVars();
}

int SimpleAna::Init(PHCompositeNode* topNode) {
	return Fun4AllReturnCodes::EVENT_OK;
}

int SimpleAna::InitRun(PHCompositeNode* topNode) {

	int ret = GetNodes(topNode);
	if(ret != Fun4AllReturnCodes::EVENT_OK) return ret;

	return Fun4AllReturnCodes::EVENT_OK;
}

int SimpleAna::process_event(PHCompositeNode* topNode) {

	if(Verbosity() >= Fun4AllBase::VERBOSITY_SOME)
		std::cout << "Entering SimpleAna::process_event: " << _event << std::endl;

	ResetEvalVars();

	map<int, int> m_trackID_nHit;

	for(auto iter=_g4hit_sensor->getHits().first; iter!=_g4hit_sensor->getHits().second; ++iter) {
		PHG4Hit* hit = iter->second;
		int track_id = hit->get_trkid();
		m_trackID_nHit[track_id] = 1;
	}

	vector<PHG4TruthInfoContainer::ConstIterator> v_mup;
	vector<PHG4TruthInfoContainer::ConstIterator> v_mum;

	auto range = _g4truth_container->GetParticleRange();
	for(auto iter = range.first; iter!= range.second; ++iter) {
		PHG4Particle *particle =  iter->second;

		if(particle->get_pid() == 13) v_mum.push_back(iter);
		if(particle->get_pid() ==-13) v_mup.push_back(iter);
	}

	for(auto i_mup : v_mup)
		for(auto i_mum : v_mum)
		{
			PHG4Particle *p_mup = i_mup->second;
			PHG4Particle *p_mum = i_mum->second;

			auto vtx_id1 = p_mup->get_vtx_id();
			auto vtx_id2 = p_mum->get_vtx_id();

			if(vtx_id1 != vtx_id2) {
				if(verbosity >= Fun4AllBase::VERBOSITY_A_LOT) {
					cout <<"event: "<< _event << " vtx1: " << vtx_id1 << " vtx2: " << vtx_id2 << endl;
				}
				continue;
			}

			TLorentzVector tlv_mup(
					p_mup->get_px(),
					p_mup->get_py(),
					p_mup->get_pz(),
					p_mup->get_e()
			);

			TLorentzVector tlv_mum(
					p_mum->get_px(),
					p_mum->get_py(),
					p_mum->get_pz(),
					p_mum->get_e()
			);

			auto vphoton = tlv_mup + tlv_mum;

			_gmass = vphoton.M();

			if(_gmass < 4.) {
				if(verbosity >= Fun4AllBase::VERBOSITY_A_LOT) {
					cout <<"event: "<< _event << " mass: " << _gmass << " " << v_mum.size() << " " << v_mup.size() << endl;
					p_mup->identify();
					p_mum->identify();
				}
			}

			auto id1 = p_mup->get_track_id();
			auto id2 = p_mum->get_track_id();

			if(m_trackID_nHit[id1]>=1 and m_trackID_nHit[id2]>=1)
				_acc = 1;

	}

	_tout->Fill();

	if(Verbosity() >= Fun4AllBase::VERBOSITY_SOME)
		std::cout << "Leaving SimpleAna::process_event: " << _event << std::endl;
	++_event;

	return Fun4AllReturnCodes::EVENT_OK;
}

int SimpleAna::End(PHCompositeNode* topNode) {
	if(Verbosity() >= Fun4AllBase::VERBOSITY_SOME)
		std::cout << "SimpleAna::End" << std::endl;

	PHTFileServer::get().cd(_out_name.c_str());
	_tout->Write();

	return Fun4AllReturnCodes::EVENT_OK;
}

int SimpleAna::InitEvalTree() {
	PHTFileServer::get().open(_out_name.c_str(), "RECREATE");

	_tout = new TTree("T", "SimpleAna");
	_tout->Branch("gmass", &_gmass, "gmass/D");
	_tout->Branch("acc",   &_acc,   "acc/I");

	return 0;
}

int SimpleAna::ResetEvalVars() {
	_gmass = std::numeric_limits<double>::max();
	_acc   = std::numeric_limits<int>::max();
	return 0;
}

int SimpleAna::GetNodes(PHCompositeNode* topNode) {

	_g4truth_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
	if (!_g4truth_container) {
		cerr << PHWHERE << " ERROR: Can't find node G4TruthInfo" << endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}

	_g4hit_sensor = findNode::getClass<PHG4HitContainer>(topNode,"G4HIT_Sensor");
	if (!_g4hit_sensor) {
		cerr << PHWHERE << " ERROR: Can't find node G4HIT_Sensor" << endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}


	return Fun4AllReturnCodes::EVENT_OK;
}







