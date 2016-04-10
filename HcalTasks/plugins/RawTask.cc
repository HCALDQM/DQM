#include "DQM/HcalTasks/interface/RawTask.h"

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
	std::vector<int> vFEDs = utilities::getFEDList(_emap);
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
	{
		vhashFEDsVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
			constants::FIBER_VME_MIN, SPIGOT_MIN,
			(*it)-constants::FED_VME_MIN).rawId());
		_vhashFEDs.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
			constants::FIBER_VME_MIN, SPIGOT_MIN,
			(*it)-constants::FED_VME_MIN).rawId());
	}
	for (std::vector<int>::const_iterator it=vFEDsuTCA.begin();
		it!=vFEDsuTCA.end(); ++it)
	{
		vhashFEDsuTCA.push_back(HcalElectronicsId(
		utilities::fed2crate(*it), SLOT_uTCA_MIN, FIBER_uTCA_MIN1,
		FIBERCH_MIN, false).rawId());
		_vhashFEDs.push_back(HcalElectronicsId(
		utilities::fed2crate(*it), SLOT_uTCA_MIN, FIBER_uTCA_MIN1,
		FIBERCH_MIN, false).rawId());
	}
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
		new quantity::LumiSection(_maxLS),
		new quantity::ValueQuantity(quantity::fN_m0to10000));
	_cBadQualityvsBX.initialize(_name, "BadQualityvsBX",
		new quantity::ValueQuantity(quantity::fBX),
		new quantity::ValueQuantity(quantity::fN_m0to10000));
	_cBadQuality_depth.initialize(_name, "BadQuality",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cBadQualityTotal_FEDVME.initialize(_name, "BadQualityTotal",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cBadQualityTotal_FEDuTCA.initialize(_name, "BadQualityTotal",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));

	//	Summary
	std::vector<std::string> fnames;
	fnames.push_back("EvnMsm");
	fnames.push_back("BcnMsm");
	fnames.push_back("BadQuality");
	_cSummary.initialize(_name, "Summary",
		new quantity::FEDQuantity(vFEDs),
		new quantity::FlagQuantity(fnames),
		new quantity::QualityQuantity());

	//	INITIALIZE HISTOGRAMS to be used in Online Only
	if (_ptype==fOnline)
	{
		_cEvnMsmvsLS_ElectronicsVME.initialize(_name, "EvnMsmvsLS",
			hashfunctions::fElectronics,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cEvnMsmvsLS_ElectronicsuTCA.initialize(_name, "EvnMsmvsLS",
			hashfunctions::fElectronics,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cBcnMsmvsLS_ElectronicsVME.initialize(_name, "BcnMsmvsLS",
			hashfunctions::fElectronics,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cBcnMsmvsLS_ElectronicsuTCA.initialize(_name, "BcnMsmvsLS",
			hashfunctions::fElectronics,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
	}

	//	BOOK HISTOGRAMS
	_cEvnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cBcnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cOrnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cEvnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cBcnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cOrnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);


	_cBadQuality_FEDVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cBadQuality_FEDuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cBadQualityTotal_FEDVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cBadQualityTotal_FEDuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cBadQuality_depth.book(ib, _emap, _subsystem);
	_cBadQualityvsLS.book(ib, _subsystem);
	_cBadQualityvsBX.book(ib, _subsystem);

	_cSummary.book(ib, _subsystem);

	// BOOK HISTOGRAMS to be used in ONLINE ONLY!
	if (_ptype==fOnline)
	{
		_cEvnMsmvsLS_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
		_cEvnMsmvsLS_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
		_cBcnMsmvsLS_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
		_cBcnMsmvsLS_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	}

	//	initialize hash map
	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fD2EHashMap);
}

/* virtual */ void RawTask::_resetMonitors(UpdateFreq uf)
{
	switch(uf)
	{
		case fEvent:
			break;
		case hcaldqm::f1LS:
			_cEvnMsm_ElectronicsVME.reset();
			_cBcnMsm_ElectronicsVME.reset();
			_cEvnMsm_ElectronicsuTCA.reset();
			_cBcnMsm_ElectronicsuTCA.reset();
			_cBadQuality_FEDVME.reset();
			_cBadQuality_FEDuTCA.reset();
			_cBadQuality_depth.reset();
			break;
		default:
			break;
	}
	
	//	base reset
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

	//	extract some info
	int bx = e.bunchCrossing();

	
	int nn = 0;
	//	loop thru and fill the detIds with bad quality
	//	NOTE: Calibration Channels are skipped!
	//	TODO: Include for Online Calibration Channels marked as bad
	//	a comment below is left on purpose!
	//_cBadQualityvsBX.fill(bx, creport->badQualityDigis());
	for (std::vector<DetId>::const_iterator it=creport->bad_quality_begin();
		it!=creport->bad_quality_end(); ++it)
	{
		if (!HcalGenericDetId(*it).isHcalDetId())
			continue;

		nn++;
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(*it));
		_cBadQuality_depth.fill(HcalDetId(*it));
		if (eid.isVMEid())
		{
			if (_filter_FEDsVME.filter(eid))
				continue;
			_cBadQuality_FEDVME.fill(eid);
		}
		else
		{
			if (_filter_FEDsuTCA.filter(eid))
				continue;
			_cBadQuality_FEDuTCA.fill(eid);
		}
	}
	_cBadQualityvsLS.fill(_currentLS,nn);
	_cBadQualityvsBX.fill(bx, nn);

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
				if (qevn)
				{
					_cEvnMsm_ElectronicsVME.fill(eid);

					if (_ptype==fOnline && is<=constants::SPIGOT_MAX)
						_cEvnMsmvsLS_ElectronicsVME.fill(eid, 100);
				}
				if (qorn)
					_cOrnMsm_ElectronicsVME.fill(eid);
				if (qbcn)
				{
					_cBcnMsm_ElectronicsVME.fill(eid);

					if (_ptype==fOnline && is<=constants::SPIGOT_MAX)
						_cBcnMsmvsLS_ElectronicsVME.fill(eid, 100);
				}
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
				HcalUHTRData uhtr(hamc13->AMCPayload(iamc),
					hamc13->AMCSize(iamc));

				uint32_t uhtr_evn = uhtr.l1ANumber();
				uint32_t uhtr_bcn = uhtr.bunchNumber();
				uint32_t uhtr_orn = uhtr.orbitNumber();
				bool qevn = (uhtr_evn!=evn);
				bool qbcn = (uhtr_bcn!=bcn);
				bool qorn = (uhtr_orn!=orn);
				if (qevn)
				{
					_cEvnMsm_ElectronicsuTCA.fill(eid);

					if (_ptype==fOnline)
						_cEvnMsmvsLS_ElectronicsuTCA.fill(eid, 100);
				}
				if (qorn)
					_cOrnMsm_ElectronicsuTCA.fill(eid);
				if (qbcn)
				{
					_cBcnMsm_ElectronicsuTCA.fill(eid);

					if (_ptype==fOnline)
						_cBcnMsmvsLS_ElectronicsuTCA.fill(eid, 100);
				}
			}
		}
	}
}

/* virtual */ void RawTask::beginLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	DQTask::beginLuminosityBlock(lb, es);

//	_cBadQualityvsLS.extendAxisRange();
}

/* virtual */ void RawTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	/*
	 
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		//	first set all flags as not-applicable
		HcalElectronicsId eid = HcalElectronicsId(*it);
		for (int flag=fEvnMsm; flag<nRawFlags; flag++)
		{
			_cSummary.setBinContent(eid, flag, fNA);
		}

		//	second, check if this FED is @cDAQ
		//	if not, leave status as inapplicable!
		std::vector<uint32_t> jt = std::find(_vcdaqEids.begin(),
			_vcdaqEids.end(), *it);
		if (jt==_vcdaqEids.end())
			continue;

		//	loop over and set status
		int nevnmsm = 0;
		int nbcnmsm = 0;
		int nbad = 0;
		if (eid.isVMEid()) //	VME
		{
			for (int is=constants::SPIGOT_MIN; is<=SPIGOT_MAX; is++)
			{
				eid = HcalElectronicsId(constants::FIBERCH_MIN,
					FIBER_VME_MIN, eid.spigot(), eid.dccid());
				nevnmsm += _cEvnMsm_ElectronicsVME.getBinContent(eid);
				nbcnmsm += _cBcnMsm_ElectronicsVME.getBinContent(eid);
				for (int ifib=constants::FIBER_VME_MIN; 
						ifib<=constants::FIBER_VME_MAX; ifib++)
					for (int ifc=constants::FIBERCH_MIN; 
						ifc<=constants::FIBERCH_MAX; ifc++)
					{
						eid = HcalElectronicsId(ifc, ifib, eid.spigot(),
							eid.dccid());
						int nn =_cBadQuality_FEDVME.getBinContent(eid);
						nbad+=nn>0?1:0;
						if (nn>0)
							_cBadQualityTotal_FEDVME.fill(eid);
					}
			}
		}
		else	// uTCA only here
		{
			for (int is=constants::SLOT_uTCA_MIN; 
				is<=constants::SLOT_uTCA_MAX; is++)
			{
				eid = HcalElectronicsId(eid.crateId(), is,
					constants::FIBER_uTCA_MIN1, constants::FIBERCH_MIN, false);
				nevnmsm += _cEvnMsm_ElectronicsuTCA.getBinContent(eid);
				nbcnmsm += _cBcnMsm_ElectronicsuTCA.getBinContent(eid);
				for (int ifib=constants::FIBER_uTCA_MIN1;
					ifib<=constants::FIBER_uTCA_MAX2; ifib++)
				{
					if (ifib>FIBER_uTCA_MAX1 && ifib<FIBER_uTCA_MIN2)
						continue;
					for (int ifc=FIBERCH_MIN; ifc<=FIBERCH_MAX; ifc++)
					{
						eid = HcalElectronicsId(eid.crateId(), 
							eid.slot(), ifib, ifc, false);
						int nn=_cBadQuality_FEDuTCA.getBinContent(eid);
						nbad+=nn>0?1:0;
						if (nn>0)
							_cBadQualityTotal_FEDuTCA.fill(eid);
					}
				}
			}
		}
		
		// set all the quality flags
		nevnmsm>0?
			_cSummary.setBinContent(eid, (int)fEvnMsm, (int)fLow):
			_cSummary.setBinContent(eid, (int)fEvnMsm, (int)fGood);
		nbcnmsm>0?
			_cSummary.setBinContent(eid, (int)fBcnMsm, (int)fLow):
			_cSummary.setBinContent(eid, (int)fBcnMsm, (int)fGood);
		nbad>0?
			_cSummary.setBinContent(eid, (int)fBadQuality, (int)fLow):
			_cSummary.setBinContent(eid, (int)fBadQuality, (int)fGood);
	}
	*/	

	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RawTask);

