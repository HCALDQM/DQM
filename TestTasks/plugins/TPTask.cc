#include "DQM/TestTasks/interface/TPTask.h"

TPTask::TPTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagData = ps.getUntrackedParameter<edm::InputTag>("tagData",
		edm::InputTag("hcalDigis"));
	_tagEmul = ps.getUntrackedParameter<edm::InputTag>("tagEmul",
		edm::InputTag("emulDigis"));

	_tokData = consumes<HcalTrigPrimDigiCollection>(_tagData);
	_tokEmul = consumes<HcalTrigPrimDigiCollection>(_tagEmul);

	_skip1x1 = ps.getUntrackedParameter<bool>("skip1x1", true);
}

/* virtual */ void TPTask::bookHistograms(DQMStore::IBooker& ib,
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
	std::vector<uint32_t> depth0;
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);
	depth0.push_back(HcalTrigTowerDetId(1, 1, 0).rawId());
	_filter_depth0.initialize(filter::fPreserver, hashfunctions::fTTdepth,
		depth0);

	//	push the rawIds of each fed into the vector
	for (std::vector<int>::const_iterator it=vFEDsVME.begin();
		it!=vFEDsVME.end(); ++it)
		_vhashFEDs.push_back(HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN,
			SPIGOT_MIN, (*it)-FED_VME_MIN).rawId());
	for (std::vector<int>::const_iterator it=vFEDsuTCA.begin();
		it!=vFEDsuTCA.end(); ++it)
		_vhashFEDs.push_back(HcalElectronicsId(utilities::fed2crate(*it), 
			SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());

	//	INITIALIZE FIRST
	//	Et/FG
	_cEtData_TTSubdet.initialize(_name, "EtData", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEtEmul_TTSubdet.initialize(_name, "EtEmul", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEtCorr_TTSubdet.initialize(_name, "EtCorr", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cFGCorr_TTSubdet.initialize(_name, "FGCorr", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fFG),
		new quantity::ValueQuantity(quantity::fFG),
		new quantity::ValueQuantity(quantity::fN, true));

	//	Shape
	_cShapeData_TTSubdet.initialize(_name, "ShapeData",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fEt_128));
	_cShapeEmul_TTSubdet.initialize(_name, "ShapeEmul",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fEt_128));

	//	Occupancies
	_cOccupancyData_ElectronicsVME.initialize(_name, "OccupancyData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyEmul_ElectronicsVME.initialize(_name, "OccupancyEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyData_ElectronicsuTCA.initialize(_name, "OccupancyData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyEmul_ElectronicsuTCA.initialize(_name, "OccupancyEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));

	//	Mismatches
	_cEtMsm_ElectronicsVME.initialize(_name, "EtMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cFGMsm_ElectronicsVME.initialize(_name, "FGMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cEtMsm_ElectronicsuTCA.initialize(_name, "EtMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cFGMsm_ElectronicsuTCA.initialize(_name, "FGMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));

	//	Missing Data w.r.t. Emulator
	_cMsnData_ElectronicsVME.initialize(_name, "MsnData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cMsnData_ElectronicsuTCA.initialize(_name, "MsnData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cMsnEmul_ElectronicsVME.initialize(_name, "MsnEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cMsnEmul_ElectronicsuTCA.initialize(_name, "MsnEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));

	std::vector<std::string> fnames;
	fnames.push_back("OcpUniData");
	fnames.push_back("OcpUniEmul");
	fnames.push_back("EtMsmUni");
	fnames.push_back("FGMsmUni");
	_cSummary.initialize(_name, "Summary",
		new quantity::FEDQuantity(vFEDs),
		new quantity::FlagQuantity(fnames),
		new quantity::QualityQuantity());

	//	BOOK HISTOGRAMS
	_cEtData_TTSubdet.book(ib, _emap);
	_cEtEmul_TTSubdet.book(ib, _emap);
	_cEtCorr_TTSubdet.book(ib, _emap);
	_cFGCorr_TTSubdet.book(ib, _emap);
	_cShapeData_TTSubdet.book(ib, _emap);
	_cShapeEmul_TTSubdet.book(ib, _emap);
	_cOccupancyData_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyData_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOccupancyEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cEtMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cEtMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cFGMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cFGMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cMsnData_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cMsnData_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cMsnEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cMsnEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	
	//	initialize the hash map
	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fTHashMap);
}

/* virtual */ void TPTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void TPTask::_process(edm::Event const& e,
	edm::EventSetup const&)
{
	edm::Handle<HcalTrigPrimDigiCollection> cdata;
	edm::Handle<HcalTrigPrimDigiCollection> cemul;
	if (!e.getByToken(_tokData, cdata))
		_logger.dqmthrow("Collection HcalTrigPrimDigiCollection isn't available"
			+ _tagData.label() + " " + _tagData.instance());
	if (!e.getByToken(_tokEmul, cemul))
		_logger.dqmthrow("Collection HcalTrigPrimDigiCollection isn't available"
			+ _tagEmul.label() + " " + _tagEmul.instance());

	//	tmp
	bool useD1 = false;
	//	\tmp

	for (HcalTrigPrimDigiCollection::const_iterator it=cdata->begin();
		it!=cdata->end(); ++it)
	{
		//	tmp
		if (_skip1x1)
			if (it->id().depth()==10)	// 10 is the depth for 1x1
				continue;
		//	\tmp

		HcalTrigTowerDetId tid = it->id();
		HcalElectronicsId eid = _ehashmap.lookupTrigger(tid);
		int soiEt_d = it->SOI_compressedEt();
		int soiFG_d = it->SOI_fineGrain()?1:0;

		//	tmp
		if (tid.depth()==1)
			useD1 = true;
		//	\tmp
	
		_cEtData_TTSubdet.fill(tid, soiEt_d);
		if (eid.isVMEid())
			_cOccupancyData_ElectronicsVME.fill(eid);
		else
			_cOccupancyData_ElectronicsuTCA.fill(eid);

		//	get the emul
		HcalTrigPrimDigiCollection::const_iterator jt=cemul->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), 0));
		if (jt!=cemul->end())
		{
			//	if such digi is present
			int soiEt_e = jt->SOI_compressedEt();
			int soiFG_e = jt->SOI_fineGrain()?1:0;

			_cEtCorr_TTSubdet.fill(tid, soiEt_d, soiEt_e);
			_cFGCorr_TTSubdet.fill(tid, soiFG_d, soiFG_e);
			_cEtEmul_TTSubdet.fill(tid, soiEt_e);
			if (eid.isVMEid())
				_cOccupancyEmul_ElectronicsVME.fill(eid);
			else
				_cOccupancyEmul_ElectronicsuTCA.fill(eid);

			if (soiEt_d!=soiEt_e)
			{
				if (eid.isVMEid())
					_cEtMsm_ElectronicsVME.fill(eid);
				else
					_cEtMsm_ElectronicsuTCA.fill(eid);
			}
			if (soiFG_d!=soiFG_e)
			{
				if (eid.isVMEid())
					_cFGMsm_ElectronicsVME.fill(eid);
				else
					_cFGMsm_ElectronicsuTCA.fill(eid);
			}
		}
		else
		{
			//	if such a data digi isn't present in emulator
			if (eid.isVMEid())
				_cMsnEmul_ElectronicsVME.fill(eid);
			else
				_cMsnEmul_ElectronicsuTCA.fill(eid);
		}
	}
	for (HcalTrigPrimDigiCollection::const_iterator it=cemul->begin();
		it!=cemul->end(); ++it)
	{
		//	tmp
		if (_skip1x1)
			if (it->id().depth()==10)
				continue;
		//	\tmp

		HcalTrigTowerDetId tid = it->id();
		HcalTrigPrimDigiCollection::const_iterator jt=cdata->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), useD1?1:0));
		if (jt==cdata->end())
		{
			HcalElectronicsId eid = _ehashmap.lookupTrigger(
				HcalTrigTowerDetId(tid.ieta(), tid.iphi(), useD1?1:0));
			//	if such an emul digi is missing from data
			_cEtEmul_TTSubdet.fill(tid, it->SOI_compressedEt());
			if (eid.isVMEid())
			{
				_cMsnData_ElectronicsVME.fill(eid);
				_cOccupancyEmul_ElectronicsVME.fill(eid);
			}
			else
			{
				_cMsnData_ElectronicsuTCA.fill(eid);
				_cOccupancyEmul_ElectronicsuTCA.fill(eid);
			}
		}
	}
}

/* virtual */ void TPTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		HcalElectronicsId eid = HcalElectronicsId(*it);
		for (int flag=fOcpUniData; flag<nTPFlag; flag++)
			_cSummary.setBinContent(eid, flag, fNA);

		if (eid.isVMEid())
		{
			//	VME
			for (int is=SPIGOT_MIN; is<=SPIGOT_MAX; is++)
			{

			}
		}
		else 
		{	
			//	uTCA
			for (int is=SLOT_uTCA_MIN; is<=SLOT_uTCA_MAX; is++)
			{

			}
		}
	}
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(TPTask);

