#include "DQM/TestTasks/interface/DigiTask.h"

DigiTask::DigiTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));

	_tokHBHE = consumes<HBHEDigiCollection>(_tagHBHE);
	_tokHO = consumes<HODigiCollection>(_tagHO);
	_tokHF = consumes<HFDigiCollection>(_tagHF);

	_cutSumQ_HBHE = ps.getUntrackedParameter<double>("cutSumQ_HBHE", 20);
	_cutSumQ_HO = ps.getUntrackedParameter<double>("cutSumQ_HO", 20);
	_cutSumQ_HF = ps.getUntrackedParameter<double>("cutSumQ_HF", 20);
}

/* virtual */ void DigiTask::bookHistograms(DQMStore::IBooker& ib,
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
	std::vector<uint32_t> vFEDHF;
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);
	vFEDHF.push_back(HcalElectronicsId(22, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	vFEDHF.push_back(HcalElectronicsId(29, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	vFEDHF.push_back(HcalElectronicsId(32, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_FEDHF.initialize(filter::fPreserver, hashfunctions::fFED,
		vFEDHF);

	//	INITIALIZE FIRST
	_cADC_SubdetPM.initialize(_name, "ADC", hashfunctions::fSubdetPM,
		new quantity::ValueQuantity(quantity::fADC_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cfC_SubdetPM.initialize(_name, "fC", hashfunctions::fSubdetPM,
		new quantity::ValueQuantity(quantity::ffC_10000),
		new quantity::ValueQuantity(quantity::fN, true));
	_cSumQ_SubdetPM.initialize(_name, "SumQ", hashfunctions::fSubdetPM,
		new quantity::ValueQuantity(quantity::ffC_10000),
		new quantity::ValueQuantity(quantity::fN, true));
	_cSumQ_depth.initialize(_name, "SumQ", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::ffC_10000));
	_cSumQvsLS_FEDSlot.initialize(_name, "SumQvsLS",
		hashfunctions::fFEDSlot,
		new quantity::LumiSection(_numLSstart),
		new quantity::ValueQuantity(quantity::ffC_10000));
	_cShapeCut_FEDSlot.initialize(_name, "Shape",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::ffC_10000));
	_cTimingCut_FEDSlot.initialize(_name, "Timing",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_FEDVME.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingCut_FEDuTCA.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingCut_ElectronicsVME.initialize(_name, "Timing",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingCut_ElectronicsuTCA.initialize(_name, "Timing",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingCutvsLS_FEDSlot.initialize(_name, "TimingvsLS",
		hashfunctions::fFEDSlot,
		new quantity::LumiSection(),
		new quantity::ValueQuantity(quantity::fTiming_TS200));

	//	Charge sharing
	_cQ2Q12CutvsLS_FEDHFSlot.initialize(_name, "Q2Q12vsLS",
		hashfunctions::fFEDSlot,
		new quantity::LumiSection(_numLSstart),
		new quantity::ValueQuantity(quantity::fRatio_0to2));

	//	Occupancy w/o a cut
	_cOccupancy_FEDVME.initialize(_name, "Occupancy",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancy_FEDuTCA.initialize(_name, "Occupancy",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancy_ElectronicsVME.initialize(_name, "Occupancy",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancy_ElectronicsuTCA.initialize(_name, "Occupancy",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancyvsLS_Subdet.initialize(_name, "OccupancyvsLS",
		hashfunctions::fSubdet,
		new quantity::LumiSection(_numLSstart),
		new quantity::ValueQuantity(quantity::fN_to3000));

	//	Occupancy w/ a cut
	_cOccupancyCut_FEDVME.initialize(_name, "OccupancyCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancyCut_FEDuTCA.initialize(_name, "OccupancyCut",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancyCut_ElectronicsVME.initialize(_name, "OccupancyCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancyCut_ElectronicsuTCA.initialize(_name, "OccupancyCut",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cOccupancyCutvsLS_Subdet.initialize(_name, "OccupancyCutvsLS",
		hashfunctions::fSubdet,
		new quantity::LumiSection(_numLSstart),
		new quantity::ValueQuantity(quantity::fN_to3000));

	_cCapIdRots_FEDVME.initialize(_name, "CapId",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cCapIdRots_FEDuTCA.initialize(_name, "CapId",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));

	//	BOOK HISTOGRAMS
	char cutstr[200];
	sprintf(cutstr, "_SumQHBHE%dHO%dHF%d", int(_cutSumQ_HBHE),
		int(_cutSumQ_HO), int(_cutSumQ_HF));
	char cutstr2[200];
	sprintf(cutstr2, "_SumQHF%d", int(_cutSumQ_HF));

	_cADC_SubdetPM.book(ib, _emap);
	_cfC_SubdetPM.book(ib, _emap);
	_cSumQ_SubdetPM.book(ib, _emap);
	_cSumQ_depth.book(ib, _emap);
	_cSumQvsLS_FEDSlot.book(ib, _emap);

	_cShapeCut_FEDSlot.book(ib, _emap);

	_cTimingCut_FEDSlot.book(ib, _emap);
	_cTimingCut_FEDVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_FEDuTCA.book(ib, _emap, _filter_VME);
	_cTimingCut_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cTimingCutvsLS_FEDSlot.book(ib, _emap);

	_cQ2Q12CutvsLS_FEDHFSlot.book(ib, _emap, _filter_FEDHF);

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

	_cCapIdRots_FEDVME.book(ib, _emap, _filter_uTCA);
	_cCapIdRots_FEDuTCA.book(ib, _emap, _filter_VME);
}

/* virtual */ void DigiTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void DigiTask::_process(edm::Event const& e,
	edm::EventSetup const&)
{
	edm::Handle<HBHEDigiCollection>     chbhe;
	edm::Handle<HODigiCollection>       cho;
	edm::Handle<HFDigiCollection>       chf;

	if (!e.getByToken(_tokHBHE, chbhe))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByToken(_tokHO, cho))
		_logger.dqmthrow("Collection HODigiCollection isn't available"
			+ _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByToken(_tokHF, chf))
		_logger.dqmthrow("Collection HFDigiCollection isn't available"
			+ _tagHF.label() + " " + _tagHF.instance());

	//	HB collection
	int numChs = 0;
	int numChsCus = 0;
	int numChsHE = 0;
	int numChsCusHE = 0;
	for (HBHEDigiCollection::const_iterator it=chbhe->begin(); it!=chbhe->end();
		++it)
	{
		double sumQ = utilities::sumQ<HBHEDataFrame>(*it, 2.5, 0, it->size()-1);
		HcalDetId const& did = it->id();
		HcalElectronicsId const& eid = it->elecId();

		_cSumQ_SubdetPM.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_FEDSlot.fill(eid, _currentLS, sumQ);
		if (eid.isVMEid())
		{
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		for (int i=0; i<it->size(); i++)
		{
			_cADC_SubdetPM.fill(did, it->sample(i).adc());
			_cfC_SubdetPM.fill(did, it->sample(i).nominal_fC());
			if (sumQ>_cutSumQ_HBHE)
				_cShapeCut_FEDSlot.fill(eid, i, it->sample(i).nominal_fC());
		}

		if (sumQ>_cutSumQ_HBHE)
		{
			double timing = utilities::aveTS<HBHEDataFrame>(*it, 2.5, 0,
				it->size()-1);
			_cTimingCut_FEDSlot.fill(eid, timing);
			_cTimingCutvsLS_FEDSlot.fill(eid, _currentLS, timing);
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
			did.subdet()==HcalBarrel?numChsCut++:numChsCutHE;
		}
		did.subdet()==HcalBarrel?numChs++:numChsHE++;
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), numChs);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalBarrel, 1, 1, 1), numChsCut);
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), numChsHE);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalEndcap, 1, 1, 1), numChsCutHE);
	numChs=0;
	numChsCut = 0;

	//	HO collection
	for (HODigiCollection::const_iterator it=cho->begin(); it!=cho->end();
		++it)
	{
		double sumQ = utilities::sumQ<HODataFrame>(*it, 8.5, 0, it->size()-1);
		HcalDetId const& did = it->id();
		HcalElectronicsId const& eid = it->elecId();

		_cSumQ_SubdetPM.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_FEDSlot.fill(eid, _currentLS, sumQ);
		if (eid.isVMEid())
		{
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		for (int i=0; i<it->size(); i++)
		{
			_cADC_SubdetPM.fill(did, it->sample(i).adc());
			_cfC_SubdetPM.fill(did, it->sample(i).nominal_fC());
			if (sumQ>_cutSumQ_HO)
				_cShapeCut_FEDSlot.fill(eid, i, it->sample(i).nominal_fC());
		}

		if (sumQ>_cutSumQ_HO)
		{
			double timing = utilities::aveTS<HODataFrame>(*it, 8.5, 0,
				it->size()-1);
			_cTimingCut_FEDSlot.fill(eid, timing);
			_cTimingCutvsLS_FEDSlot.fill(eid, _currentLS, timing);
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
			numChsCut++;
		}
		numChs++:
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalOuter, 1, 1, 1), numChs);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalOuter, 1, 1, 1), numChsCut);
	numChs=0; numChsCut=0;

	//	HF collection
	for (HFDigiCollection::const_iterator it=chf->begin(); it!=chf->end();
		++it)
	{
		double sumQ = utilities::sumQ<HFDataFrame>(*it, 2.5, 0, it->size()-1);
		HcalDetId const& did = it->id();
		HcalElectronicsId const& eid = it->elecId();

		_cSumQ_SubdetPM.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_FEDSlot.fill(eid, _currentLS, sumQ);
		if (eid.isVMEid())
		{
			_cOccupancy_FEDVME.fill(eid);
			_cOccupancy_ElectronicsVME.fill(eid);
		}
		else
		{
			_cOccupancy_FEDuTCA.fill(eid);
			_cOccupancy_ElectronicsuTCA.fill(eid);
		}

		for (int i=0; i<it->size(); i++)
		{
			_cADC_SubdetPM.fill(did, it->sample(i).adc());
			_cfC_SubdetPM.fill(did, it->sample(i).nominal_fC());
			if (sumQ>_cutSumQ_HF)
				_cShapeCut_FEDSlot.fill(eid, i, it->sample(i).nominal_fC());
		}

		if (sumQ>_cutSumQ_HF)
		{
			double timing = utilities::aveTS<HFDataFrame>(*it, 2.5, 0,
				it->size()-1);
			double q1 = it->sample(1).nominal_fC()-2.5;
			double q2 = it->sample(2).nominal_fC()-2.5;
			double q2q12 = q2/(q1+q2);
			_cTimingCut_FEDSlot.fill(eid, timing);
			_cTimingCutvsLS_FEDSlot.fill(eid, _currentLS, timing);
			_cQ2Q12CutvsLS_FEDHFSlot.fill(eid, _currentLS, q2q12);
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
			numChsCut++;
		}
		numChs++;
	}
	_cOccupancyvsLS_Subdet.fill(HcalDetId(HcalForward, 1, 1, 1), numChs);
	_cOccupancyCutvsLS_Subdet.fill(HcalDetId(HcalForward, 1, 1, 1), numChsCut);
}

/* virtual */ void DigiTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	/*
	 *	
	 */
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		HcalElectronicsId eid = HcalElectronicsId(*it);
		for (int flag=fLowOcp; flag<nDigiFlag; flag++)
			_cSummary.setBinContent(eid, flag, fNA);

		int ncapid = 0;
		int nmissing = 0;
		if (eid.isVMEid())
		{
			//	VME
			for (int is=SPIGOT_MIN; is<=SPIGOT_MAX; is++)
			{
				eid = HcalElectronicsId(FIBERCH_MIN,
					FIBER_VME_MIN, is, eid.dccid());
				ejd = HcalElectronicsId(FIBERCH_MIN,
					FIBER_VME_MIN, is==SPIGOT_MAX?SPIGOT_MIN:is+1,eid.dccid());
				int niscut = _cOccupancyCut_ElectronicsVME.getBinContent(eid);
				int njscut = _cOccupancyCut_ElectronicsVME.getBinContent(eid);
				for (int ifib=FIBER_VME_MIN;ifib<=FIBER_VMEMAX;fib++)
					for (int ifc=FIBERCH_MIN; ifc<+FIBERCH_MAX; ifc++)
					{
						eid=HcalElectronicsId(ifc, ifib, eid.spigot(),
							eid.dccid());
						ncapid+=_cCapIdRots_FEDVME.getBinContent(eid);
						if (_cOccupancy_FEDVME.getBinContent(eid)<1)
						{
							_cMissing1LS_FEDVME.fill(eid);
							nmissing++;
						}
					}
				double ratio = std::min(niscut, njscut)/std::max(niscut, 
					njscut);
				_cSummary.setBinContent(eid, fUniSlot, fGood);
				if (ratio<0.8)
					_cSummary.setBinContent(eid, fUniSlot, fLow);
			}
		}
		else
		{
			//	uTCA
		}

		if (ncapid>0)
			_cSummary.setBinContent(eid, fCapIdRot, fLow);
		else
			_cSummary.setBinContent(eid, fCapIdRot, fGood);
	}
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(DigiTask);

