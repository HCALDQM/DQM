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

	//	INITIALIZE FIRST
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
	_cShapeData_TTSubdet.initialize(_name, "ShapeData",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fEt_128));
	_cShapeEmul_TTSubdet.initialize(_name, "ShapeEmul",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fEt_128));
	_cOccupancyData_TTdepth.initialize(_name, "OccupancyData",
		hashfunctions::fTTdepth,
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyEmul_TTdepth.initialize(_name, "OccupancyEmul",
		hashfunctions::fTTdepth,
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN));
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


	//	BOOK HISTOGRAMS
	_cEtData_TTSubdet.book(ib, _emap);
	_cEtEmul_TTSubdet.book(ib, _emap);
	_cEtCorr_TTSubdet.book(ib, _emap);
	_cFGCorr_TTSubdet.book(ib, _emap);
	_cShapeData_TTSubdet.book(ib, _emap);
	_cShapeEmul_TTSubdet.book(ib, _emap);
	_cOccupancyData_TTdepth.book(ib, _emap, _filter_depth0);
	_cOccupancyEmul_TTdepth.book(ib, _emap, _filter_depth0);
	_cOccupancyData_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA);
	_cOccupancyData_ElectronicsuTCA.book(ib, _emap, _filter_VME);
	_cOccupancyEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME);
}

/* virtual */ void TPTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void TPTask::_process(edm::Event const&,
	edm::EventSetup const&)
{

}

/* virtual */ void TPTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(TPTask);

