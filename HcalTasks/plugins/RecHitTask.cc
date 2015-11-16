
#include "DQM/HcalTasks/interface/RecHitTask.h"

	using namespace hcaldqm;
	RecHitTask::RecHitTask(edm::ParameterSet const& ps) :
		DQTask(ps),

		//	Energy
		_cEnergy_SubDet(_name+"/Energy/SubDet", "Energy", mapper::fSubDet, 
			new axis::ValueAxis(axis::fXaxis, axis::fEnergy),
			new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
		_cEnergy_SubDet_ieta(_name+"/Energy/SubDet_ieta", "Energy",
			mapper::fSubDet_ieta, 
			new axis::ValueAxis(axis::fXaxis, axis::fEnergy),
			new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
		_cEnergy_SubDetPM_iphi(_name+"/Energy/SubDetPM_iphi", 
			"Energy", mapper::fSubDetPM_iphi,
			new axis::ValueAxis(axis::fXaxis, axis::fEnergy),
			new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
		_cEnergyvsieta_SubDet(_name+"/Energy/vsieta_SubDet", "Energyvsieta",
			mapper::fSubDet, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy)),
		_cEnergyvsiphi_SubDet(_name+"/Energy/vsiphi_SubDet", "Energyvsiphi",
			mapper::fSubDet, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fiphi), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy)),
		_cEnergy_depth(_name+"/Energy/depth", "Energy",
			mapper::fdepth, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
			new axis::ValueAxis(axis::fZaxis, axis::fEnergy)),
		_cEnergyvsietaCut_SubDet(_name+"/Energy/vsieta_SubDet", "Energyvsieta",
			mapper::fSubDet, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy)),
		_cEnergyvsiphiCut_SubDet(_name+"/Energy/vsiphi_SubDet", "Energyvsiphi",
			mapper::fSubDet, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fiphi), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy)),
		_cEnergyCut_depth(_name+"/Energy/depth", "Energy",
			mapper::fdepth, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
			new axis::ValueAxis(axis::fZaxis, axis::fEnergy)),

		//	Timing
		_cTimingCut_SubDet(_name+"/Timing/SubDet", "Timing", mapper::fSubDet,
			new axis::ValueAxis(axis::fXaxis, axis::fTime)),
		_cTimingCut_SubDetPM_iphi(_name+"/Timing/SubDetPM_iphi", "Timing", 
			mapper::fSubDetPM_iphi, 
			new axis::ValueAxis(axis::fXaxis, axis::fTime)),
		_cTimingCutvsLS_SubDetPM_iphi(_name+"/Timing/vsLS_SubDetPM_iphi",
			"Timing", mapper::fSubDetPM_iphi,
			new axis::ValueAxis(axis::fXaxis, axis::fLS),
			new axis::ValueAxis(axis::fYaxis, axis::fTime)),
		_cTimingCut_SubDet_ieta(_name+"/Timing/SubDet_ieta", "Timing", 
			mapper::fSubDet_ieta, 
			new axis::ValueAxis(axis::fXaxis, axis::fTime)),
		_cTimingvsietaCut_SubDet_iphi(_name+"/Timing/vsieta_SubDet_iphi", 
			"Timing",
			mapper::fSubDet_iphi,
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
			new axis::ValueAxis(axis::fYaxis, axis::fTime)),
		_cTimingvsiphiCut_SubDet_ieta(_name+"/Timing/vsiphi_SubDet_ieta", 
			"Timing",
			mapper::fSubDet_ieta,
			new axis::CoordinateAxis(axis::fXaxis, axis::fiphi),
			new axis::ValueAxis(axis::fYaxis, axis::fTime)),
		_cTimingCut_depth(_name+"/Timing/depth", "Timing",
			mapper::fdepth, 
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
			new axis::ValueAxis(axis::fZaxis, axis::fTime)),

		//	Occupancy
		_cOccupancy_depth(_name+"/Occupancy/depth", "Occupancy", mapper::fdepth,
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
		_cOccupancyCut_depth(_name+"/Occupancy/depth", "Occupancy", 
			mapper::fdepth,
			new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
			new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),

		//	Energy vs Timing
		_cEnergyvsTiming_SubDet(_name+"/EnergyvsTiming/SubDet", "EnergyvsTime", 
			mapper::fSubDet, 
			new axis::ValueAxis(axis::fXaxis, axis::fTime), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy)),
		_cEnergyvsTimingCut_SubDet(_name+"/EnergyvsTiming/SubDet", 
			"EnergyvsTime", 
			mapper::fSubDet, 
			new axis::ValueAxis(axis::fXaxis, axis::fTime), 
			new axis::ValueAxis(axis::fYaxis, axis::fEnergy))
	{
		//	tags
		_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
			edm::InputTag("hbhereco"));
		_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
			edm::InputTag("horeco"));
		_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
			edm::InputTag("hfreco"));

		//	cuts
		_cutE_HBHE = ps.getUntrackedParameter<double>("cutE_HBHE", 5);
		_cutE_HO = ps.getUntrackedParameter<double>("cutE_HO", 5);
		_cutE_HF = ps.getUntrackedParameter<double>("cutE_HF", 5);
	}

	/* virtual */ void RecHitTask::bookHistograms(DQMStore::IBooker & ib,
		edm::Run const& r, edm::EventSetup const& es)
	{
		DQTask::bookHistograms(ib, r, es);
		char cutstr[200];
		sprintf(cutstr, "_EHBHE%dHO%dHF%d", int(_cutE_HBHE),
			int(_cutE_HO), int(_cutE_HF));

		_cEnergy_SubDet.book(ib);
		_cEnergy_SubDet_ieta.book(ib);
		_cEnergy_SubDetPM_iphi.book(ib);
		_cEnergyvsieta_SubDet.book(ib);
		_cEnergyvsiphi_SubDet.book(ib);
		_cEnergy_depth.book(ib);

		_cEnergyvsietaCut_SubDet.book(ib, _subsystem, std::string(cutstr));
		_cEnergyvsiphiCut_SubDet.book(ib, _subsystem, std::string(cutstr));
		_cEnergyCut_depth.book(ib, _subsystem, std::string(cutstr));

		_cTimingCut_SubDet.book(ib, _subsystem, std::string(cutstr));
		_cTimingCut_SubDetPM_iphi.book(ib, _subsystem, std::string(cutstr));
		_cTimingCut_SubDet_ieta.book(ib, _subsystem, std::string(cutstr));
		_cTimingvsietaCut_SubDet_iphi.book(ib, _subsystem, std::string(cutstr));
		_cTimingvsiphiCut_SubDet_ieta.book(ib, _subsystem, std::string(cutstr));
		_cTimingCut_depth.book(ib, _subsystem, std::string(cutstr));
		_cTimingCutvsLS_SubDetPM_iphi.book(ib, _subsystem, std::string(cutstr));

		_cOccupancyCut_depth.book(ib, _subsystem, std::string(cutstr));
		_cOccupancy_depth.book(ib);

		_cEnergyvsTiming_SubDet.book(ib);
		_cEnergyvsTimingCut_SubDet.book(ib, _subsystem, std::string(cutstr));
	}

	/* virtual */ void RecHitTask::_process(edm::Event const& e,
		edm::EventSetup const& es)
	{
		edm::Handle<HBHERecHitCollection>	chbhe;
		edm::Handle<HORecHitCollection>		cho;
		edm::Handle<HFRecHitCollection>		chf;

		if (!(e.getByLabel(_tagHBHE, chbhe)))
			this->_throw("Collection HBHERecHitCollection not available",
				" " + _tagHBHE.label() + " " + _tagHBHE.instance());
		if (!(e.getByLabel(_tagHO, cho)))
			this->_throw("Collection HORecHitCollection not available",
				" " + _tagHO.label() + " " + _tagHO.instance());
		if (!(e.getByLabel(_tagHF, chf)))
			this->_throw("Collection HFRecHitCollection not available",
				" " + _tagHF.label() + " " + _tagHF.instance());

		//	Processing
		for (HBHERecHitCollection::const_iterator it=chbhe->begin();
			it!=chbhe->end(); ++it)
		{
			const HBHERecHit rh = (const HBHERecHit)(*it);
			double energy = rh.energy();
			double time = rh.time();
			const HcalDetId did = rh.id();

			_cEnergy_SubDet.fill(did, energy);
			_cEnergy_SubDet_ieta.fill(did, energy);
			_cEnergy_SubDetPM_iphi.fill(did, energy);
			_cEnergyvsieta_SubDet.fill(did, energy);
			_cEnergyvsiphi_SubDet.fill(did, energy);
			_cEnergy_depth.fill(did, energy);
			
			_cOccupancy_depth.fill(did);
			_cEnergyvsTiming_SubDet.fill(did, time, energy);

			if (energy>_cutE_HBHE)
			{
				_cEnergyCut_depth.fill(did, energy);
				_cEnergyvsietaCut_SubDet.fill(did, energy);
				_cEnergyvsiphiCut_SubDet.fill(did, energy);
				_cEnergyCut_depth.fill(did, energy);
				_cTimingCut_SubDet.fill(did, time);
				_cTimingCut_SubDetPM_iphi.fill(did, time);
				_cTimingCut_SubDet_ieta.fill(did, time);
				_cTimingvsietaCut_SubDet_iphi.fill(did, time);
				_cTimingvsiphiCut_SubDet_ieta.fill(did, time);
				_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, time);
				_cTimingCut_depth.fill(did, time);
				_cOccupancyCut_depth.fill(did);
				_cEnergyvsTimingCut_SubDet.fill(did, time, energy);
			}
		}
		for (HORecHitCollection::const_iterator it=cho->begin();
			it!=cho->end(); ++it)
		{
			const HORecHit rh = (const HORecHit)(*it);
			double energy = rh.energy();
			double time = rh.time();
			const HcalDetId did = rh.id();

			_cEnergy_SubDet.fill(did, energy);
			_cEnergy_SubDet_ieta.fill(did, energy);
			_cEnergy_SubDetPM_iphi.fill(did, energy);
			_cEnergyvsieta_SubDet.fill(did, energy);
			_cEnergyvsiphi_SubDet.fill(did, energy);
			_cEnergy_depth.fill(did, energy);
			
			_cOccupancy_depth.fill(did);
			_cEnergyvsTiming_SubDet.fill(did, time, energy);

			if (energy>_cutE_HO)
			{
				_cEnergyCut_depth.fill(did, energy);
				_cEnergyvsietaCut_SubDet.fill(did, energy);
				_cEnergyvsiphiCut_SubDet.fill(did, energy);
				_cEnergyCut_depth.fill(did, energy);
				_cTimingCut_SubDet.fill(did, time);
				_cTimingCut_SubDetPM_iphi.fill(did, time);
				_cTimingCut_SubDet_ieta.fill(did, time);
				_cTimingvsietaCut_SubDet_iphi.fill(did, time);
				_cTimingvsiphiCut_SubDet_ieta.fill(did, time);
				_cTimingCut_depth.fill(did, time);
				_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, time);
				_cOccupancyCut_depth.fill(did);
				_cEnergyvsTimingCut_SubDet.fill(did, time, energy);
			}
		}
		for (HFRecHitCollection::const_iterator it=chf->begin();
			it!=chf->end(); ++it)
		{
			const HFRecHit rh = (const HFRecHit)(*it);
			double energy = rh.energy();
			double time = rh.time();
			const HcalDetId did = rh.id();

			_cEnergy_SubDet.fill(did, energy);
			_cEnergy_SubDet_ieta.fill(did, energy);
			_cEnergy_SubDetPM_iphi.fill(did, energy);
			_cEnergyvsieta_SubDet.fill(did, energy);
			_cEnergyvsiphi_SubDet.fill(did, energy);
			_cEnergy_depth.fill(did, energy);
			
			_cOccupancy_depth.fill(did);
			_cEnergyvsTiming_SubDet.fill(did, time, energy);

			if (energy>_cutE_HF)
			{
				_cEnergyCut_depth.fill(did, energy);
				_cEnergyvsietaCut_SubDet.fill(did, energy);
				_cEnergyvsiphiCut_SubDet.fill(did, energy);
				_cEnergyCut_depth.fill(did, energy);
				_cTimingCut_SubDet.fill(did, time);
				_cTimingCut_SubDetPM_iphi.fill(did, time);
				_cTimingCut_SubDet_ieta.fill(did, time);
				_cTimingvsietaCut_SubDet_iphi.fill(did, time);
				_cTimingvsiphiCut_SubDet_ieta.fill(did, time);
				_cTimingCut_depth.fill(did, time);
				_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, time);
				_cOccupancyCut_depth.fill(did);
				_cEnergyvsTimingCut_SubDet.fill(did, time, energy);
			}
		}
	}

	/* virtual */ void RecHitTask::_resetMonitors(UpdateFreq uf)
	{
		switch(uf)
		{
			case fEvent:
				for (unsigned int i=0; i<constants::SUBDET_NUM; i++)
					_numRecHits[i] = 0;
				break;
			default:
				break;
		}
		DQTask::_resetMonitors(uf);
	}

DEFINE_FWK_MODULE(RecHitTask);




