#include "DQM/TestTasks/interface/RawTask.h"

RawTask::RawTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagFEDs = ps.getUntrackedParameter<edm::InputTag>("tagFEDs",
		edm::InputTag("rawDataCollector"));
	_tagReport = ps.getUntrackedParameter<edm::InputTag>("tagReport",
		edm::InputTag("hcalDigis"));

	_tokFEDs = consumes<FEDRawDataCollection>(_tagFEDs);
	_tokReport = consumes<HcalUnpackerReport>(_tagReport);
}

/* virtual */ void RawTask::bookHistograms(DQMStore::IBooker& ib,
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
	std::vector<uint32_t> vhashFEDsVME;
	std::vector<uint32_t> vhashFEDsuTCA;
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);

	for (std::vector<int>::const_iterator it=vFEDsVME.begin();
		it!=vFEDsVME.end(); ++it)
		vhashFEDsVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
			constants::FIBER_VME_MIN, SPIGOT_MIN,
			(*it)-constants::FED_VME_MIN).rawId());
	for (std::vector<int>::const_iterator it=vFEDsuTCA.begin();
		it!=vFEDsuTCA.end(); ++it)
		vhashFEDsuTCA.push_back(HcalElectronicsId(
		utilities::fed2crate(*it), SLOT_uTCA_MIN, FIBER_uTCA_MIN1,
		FIBERCH_MIN, false).rawId());
	_filter_FEDsVME.initialize(filter::fPreserver, 
		hashfunctions::fFED, vhashFEDsVME);
	_filter_FEDsuTCA.initialize(filter::fPreserver,
		hashfunctions::fFED, vhashFEDsuTCA);

	//	INITIALIZE FIRST
	_cEvnMsm_ElectronicsVME.initialize(_name, "EvnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cBcnMsm_ElectronicsVME.initialize(_name, "BcnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cOrnMsm_ElectronicsVME.initialize(_name, "OrnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN));
	_cEvnMsm_ElectronicsuTCA.initialize(_name, "EvnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cBcnMsm_ElectronicsuTCA.initialize(_name, "BcnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cOrnMsm_ElectronicsuTCA.initialize(_name, "OrnMsm",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));

	//	Occupancies
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

	//	Bad Quality
	_cBadQuality_FEDVME.initialize(_name, "BadQuality",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cBadQuality_FEDuTCA.initialize(_name, "BadQuality",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cBadQualityvsLS.initialize(_name, "BadQualityvsLS",
		new quantity::ValueQuantity(quantity::fLS),
		new quantity::ValueQuantity(quantity::fN_m0to10000));

	//	BOOK HISTOGRAMS
	_cEvnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cBcnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOrnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cEvnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cBcnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOrnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);

	_cOccupancy_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancy_ElectronicsuTCA.book(ib, _emap, _filter_VME);

	_cBadQuality_FEDVME.book(ib, _emap, _filter_uTCA);
	_cBadQuality_FEDuTCA.book(ib, _emap, _filter_VME);
	_cBadQualityvsLS.book(ib);

	//	initialize hash map
	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fDHashMap);
}

/* virtual */ void RawTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void RawTask::_process(edm::Event const& e,
	edm::EventSetup const&)
{
	edm::Handle<FEDRawDataCollection> craw;
	edm::Handle<HcalUnpackerReport> creport;
	if (!e.getByToken(_tokFEDs, craw))
		_logger.dqmthrow("Collection FEDRawDataCollection isn't available"+
			_tagFEDs.label()+" " +_tagFEDs.instance());
	if (!e.getByToken(_tokReport, creport))
		_logger.dqmthrow("Collection HcalUnpackerReport isn't available"+
			_tagReport.label()+" " +_tagReport.instance());

	_cBadQualityvsLS.fill(_currentLS,creport->badQualityDigis());
	for (std::vector<DetId>::const_iterator it=creport->bad_quality_begin();
		it!=creport->bad_quality_end(); ++it)
	{
		std::cout << HcalDetId(*it) << std::endl;
		HcalElectronicsId eid = _ehashmap.lookup(*it);
		if (eid.isVMEid())
			_cBadQuality_FEDVME.fill(eid);
		else
			_cBadQuality_FEDuTCA.fill(eid);
	}

	for (int fed=FEDNumbering::MINHCALFEDID; 
		fed<=FEDNumbering::MAXHCALuTCAFEDID; fed++)
	{
		//	skip nonHCAL FEDs
		if ((fed>FEDNumbering::MAXHCALFEDID &&
			fed<FEDNumbering::MINHCALuTCAFEDID) || 
			fed>FEDNumbering::MAXHCALuTCAFEDID)
			continue;
		FEDRawData const& raw = craw->FEDData(fed);
		if (raw.size()<constants::RAW_EMPTY)
			continue;

		if (fed<=FEDNumbering::MAXHCALFEDID)	// VME
		{
			HcalDCCHeader const* hdcc = (HcalDCCHeader const*)(raw.data());
			if (!hdcc)
				continue;

			uint32_t bcn = hdcc->getBunchId();
			uint32_t orn = hdcc->getOrbitNumber();
			uint32_t evn = hdcc->getDCCEventNumber();
			int dccId = hdcc->getSourceId()-constants::FED_VME_MIN;

			//	iterate over spigots
			HcalHTRData htr;
			for (int is=0; is<HcalDCCHeader::SPIGOT_COUNT; is++)
			{
				int r = hdcc->getSpigotData(is, htr, raw.size());
				if (r!=0)
					continue;
				HcalElectronicsId eid = HcalElectronicsId(
					constants::FIBERCH_MIN, constants::FIBER_VME_MIN,
					is, dccId);
				if (_filter_FEDsVME.filter(eid))
					continue;

				uint32_t htr_evn = htr.getL1ANumber();
				uint32_t htr_orn = htr.getOrbitNumber();
				uint32_t htr_bcn = htr.getBunchNumber();
				bool qevn = (htr_evn!=evn);
				bool qbcn = (htr_bcn!=bcn);
				bool qorn = (htr_orn!=orn);
				_cOccupancy_ElectronicsVME.fill(eid);
				if (qevn)
				{std::cout << "1111111111111111111111" << std::endl;
					std::cout << eid << std::endl;
					_cEvnMsm_ElectronicsVME.fill(eid);
				}
				if (qorn)
					_cOrnMsm_ElectronicsVME.fill(eid);
				if (qbcn)
					_cBcnMsm_ElectronicsVME.fill(eid);
			}
		}
		else	// uTCA
		{
			hcal::AMC13Header const* hamc13 = (hcal::AMC13Header const*)
				raw.data();
			if (!hamc13)
				continue;

			uint32_t bcn = hamc13->bunchId();
			uint32_t orn = hamc13->orbitNumber();
			uint32_t evn = hamc13->l1aNumber();
			int namc = hamc13->NAMC();

			for (int iamc=0; iamc<namc; iamc++)
			{
				int slot = hamc13->AMCSlot(iamc);
				int crate = hamc13->AMCId(iamc)&0xFF;
				HcalElectronicsId eid(crate, slot, FIBER_uTCA_MIN1,
					FIBERCH_MIN, false);
				if (_filter_FEDsuTCA.filter(eid))
					continue;
				std::cout << eid << std::endl;
				HcalUHTRData uhtr(hamc13->AMCPayload(iamc),
					hamc13->AMCSize(iamc));

				_cOccupancy_ElectronicsuTCA.fill(eid);
				uint32_t uhtr_evn = uhtr.l1ANumber();
				uint32_t uhtr_bcn = uhtr.bunchNumber();
				uint32_t uhtr_orn = uhtr.orbitNumber();
				bool qevn = (uhtr_evn!=evn);
				bool qbcn = (uhtr_bcn!=bcn);
				bool qorn = (uhtr_orn!=orn);
				if (qevn)
					_cEvnMsm_ElectronicsuTCA.fill(eid);
				if (qorn)
					_cOrnMsm_ElectronicsuTCA.fill(eid);
				if (qbcn)
					_cBcnMsm_ElectronicsuTCA.fill(eid);
			}
		}
	}
}

/* virtual */ void RawTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RawTask);

