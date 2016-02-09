#include "DQM/TestTasks/interface/DigiTask.h"

DigiTask::DigiTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagHBHE = ps.getUntrackedParameter(edm::InputTag)("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter(edm::InputTag)("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter(edm::InputTag)("tagHF",
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
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);

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
		new quantity::ValueQuantity(quantity::fLS),
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
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_FEDuTCA.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_ElectronicsVME.initialize(_name, "Timing",
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingCut_ElectronicsuTCA.initialize(_name, "Timing",
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_TS200));


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
}

/* virtual */ void DigiTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void DigiTask::_process(edm::Event const&,
	edm::EventSetup const&)
{

}

/* virtual */ void DigiTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es)
}

DEFINE_FWK_MODULE(DigiTask);

