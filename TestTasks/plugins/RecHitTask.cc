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
	std::vector<int> vFEDs = utilities::getFEDList(_emap);
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

	//	push the rawIds of each fed into the vector
	for (std::vector<int>::const_iterator it=vFEDsVME.begin();
		it!=vFEDsVME.end(); ++it)
		_vhashFEDs.push_back(HcalElectronicsId(
			FIBERCH_MIN, FIBER_VME_MIN, SPIGOT_MIN, (*it)-FED_VME_MIN).rawId());
	for (std::vector<int>::const_iterator it=vFEDsuTCA.begin(); 
		it!=vFEDsuTCA.end(); ++it)
		_vhashFEDs.push_back(HcalElectronicsId(
			utilities::fed2crate(*it), SLOT_uTCA_MIN, FIBER_uTCA_MIN1,
			FIBERCH_MIN, false).rawId());

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
		new quantity::LumiSection(_numLSstart),
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
		new quantity::LumiSection(_numLSstart),
		new quantity::ValueQuantity(quantity::fN_to3000));

	std::vector<std::string> fnames;
	fnames.push_back("OcpUniSlot");
	fnames.push_back("TimeUniSlot");
	fnames.push_back("TCDS");
	fnames.push_back("LowOcp");
	fnames.push_back("Msn1LS");
	_cSummary.initialize(_name, "Summary", 
		new quantity::FEDQuantity(vFEDs),
		new quantity::FlagQuantity(fnames),
		new quantity::QualityQuantity());

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

	_cSummary.book(ib);

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

	double ehbm = 0; double ehbp = 0;
	double ehem = 0; double ehep = 0;
	int nChsHB = 0; int nChsHE = 0;
	int nChsHBCut = 0; int nChsHECut = 0;
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
		did.subdet()==HcalBarrel?did.ieta()>0?ehbp+=energy:ehbm+=energy:
			did.ieta()>0?ehep+=energy:ehem+=energy;
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
			did.subdet()==HcalBarrel?nChsHBCut++:nChsHECut++;
		}
		did.subdet()==HcalBarrel?nChsHB++:nChsHE++;
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), _currentLS, 
		nChsHB);
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), _currentLS,
		nChsHE);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), _currentLS,
		nChsHBCut);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), _currentLS, 
		nChsHECut);
	_cEnergyTotalPM_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), ehbp, ehbm);
	_cEnergyTotalPM_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), ehep, ehem);
	_cEnergyTotal_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), ehbp+ehbm);
	_cEnergyTotal_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), ehep+ehem);

	int nChsHO = 0; int nChsHOCut = 0;
	double ehop = 0; double ehom = 0;
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
		did.ieta()>0?ehop+=energy:ehom+=energy;
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
			nChsHOCut++;
		}
		nChsHO++;
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalOuter, 1, 1, 4), _currentLS,
		nChsHO);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalOuter, 1, 1, 4), _currentLS,
		nChsHOCut);
	_cEnergyTotalPM_Subdet.fill(HcalDetId(HcalOuter, 1, 1,4), ehop, ehom);
	_cEnergyTotal_Subdet.fill(HcalDetId(HcalOuter, 1, 1, 4), ehop+ehom);

	int nChsHF = 0; int nChsHFCut = 0;
	double ehfp = 0; double ehfm = 0;
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
		did.ieta()>0?ehfp+=energy:ehfm+=energy;
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
			nChsHFCut++;
		}
		nChsHF++;
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalForward, 1, 1, 1), _currentLS,
		nChsHF);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalForward, 1, 1, 1), _currentLS,
		nChsHFCut);
	_cEnergyTotalPM_Subdet.fill(HcalDetId(HcalForward, 1, 1,1), ehfp, ehfm);
	_cEnergyTotal_Subdet.fill(HcalDetId(HcalForward, 1, 1, 1), ehfp+ehfm);
}

/* virtual */ void RecHitTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		HcalElectronicsId eid = HcalElectronicsId(*it);
		for (int flag=fLowOcp; flag<nRecoFlag; flag++)
			_cSummary.setBinContent(eid, flag, fNA);

		int nmissing = 0;
		int ocpUniSlot = false;
		if (eid.isVMEid())
		{
			for (int is=SPIGOT_MIN; is<=SPIGOT_MAX; is++)
			{
				eid = HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN, 
					is, eid.dccid());
				HcalElectronicsId ejd = HcalElectronicsId(
					FIBERCH_MIN, FIBER_VME_MIN, 
					is==SPIGOT_MAX?SPIGOT_MIN:is+1, eid.dccid());
				int niscut = _cOccupancyCut_ElectronicsVME.getBinContent(eid);
				int njscut = _cOccupancyCut_ElectronicsVME.getBinContent(ejd);
				for (int ifib=FIBER_VME_MIN; ifib<=FIBER_VME_MAX; ifib++)
					for (int ifc=FIBERCH_MIN; ifc<=FIBERCH_MAX; ifc++)
					{
						eid = HcalElectronicsId(ifc, ifib, is, eid.dccid());
						if (_cOccupancy_FEDVME.getBinContent(eid)<1)
						{
							_cMissing1LS_FEDVME.fill(eid);
							nmissing++;
						}
					}
				double ratio = std::min(niscut, njscut)/std::max(niscut, 
					njscut);
				if (ratio<0.8)
					ocpUniSlot = true;
			}
		}
		else
		{
			//	uTCA
			for (int is=SLOT_uTCA_MIN; is<=SLOT_uTCA_MAX; is++)
			{
				eid = HcalElectronicsId(eid.crateId(), is, 
					FIBER_uTCA_MIN1, FIBERCH_MIN, false);
				HcalElectronicsId ejd = HcalElectronicsId(eid.crateId(),
					is==SLOT_uTCA_MAX?SLOT_uTCA_MIN:is+1, FIBER_uTCA_MIN1,
					FIBERCH_MIN, false);
				int niscut = _cOccupancyCut_ElectronicsuTCA.getBinContent(eid);
				int njscut = _cOccupancyCut_ElectronicsuTCA.getBinContent(ejd);
				for (int ifib=FIBER_uTCA_MIN1; ifib<=FIBER_uTCA_MAX2; ifib++)
				{
					if (ifib>FIBER_uTCA_MAX1 && ifib<FIBER_uTCA_MIN2)
						continue;
					for (int ifc=FIBERCH_MIN; ifc<=FIBERCH_MAX; ifc++)
					{
						eid=HcalElectronicsId(eid.crateId(), is, ifib, ifc, 
							false);
						if (_cOccupancy_FEDuTCA.getBinContent(eid)<1)
						{
							_cMissing1LS_FEDuTCA.fill(eid);
							nmissing++;
						}
					}
				}
				double ratio = std::min(niscut, njscut)/std::max(niscut, 
					njscut);
				if (ratio<0.8)
					ocpUniSlot = true;
			}
		}

		if (nmissing>0)
			_cSummary.setBinContent(eid, fMsn1LS, fLow);
		else
			_cSummary.setBinContent(eid, fMsn1LS, fGood);
		if (ocpUniSlot)
			_cSummary.setBinContent(eid, fOcpUniSlot, fLow);
		else
			_cSummary.setBinContent(eid, fOcpUniSlot, fGood);
	}
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RecHitTask);

