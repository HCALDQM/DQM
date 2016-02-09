#include "DQM/TestTasks/interface/RawTask.h"

RawTask::RawTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_tagFEDs = ps.getUntrackedParameter<edm::InputTag>("tagFEDs",
		edm::InputTag("rawDataCollector"));

	_tokFEDs = consumes<FEDRawDataCollection>(_tagFEDs);
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
	vVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN, 
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vuTCA);

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


	//	BOOK HISTOGRAMS
	_cEvnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cBcnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOrnMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cEvnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cBcnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOrnMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME);

	_cOccupancy_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancy_ElectronicsuTCA.book(ib, _emap, _filter_VME);
}

/* virtual */ void RawTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void RawTask::_process(edm::Event const&,
	edm::EventSetup const&)
{

}

/* virtual */ void RawTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(RawTask);

