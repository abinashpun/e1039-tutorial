
#include <iostream>

using namespace std;

int Fun4Tutorial(
		const int nEvents = 1
		)
{
	gSystem->Load("libfun4all");
	gSystem->Load("libg4detectors");
	gSystem->Load("libg4testbench");
	gSystem->Load("libg4eval");

	///////////////////////////////////////////
	// Make the Server
	//////////////////////////////////////////
	Fun4AllServer *se = Fun4AllServer::instance();
	se->Verbosity(100);

	///////////////////////////////////////////
	// Setup generator
	//////////////////////////////////////////
	{
		gSystem->Load("libPHPythia8.so");

		PHPythia8 *pythia8 = new PHPythia8();
		pythia8->set_config_file("phpythia8_DY.cfg");
		pythia8->set_vertex_distribution_mean(0, 0, -300, 0);
		se->registerSubsystem(pythia8);

		pythia8->set_trigger_AND();

		PHPy8ParticleTrigger* trigger_mup = new PHPy8ParticleTrigger();
		trigger_mup->AddParticles("-13");
		trigger_mup->SetPzHighLow(120, 10);
		pythia8->register_trigger(trigger_mup);

		PHPy8ParticleTrigger* trigger_mum = new PHPy8ParticleTrigger();
		trigger_mum->AddParticles("13");
		trigger_mum->SetPzHighLow(120, 10);
		pythia8->register_trigger(trigger_mum);

		HepMCNodeReader *hr = new HepMCNodeReader();
		hr->set_particle_filter_on(true);
		hr->insert_particle_filter_pid(13);
		hr->insert_particle_filter_pid(-13);
		se->registerSubsystem(hr);
	}

	///////////////////////////////////////////
	// Fun4All G4 module
	//////////////////////////////////////////
	PHG4Reco *g4Reco = new PHG4Reco();

	// size of the world - every detector has to fit in here
	g4Reco->SetWorldSizeX(1000);
	g4Reco->SetWorldSizeY(1000);
	g4Reco->SetWorldSizeZ(5000);
	// shape of our world - it is a tube
	g4Reco->SetWorldShape("G4BOX");
	// this is what our world is filled with
	g4Reco->SetWorldMaterial("G4_Galactic");
	// Geant4 Physics list to use
	g4Reco->SetPhysicsList("FTFP_BERT");
	se->registerSubsystem(g4Reco);

	
	///////////////////////////////////////////
	// Setup field
	///////////////////////////////////////////
	g4Reco->set_field(2.);

	///////////////////////////////////////////
	// Setup geometries
	///////////////////////////////////////////

  // A big shielding block
	PHG4BlockSubsystem *box = new PHG4BlockSubsystem("Shield", 0);
	box->SuperDetector("Shield");
	box->set_double_param("size_x", 100);
	box->set_double_param("size_y", 100);
	box->set_double_param("size_z", 200);
	box->set_double_param("place_x", 0);
	box->set_double_param("place_y", 0);
	box->set_double_param("place_z", 0);
	box->set_string_param("material", "G4_Fe");// G4_Si, G4_AIR
	box->SetActive(0);
	g4Reco->registerSubsystem(box);

  // Sensitive detector - PHG4Hits would be saved
	box = new PHG4BlockSubsystem("Sensor", 0);
	box->SuperDetector("Sensor");
	box->set_double_param("size_x", 100);
	box->set_double_param("size_y", 100);
	box->set_double_param("size_z", 1);
	box->set_double_param("place_x", 0);
	box->set_double_param("place_y", 0);
	box->set_double_param("place_z", 300);
	box->set_string_param("material", "G4_Si");// G4_Si, G4_AIR
	box->SetActive(1);
	g4Reco->registerSubsystem(box);

  // Save truth information to Node Tree
	PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
	g4Reco->registerSubsystem(truth);

	///////////////////////////////////////////
  // Experiment 3
  // Simple analysis module
	///////////////////////////////////////////
  
	//gSystem->Load("libsimple_ana");
	//SimpleAna* ana = new SimpleAna();
	//ana->Verbosity(0);
	//se->registerSubsystem(ana);


	// input - we need a dummy to drive the event loop
	Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
	se->registerInputManager(in);

	///////////////////////////////////////////
  // Experiment 1
  // output - DST output
	///////////////////////////////////////////
  
	//Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "DST.root");
	//se->registerOutputManager(out);

	if (nEvents > 0)
	{
		se->run(nEvents);
	  ///////////////////////////////////////////
    // Experiment 2
    // export the geometry
	  ///////////////////////////////////////////
    
		//PHGeomUtility::ExportGeomtry(se->topNode(),"geom.root");

		// finish job - close and save output files
		se->End();
		se->PrintTimer();
		std::cout << "All done" << std::endl;

		// cleanup - delete the server and exit
		delete se;
		gSystem->Exit(0);
	}
	return;
}

PHG4ParticleGun *get_gun(const char *name = "PGUN")
{
	Fun4AllServer *se = Fun4AllServer::instance();
	PHG4ParticleGun *pgun = (PHG4ParticleGun *) se->getSubsysReco(name);
	return pgun;
}
