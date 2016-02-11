#include "DQM/TestTasks/interface/RecHitTask.h"

RecHitTask::RecHitTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hbhereco"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("horeco"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hfreco"));

	_tokHBHE = consumes<HBHERecHitCollection>(_tagHBHE);
	_tokHO = consumes<HORecHitCollection>(_tagHO);
	_tokHF = consumes<HFRecHitCollection>(_tagHF);

	_cutE_HBHE = ps.getUntrackedParameter<double>("cutE_HBHE", 20);
	_cutE_HO = ps.getUntrackedParameter<double>("cutE_HO", 20);
	_cutE_HF = ps.getUntrackedParameter<double>("cutE_HF", 20);
}

/* virtual */ void RecHitTask::bookHistograms(DQMStore::IBooker& ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib,r,es);

	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	std::vector<int> vFEDsVME = utilities::getFEDVMEList(_emap);
	std::vector<int> vFEDsuTCA = utilities::getFEDuTCAList(_emap);
	std::vector<uint32_t> vVME;
	std::vector<uint32_t> vuTCA;
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);

	//	INITIALIZE FIRST
	//	Energy
	_cEnergy_Subdet.initialize(_name, "Energy", hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fEnergy),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEnergyTotal_Subdet.initialize(_name, "EnergyTotal", 
		hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fEnergyTotal, true),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEnergyTotalPM_Subdet.initialize(_name, "EnergyTotalPM",
		hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fEnergyTotal, true),
		new quantity::ValueQuantity(quantity::fEnergyTotal, true),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEnergy_depth.initialize(_name, "Energy", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fEnergy, true));
	_cEnergy_FEDVME.initialize(_name, "Energy", hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fEnergy));
	_cEnergy_FEDuTCA.initialize(_name, "Energy", hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fEnergy));
	_cEnergy_ElectronicsVME.initialize(_name, "Energy", 
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fEnergy));
	_cEnergy_ElectronicsuTCA.initialize(_name, "Energy", 
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fEnergy));

	//	Timing
	_cTimingCut_Subdet.initialize(_name, "TimingCut", hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTimingvsEnergy_FEDSlot.initialize(_name, "TimingvsEnergy",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fEnergy, true),
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTimingCut_FEDSlot.initialize(_name, "TimingCut",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_FEDVME.initialize(_name, "TimingCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_FEDuTCA.initialize(_name, "TimingCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_ElectronicsVME.initialize(_name, "TimingCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_ElectronicsuTCA.initialize(_name, "TimingCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_HBHEPartition.initialize(_name, "TimingCut",
		hashfunctions::fHBHEPartition,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN));

	//	Occupancy
	_cOccupancy_depth.initialize(_name, "Occupancy",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancy_FEDVME.initialize(_name, "Occupancy",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancy_FEDuTCA.initialize(_name, "Occupancy",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancy_ElectronicsVME.initialize(_name, "Occupancy",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancy_ElectronicsuTCA.initialize(_name, "Occupancy",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyvsLS_Subdet.initialize(_name, "OccupancyvsLS",
		hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fLS),
		new quantity::ValueQuantity(quantity::fN_to3000));
	_cOccupancyCut_FEDVME.initialize(_name, "OccupancyCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCut_FEDuTCA.initialize(_name, "OccupancyCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCut_ElectronicsVME.initialize(_name, "OccupancyCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCut_ElectronicsuTCA.initialize(_name, "OccupancyCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCutvsLS_Subdet.initialize(_name, "OccupancyCutvsLS",
		hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fLS),
		new quantity::ValueQuantity(quantity::fN_to3000));


	//	BOOK HISTOGRAMS
	char cutstr[200];
	sprintf(cutstr, "_EHBHE%dHO%dHF%d", int(_cutE_HBHE),
		int(_cutE_HO), int(_cutE_HF));
	char cutstr2[200];
	sprintf(cutstr2, "_EHF%d", int(_cutE_HF));

	//	Energy
	_cEnergy_Subdet.book(ib, _emap);
	_cEnergyTotal_Subdet.book(ib, _emap);
	_cEnergyTotalPM_Subdet.book(ib, _emap);
	_cEnergy_depth.book(ib, _emap);
	_cEnergy_FEDVME.book(ib, _emap, _filter_uTCA);
	_cEnergy_FEDuTCA.book(ib, _emap, _filter_VME);
	_cEnergy_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cEnergy_ElectronicsuTCA.book(ib, _emap, _filter_VME);

	//	Timing
	_cTimingCut_Subdet.book(ib, _emap);
	_cTimingvsEnergy_FEDSlot.book(ib, _emap);
	_cTimingCut_FEDSlot.book(ib, _emap);
	_cTimingCut_FEDVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_FEDuTCA.book(ib, _emap, _filter_VME);
	_cTimingCut_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cTimingCut_HBHEPartition.book(ib, _emap);

	//	Occupancy
	_cOccupancy_depth.book(ib, _emap);
	_cOccupancy_FEDVME.book(ib, _emap, _filter_uTCA);
	_cOccupancy_FEDuTCA.book(ib, _emap, _filter_VME);
	_cOccupancy_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancy_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOccupancyvsLS_Subdet.book(ib, _emap);
	_cOccupancyCut_FEDVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyCut_FEDuTCA.book(ib, _emap, _filter_VME);
	_cOccupancyCut_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyCut_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOccupancyCutvsLS_Subdet.book(ib, _emap);

	//	initialize hash map
	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fDHashMap);
}

/* virtual */ void RecHitTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void RecHitTask::_process(edm::Event const& e,
	edm::EventSetup const&)
{
	edm::Handle<HBHERecHitCollection> chbhe;
	edm::Handle<HORecHitCollection> cho;
	edm::Handle<HFRecHitCollection> chf;

	if (!(e.getByToken(_tokHBHE, chbhe)))
		_logger.dqmthrow("Collection HBHERecHitCollection not available "
			+ _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!(e.getByToken(_tokHO, cho)))
		_logger.dqmthrow("Collection HORecHitCollection not available "
			+ _tagHO.label() + " " + _tagHO.instance());
	if (!(e.getByToken(_tokHF, chf)))
		_logger.dqmthrow("Collection HFRecHitCollection not available "
			+ _tagHF.label() + " " + _tagHF.instance());

	for (HBHERecHitCollection::const_iterator it=chbhe->begin();
		it!=chbhe->end(); ++it)
	{
		double energy = it->energy();
		double timing = it->time();
		HcalDetId did = it->id();
		HcalElectronicsId eid = _ehashmap.lookup(did);
		_cEnergy_Subdet.fill(did, energy);
		_cEnergy_depth.fill(did, energy);
		_cTimingvsEnergy_FEDSlot.fill(eid, energy, timing);
		_cOccupancy_depth.fill(did);
		if (eid.isVMEid())
		{
			_cEnergy_FEDVME.fill(eid, energy);
			_cEnergy_ElectronicsVME.fill(eid, energy);
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cEnergy_FEDuTCA.fill(eid, energy);
			_cEnergy_ElectronicsuTCA.fill(eid, energy);
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		if (energy>_cutE_HBHE)
		{
			_cTimingCut_Subdet.fill(did, timing);
			_cTimingCut_FEDSlot.fill(eid, timing);
			_cTimingCut_HBHEPartition.fill(did, timing);
			if (eid.isVMEid())
			{
				_cTimingCut_FEDVME.fill(eid, timing);
				_cTimingCut_ElectronicsVME.fill(eid, timing);
				_cOccupancyCut_FEDVME.fill(eid);
				_cOccupancyCut_ElectronicsVME.fill(eid);
			}
			else
			{
				_cTimingCut_FEDuTCA.fill(eid, timing);
				_cTimingCut_ElectronicsuTCA.fill(eid, timing);
				_cOccupancyCut_FEDuTCA.fill(eid);
				_cOccupancyCut_ElectronicsuTCA.fill(eid);
			}
		}
	}
	for (HORecHitCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		double energy = it->energy();
		double timing = it->time();
		HcalDetId did = it->id();
		HcalElectronicsId eid = _ehashmap.lookup(did);
		_cEnergy_Subdet.fill(did, energy);
		_cEnergy_depth.fill(did, energy);
		_cTimingvsEnergy_FEDSlot.fill(eid, energy, timing);
		_cOccupancy_depth.fill(did);
		if (eid.isVMEid())
		{
			_cEnergy_FEDVME.fill(eid, energy);
			_cEnergy_ElectronicsVME.fill(eid, energy);
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cEnergy_FEDuTCA.fill(eid, energy);
			_cEnergy_ElectronicsuTCA.fill(eid, energy);
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		if (energy>_cutE_HO)
		{
			_cTimingCut_Subdet.fill(did, timing);
			_cTimingCut_FEDSlot.fill(eid, timing);
			if (eid.isVMEid())
			{
				_cTimingCut_FEDVME.fill(eid, timing);
				_cTimingCut_ElectronicsVME.fill(eid, timing);
				_cOccupancyCut_FEDVME.fill(eid);
				_cOccupancyCut_ElectronicsVME.fill(eid);
			}
			else
			{
				_cTimingCut_FEDuTCA.fill(eid, timing);
				_cTimingCut_ElectronicsuTCA.fill(eid, timing);
				_cOccupancyCut_FEDuTCA.fill(eid);
				_cOccupancyCut_ElectronicsuTCA.fill(eid);
			}
		}
	}

	for (HFRecHitCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		double energy = it->energy();
		double timing = it->time();
		HcalDetId did = it->id();
		HcalElectronicsId eid = _ehashmap.lookup(did);
		_cEnergy_Subdet.fill(did, energy);
		_cEnergy_depth.fill(did, energy);
		_cTimingvsEnergy_FEDSlot.fill(eid, energy, timing);
		_cOccupancy_depth.fill(did);
		if (eid.isVMEid())
		{
			_cEnergy_FEDVME.fill(eid, energy);
			_cEnergy_ElectronicsVME.fill(eid, energy);
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cEnergy_FEDuTCA.fill(eid, energy);
			_cEnergy_ElectronicsuTCA.fill(eid, energy);
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		if (energy>_cutE_HF)
		{
			_cTimingCut_Subdet.fill(did, timing);
			_cTimingCut_FEDSlot.fill(eid, timing);
			if (eid.isVMEid())
			{
				_cTimingCut_FEDVME.fill(eid, timing);
				_cTimingCut_ElectronicsVME.fill(eid, timing);
				_cOccupancyCut_FEDVME.fill(eid);
				_cOccupancyCut_ElectronicsVME.fill(eid);
			}
			else
			{
				_cTimingCut_FEDuTCA.fill(eid, timing);
				_cTimingCut_ElectronicsuTCA.fill(eid, timing);
				_cOccupancyCut_FEDuTCA.fill(eid);
				_cOccupancyCut_ElectronicsuTCA.fill(eid);
			}
		}
	}
}

/* virtual */ void RecHitTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RecHitTask);

