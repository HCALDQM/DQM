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
	_cEnergy_SubdetPM.initialize(_name, "Energy", hashfunctions::fSubdetPM,
		new quantity::ValueQuantity(quantity::fEnergy),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTimingCut_SubdetPM.initialize(_name, "Timing", hashfunctions::fSubdetPM,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTimingvsEnergy_FEDSlot.initialize(_name, "TimingvsEnergy",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fEnergy, true),
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_FEDSlot.initialize(_name, "Timing",
		hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN));
	_cTimingCut_FEDVME.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_FEDuTCA.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_ElectronicsVME.initialize(_name, "Timing",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTimingCut_ElectronicsuTCA.initialize(_name, "Timing",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fTiming_ns));


	//	BOOK HISTOGRAMS
	char cutstr[200];
	sprintf(cutstr, "_EHBHE%dHO%dHF%d", int(_cutE_HBHE),
		int(_cutE_HO), int(_cutE_HF));
	char cutstr2[200];
	sprintf(cutstr2, "_EHF%d", int(_cutE_HF));

	_cEnergy_SubdetPM.book(ib, _emap);
	_cTimingCut_SubdetPM.book(ib, _emap);
	_cTimingvsEnergy_FEDSlot.book(ib, _emap);
	_cTimingCut_FEDSlot.book(ib, _emap);
	_cTimingCut_FEDVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_FEDuTCA.book(ib, _emap, _filter_VME);
	_cTimingCut_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cTimingCut_ElectronicsuTCA.book(ib, _emap, _filter_VME);
}

/* virtual */ void RecHitTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void RecHitTask::_process(edm::Event const&,
	edm::EventSetup const&)
{

}

/* virtual */ void RecHitTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RecHitTask);

