#include "DQM/HcalTasks/interface/TPTask.h"

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
	_cutEt = ps.getUntrackedParameter<int>("cutEt", 3);
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
	vVME.push_back(HcalElectronicsId(FIBERCH_MIN, 
		FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
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
		_vhashFEDs.push_back(HcalElectronicsId(SLBCH_MIN, SLB_MIN,
			SPIGOT_MIN, (*it)-FED_VME_MIN, CRATE_VME_MIN,
			SLOT_VME_MIN1, 0).rawId());
	for (std::vector<int>::const_iterator it=vFEDsuTCA.begin();
		it!=vFEDsuTCA.end(); ++it)
		_vhashFEDs.push_back(HcalElectronicsId(utilities::fed2crate(*it), 
			SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, true).rawId());

	//	INITIALIZE FIRST
	//	Et/FG
	_cEtData_TTSubdet.initialize(_name, "EtData", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEtEmul_TTSubdet.initialize(_name, "EtEmul", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEt_128),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEtCorr_TTSubdet.initialize(_name, "EtCorr", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fEtCorr_256),
		new quantity::ValueQuantity(quantity::fEtCorr_256),
		new quantity::ValueQuantity(quantity::fN, true));
	_cFGCorr_TTSubdet.initialize(_name, "FGCorr", hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fFG),
		new quantity::ValueQuantity(quantity::fFG),
		new quantity::ValueQuantity(quantity::fN, true));

	_cEtData_ElectronicsVME.initialize(_name, "EtData", 
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtData_ElectronicsuTCA.initialize(_name, "EtData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtEmul_ElectronicsVME.initialize(_name, "EtEmul", 
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtEmul_ElectronicsuTCA.initialize(_name, "EtEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtData_depthlike.initialize(_name, "EtData",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtEmul_depthlike.initialize(_name, "EtEmul",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtCutData_depthlike.initialize(_name, "EtCutData",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fEt_256));
	_cEtCutEmul_depthlike.initialize(_name, "EtCutEmul",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fEt_256));

	//	Occupancies
	_cOccupancyData_ElectronicsVME.initialize(_name, "OccupancyData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyEmul_ElectronicsVME.initialize(_name, "OccupancyEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyData_ElectronicsuTCA.initialize(_name, "OccupancyData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyEmul_ElectronicsuTCA.initialize(_name, "OccupancyEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN, true));

	_cOccupancyCutData_ElectronicsVME.initialize(_name, "OccupancyCutData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyCutEmul_ElectronicsVME.initialize(_name, "OccupancyCutEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyCutData_ElectronicsuTCA.initialize(_name, "OccupancyCutData",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyCutEmul_ElectronicsuTCA.initialize(_name, "OccupancyCutEmul",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN, true));

	_cOccupancyData_depthlike.initialize(_name, "OccupancyData",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyEmul_depthlike.initialize(_name, "OccupancyEmul",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyCutData_depthlike.initialize(_name, "OccupancyCutData",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancyCutEmul_depthlike.initialize(_name, "OccupancyCutEmul",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN, true));

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
	_cEtMsm_depthlike.initialize(_name, "EtMsm",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
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
	_cMsnData_depthlike.initialize(_name, "MsnData",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cMsnEmul_depthlike.initialize(_name, "MsnEmul",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cEtCorrRatio_ElectronicsVME.initialize(_name, "EtCorrRatio",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsVME),
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ValueQuantity(quantity::fRatio_0to2));
	_cEtCorrRatio_ElectronicsuTCA.initialize(_name, "EtCorrRatio",
		hashfunctions::fElectronics,
		new quantity::FEDQuantity(vFEDsuTCA),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fRatio_0to2));
	_cEtCorrRatio_depthlike.initialize(_name, "EtCorrRatio",
		new quantity::TrigTowerQuantity(quantity::fTTieta),
		new quantity::TrigTowerQuantity(quantity::fTTiphi),
		new quantity::ValueQuantity(quantity::fRatio_0to2));

	_cOccupancyDatavsBX_TTSubdet.initialize(_name, "OccupancyDatavsBX",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fBX),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyEmulvsBX_TTSubdet.initialize(_name, "OccupancyEmulvsBX",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fBX),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCutDatavsBX_TTSubdet.initialize(_name, "OccupancyCutDatavsBX",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fBX),
		new quantity::ValueQuantity(quantity::fN));
	_cOccupancyCutEmulvsBX_TTSubdet.initialize(_name, "OccupancyCutEmulvsBX",
		hashfunctions::fTTSubdet,
		new quantity::ValueQuantity(quantity::fBX),
		new quantity::ValueQuantity(quantity::fN));

	//	INITIALIZE HISTOGRAMS to be used in Online only!
	if (_ptype==fOnline)
	{
		_cEtCorr2x3_TTSubdet.initialize(_name, "EtCorr2x3", 
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fEtCorr_256),
			new quantity::ValueQuantity(quantity::fEtCorr_256),
			new quantity::ValueQuantity(quantity::fN, true));
		_cOccupancyData2x3_depthlike.initialize(_name, "OccupancyData2x3",
			new quantity::TrigTowerQuantity(quantity::fTTieta2x3),
			new quantity::TrigTowerQuantity(quantity::fTTiphi),
			new quantity::ValueQuantity(quantity::fN, true));
		_cOccupancyEmul2x3_depthlike.initialize(_name, "OccupancyEmul2x3",
			new quantity::TrigTowerQuantity(quantity::fTTieta2x3),
			new quantity::TrigTowerQuantity(quantity::fTTiphi),
			new quantity::ValueQuantity(quantity::fN, true));
		_cEtCutDatavsLS_TTSubdet.initialize(_name, "EtCutDatavsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fEtCorr_256));
		_cEtCutEmulvsLS_TTSubdet.initialize(_name, "EtCutEmulvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fEtCorr_256));
		_cEtCutDatavsBX_TTSubdet.initialize(_name, "EtCutDatavsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fEtCorr_256));
		_cEtCutEmulvsBX_TTSubdet.initialize(_name, "EtCutEmulvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fEtCorr_256));
		_cEtCorrRatiovsLS_TTSubdet.initialize(_name, "EtCorrRatiovsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fRatio_0to2));
		_cEtCorrRatiovsBX_TTSubdet.initialize(_name, "EtCorrRatiovsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fRatio_0to2));
		_cEtMsmvsLS_TTSubdet.initialize(_name, "EtMsmvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cEtMsmvsBX_TTSubdet.initialize(_name, "EtMsmvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnDatavsLS_TTSubdet.initialize(_name, "MsnDatavsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnCutDatavsLS_TTSubdet.initialize(_name, "MsnCutDatavsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnDatavsBX_TTSubdet.initialize(_name, "MsnDatavsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnCutDatavsBX_TTSubdet.initialize(_name, "MsnCutDatavsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnEmulvsLS_TTSubdet.initialize(_name, "MsnEmulvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnCutEmulvsLS_TTSubdet.initialize(_name, "MsnCutEmulvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnEmulvsBX_TTSubdet.initialize(_name, "MsnEmulvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cMsnCutEmulvsBX_TTSubdet.initialize(_name, "MsnCutEmulvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyDatavsLS_TTSubdet.initialize(_name, "OccupancyDatavsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyCutDatavsLS_TTSubdet.initialize(_name, 
			"OccupancyCutDatavsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyDatavsBX_TTSubdet.initialize(_name, "OccupancyDatavsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyCutDatavsBX_TTSubdet.initialize(_name, 
			"OccupancyCutDatavsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyEmulvsLS_TTSubdet.initialize(_name, "OccupancyEmulvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyCutEmulvsLS_TTSubdet.initialize(_name, 
			"OccupancyCutEmulvsLS",
			hashfunctions::fTTSubdet,
			new quantity::LumiSection(_maxLS),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyEmulvsBX_TTSubdet.initialize(_name, "OccupancyEmulvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
		_cOccupancyCutEmulvsBX_TTSubdet.initialize(_name, 
			"OccupancyCutEmulvsBX",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fBX),
			new quantity::ValueQuantity(quantity::fN));
	}

	std::vector<std::string> fnames;
	fnames.push_back("OcpUniSlotD");
	fnames.push_back("OcpUniSlotE");
	fnames.push_back("EtMsmUniSlot");
	fnames.push_back("FGMsmUniSlot");
	fnames.push_back("MsnUniSlotD");
	fnames.push_back("MsnUniSlotE");
	fnames.push_back("EtCorrRatio");
	fnames.push_back("EtMsmRatio");
	fnames.push_back("FGMsmNumber");
	_cSummary.initialize(_name, "Summary",
		new quantity::FEDQuantity(vFEDs),
		new quantity::FlagQuantity(fnames),
		new quantity::QualityQuantity());

	//	BOOK HISTOGRAMS
	_cEtData_TTSubdet.book(ib, _emap, _subsystem);
	_cEtEmul_TTSubdet.book(ib, _emap, _subsystem);
	_cEtCorr_TTSubdet.book(ib, _emap, _subsystem);
	_cFGCorr_TTSubdet.book(ib, _emap, _subsystem);
	_cEtData_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cEtData_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cEtEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cEtEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cEtData_depthlike.book(ib, _subsystem);
	_cEtEmul_depthlike.book(ib, _subsystem);
	_cEtCutData_depthlike.book(ib, _subsystem);
	_cEtCutEmul_depthlike.book(ib, _subsystem);
	_cOccupancyData_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cOccupancyEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cOccupancyData_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cOccupancyEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cOccupancyCutData_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cOccupancyCutEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cOccupancyCutData_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cOccupancyCutEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cOccupancyData_depthlike.book(ib, _subsystem);
	_cOccupancyEmul_depthlike.book(ib, _subsystem);
	_cOccupancyCutData_depthlike.book(ib, _subsystem);
	_cOccupancyCutEmul_depthlike.book(ib, _subsystem);
	_cEtMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cEtMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cFGMsm_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cFGMsm_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cMsnData_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cMsnData_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cMsnEmul_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cMsnEmul_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cEtCorrRatio_ElectronicsVME.book(ib, _emap, _filter_uTCA, _subsystem);
	_cEtCorrRatio_ElectronicsuTCA.book(ib, _emap, _filter_VME, _subsystem);
	_cEtCorrRatio_depthlike.book(ib, _subsystem);
	_cEtMsm_depthlike.book(ib, _subsystem);
	_cMsnData_depthlike.book(ib, _subsystem);
	_cMsnEmul_depthlike.book(ib, _subsystem);
	_cSummary.book(ib, _subsystem);

	//	whatever has to go online only goes here
	if (_ptype==fOnline)
	{
		_cEtCorr2x3_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyData2x3_depthlike.book(ib, _subsystem);
		_cOccupancyEmul2x3_depthlike.book(ib, _subsystem);
		_cEtCutDatavsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cEtCutEmulvsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cEtCutDatavsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cEtCutEmulvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cEtCorrRatiovsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cEtCorrRatiovsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cEtMsmvsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cEtMsmvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnDatavsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnCutDatavsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnDatavsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnCutDatavsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnEmulvsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnCutEmulvsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnEmulvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cMsnCutEmulvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyDatavsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyEmulvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyCutDatavsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyCutEmulvsBX_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyDatavsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyEmulvsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyCutDatavsLS_TTSubdet.book(ib, _emap, _subsystem);
		_cOccupancyCutEmulvsLS_TTSubdet.book(ib, _emap, _subsystem);
	}
	
	//	initialize the hash map
	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fT2EHashMap);
}

/* virtual */ void TPTask::_resetMonitors(UpdateFreq uf)
{
	/*
	switch (uf)
	{
		case hcaldqm::f1LS:
			_cOccupancyData_ElectronicsVME.reset();
			_cOccupancyEmul_ElectronicsVME.reset();
			_cOccupancyData_ElectronicsuTCA.reset();
			_cOccupancyEmul_ElectronicsuTCA.reset();
			_cEtMsm_ElectronicsuTCA.reset();
			_cEtMsm_ElectronicsVME.reset();
			_cFGMsm_ElectronicsuTCA.reset();
			_cFGMsm_ElectronicsVME.reset();
			break;
		case hcaldqm::f10LS:
			_cEtCorrRatio_ElectronicsVME.reset();
			_cEtCorrRatio_ElectronicsuTCA.reset();
			break;
		default:
			break;
	}
	*/

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

	//	extract some info per event
	int bx = e.bunchCrossing();

	//	tmp
	bool useD1 = false;
	//	\tmp

	//	some summaries... per event
	int numHBHE(0), numHF(0), numCutHBHE(0), numCutHF(0);
	int numMsmHBHE(0), numMsmHF(0);
	int numMsnHBHE(0), numMsnHF(0), numMsnCutHBHE(0), numMsnCutHF(0);

	/*
	 * STEP1: 
	 * Loop over the data digis and 
	 * - do ... for all the data digis
	 * - find the emulator digi
	 * --- compare soi Et
	 * --- compare soi FG
	 * --- Do not fill anything for emulator Et!!!
	 */
	for (HcalTrigPrimDigiCollection::const_iterator it=cdata->begin();
		it!=cdata->end(); ++it)
	{
		HcalTrigTowerDetId tid = it->id();

		//
		//	HF 2x3 TPs Treat theam separately and only for ONLINE!
		//
		if (tid.version()==0 && tid.ietaAbs()>=29)
		{
			//	do this only for online processing
			if (_ptype==fOnline)
			{
				_cOccupancyData2x3_depthlike.fill(tid);
				HcalTrigPrimDigiCollection::const_iterator jt=cemul->find(
					HcalTrigTowerDetId(tid.ieta(), tid.iphi(), 0));
				if (jt!=cemul->end())
					_cEtCorr2x3_TTSubdet.fill(tid, it->SOI_compressedEt(),
						jt->SOI_compressedEt());
			}

			//	skip to the next tp digi
			continue;
		}

		//	FROM THIS POINT, HBHE + 1x1 HF TPs
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(tid));
		int soiEt_d = it->SOI_compressedEt();
		int soiFG_d = it->SOI_fineGrain()?1:0;
		tid.ietaAbs()>=29?numHF++:numHBHE++;

		//	tmp
		if (tid.depth()==1)
			useD1 = true;
		//	\tmp
	
		//	 fill w/o a cut
		_cEtData_TTSubdet.fill(tid, soiEt_d);
		_cEtData_depthlike.fill(tid, soiEt_d);
		_cOccupancyData_depthlike.fill(tid);
		if (eid.isVMEid())
		{
			_cOccupancyData_ElectronicsVME.fill(eid);
			_cEtData_ElectronicsVME.fill(eid, soiEt_d);
		}
		else
		{
			_cOccupancyData_ElectronicsuTCA.fill(eid);
			_cEtData_ElectronicsuTCA.fill(eid, soiEt_d);
		}
		
		//	FILL w/a CUT
		if (soiEt_d>_cutEt)
		{
			tid.ietaAbs()>=29?numCutHF++:numCutHBHE++;
			_cOccupancyCutData_depthlike.fill(tid);
			_cEtCutData_depthlike.fill(tid, soiEt_d);

			//	ONLINE ONLY!
			if (_ptype==fOnline)
			{
				_cEtCutDatavsLS_TTSubdet.fill(tid, _currentLS, soiEt_d);
				_cEtCutDatavsBX_TTSubdet.fill(tid, bx, soiEt_d);
			}
			//	^^^ONLINE ONLY!

			if (eid.isVMEid())
				_cOccupancyCutData_ElectronicsVME.fill(eid);
			else
				_cOccupancyCutData_ElectronicsuTCA.fill(eid);
		}

		//	FIND the EMULATOR DIGI
		HcalTrigPrimDigiCollection::const_iterator jt=cemul->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), 0));
		if (jt!=cemul->end())
		{
			//	if PRESENT!
			int soiEt_e = jt->SOI_compressedEt();
			int soiFG_e = jt->SOI_fineGrain()?1:0;
			//	if both are zeroes => set 1
			double rEt = soiEt_d==0 && soiEt_e==0?1:
				double(std::min(soiEt_d, soiEt_e))/
				double(std::max(soiEt_e, soiEt_d));

			//	ONLINE ONLY!
			if (_ptype==fOnline)
			{		
				_cEtCorrRatiovsLS_TTSubdet.fill(tid, _currentLS, rEt);
				_cEtCorrRatiovsBX_TTSubdet.fill(tid, bx, rEt);
			}
			//	^^^ONLINE ONLY!

			_cEtCorrRatio_depthlike.fill(tid, rEt);
			_cEtCorr_TTSubdet.fill(tid, soiEt_d, soiEt_e);
			_cFGCorr_TTSubdet.fill(tid, soiFG_d, soiFG_e);
			//	FILL w/o a CUT
			if (eid.isVMEid())
			{
				_cEtCorrRatio_ElectronicsVME.fill(eid, rEt);
			}
			else
			{
				_cEtCorrRatio_ElectronicsuTCA.fill(eid, rEt);
			}

			//	if SOI Et are not equal
			//	fill mismatched
			if (soiEt_d!=soiEt_e)
			{
				tid.ietaAbs()>=29?numMsmHF++:numMsmHBHE++;
				_cEtMsm_depthlike.fill(tid);
				if (eid.isVMEid())
					_cEtMsm_ElectronicsVME.fill(eid);
				else
					_cEtMsm_ElectronicsuTCA.fill(eid);
			}
			//	 if SOI FG are not equal
			//	 fill mismatched
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
			//	IF MISSING
			_cEtCorr_TTSubdet.fill(tid, soiEt_d, -2);
			_cMsnEmul_depthlike.fill(tid);
			tid.ietaAbs()>=29?numMsnHF++:numMsnHBHE++;
			if (eid.isVMEid())
				_cMsnEmul_ElectronicsVME.fill(eid);
			else
				_cMsnEmul_ElectronicsuTCA.fill(eid);

			if (soiEt_d>_cutEt)
				tid.ietaAbs()>=29?numMsnCutHF++:numMsnCutHBHE++;
		}
	}
	
	//	ONLINE ONLY!
	if (_ptype==fOnline)
	{
		_cOccupancyDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1), bx,
			numHBHE);
		_cOccupancyDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1), bx,
			numHF);
		_cOccupancyCutDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1), bx,
			numCutHBHE);
		_cOccupancyCutDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1), bx,
			numCutHF);
		_cOccupancyDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1), 
			_currentLS, numHBHE);
		_cOccupancyDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1), 
			_currentLS,numHF);
		_cOccupancyCutDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numCutHBHE);
		_cOccupancyCutDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1), 
			_currentLS, numCutHF);

		_cEtMsmvsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1), _currentLS,
			numMsmHBHE);
		_cEtMsmvsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1), _currentLS, 
			numMsmHF);
		_cEtMsmvsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1), bx,
			numMsmHBHE);
		_cEtMsmvsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1), bx, 
			numMsmHF);

		_cMsnEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numMsnHBHE);
		_cMsnEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			_currentLS, numMsnHF);
		_cMsnCutEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numMsnCutHBHE);
		_cMsnCutEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			_currentLS, numMsnCutHF);

		_cMsnEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			bx, numMsnHBHE);
		_cMsnEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			bx, numMsnHF);
		_cMsnCutEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			bx, numMsnCutHBHE);
		_cMsnCutEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			bx, numMsnCutHF);
	}

	numHBHE=0; numHF=0; numCutHBHE=0; numCutHF=0;
	numMsnHBHE=0; numMsnHF=0; numCutHBHE=0; numCutHF=0;

	/*
	 *	STEP2:
	 *	Loop over the emulator digis and 
	 *	- do ... for all the emulator digis
	 *	- find data digi and 
	 *	--- if found skip
	 *	--- if not found - fill the missing Data plot
	 */
	for (HcalTrigPrimDigiCollection::const_iterator it=cemul->begin();
		it!=cemul->end(); ++it)
	{
		HcalTrigTowerDetId tid = it->id();

		//	HF 2x3 TPs. Only do it for Online!!!
		if (tid.version()==0 && tid.ietaAbs()>=29)
		{
			//	only do this for online processing
			if (_ptype==fOnline)
				_cOccupancyEmul2x3_depthlike.fill(tid);
			continue;
		}
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), useD1?1:0)));
		int soiEt = it->SOI_compressedEt();

		//	FILL/INCREMENT w/o a CUT
		tid.ietaAbs()>=29?numHF++:numHBHE++;
		_cEtEmul_TTSubdet.fill(tid, soiEt);
		_cEtEmul_depthlike.fill(tid, soiEt);
		_cOccupancyEmul_depthlike.fill(tid);
		if (eid.isVMEid())
		{
			_cOccupancyEmul_ElectronicsVME.fill(eid);
			_cEtEmul_ElectronicsVME.fill(eid, soiEt);
		}
		else
		{
			_cOccupancyEmul_ElectronicsuTCA.fill(eid);
			_cEtEmul_ElectronicsuTCA.fill(eid, soiEt);
		}

		//	FILL w/ a CUT
		if (soiEt>_cutEt)
		{
			tid.ietaAbs()>=29?numCutHF++:numCutHBHE++;
			_cOccupancyCutEmul_depthlike.fill(tid);
			_cEtCutEmul_depthlike.fill(tid, soiEt);
			if (eid.isVMEid())
				_cOccupancyCutEmul_ElectronicsVME.fill(eid);
			else 
				_cOccupancyCutEmul_ElectronicsuTCA.fill(eid);

			//	ONLINE ONLY!
			if (_ptype==fOnline)
			{
				_cEtCutEmulvsLS_TTSubdet.fill(tid, _currentLS, soiEt);
				_cEtCutEmulvsBX_TTSubdet.fill(tid, bx, soiEt);
			}
			//	^^^ONLINE ONLY!
		}

		//	FIND a data digi
		HcalTrigPrimDigiCollection::const_iterator jt=cdata->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), useD1?1:0));
		if (jt==cdata->end())
		{
			tid.ietaAbs()>=29?numMsnHF++:numMsnHBHE++;
			_cEtCorr_TTSubdet.fill(tid, -2, soiEt);
			if (eid.isVMEid())
				_cMsnData_ElectronicsVME.fill(eid);
			else
				_cMsnData_ElectronicsuTCA.fill(eid);
			if (soiEt>_cutEt)
				tid.ietaAbs()>=29?numMsnCutHF++:numMsnCutHBHE++;
		}
	}

	//	ONLINE ONLY!
	if (_ptype==fOnline)
	{
		_cOccupancyEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1), bx,
			numHBHE);
		_cOccupancyEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1), bx,
			numHF);
		_cOccupancyCutEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1), bx,
			numCutHBHE);
		_cOccupancyCutEmulvsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1), bx,
			numCutHF);

		_cOccupancyEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1), 
			_currentLS, numHBHE);
		_cOccupancyEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1), 
			_currentLS,numHF);
		_cOccupancyCutEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numCutHBHE);
		_cOccupancyCutEmulvsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1), 
			_currentLS, numCutHF);

		_cMsnDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numMsnHBHE);
		_cMsnDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			_currentLS, numMsnHF);
		_cMsnCutDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			_currentLS, numMsnCutHBHE);
		_cMsnCutDatavsLS_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			_currentLS, numMsnCutHF);

		_cMsnDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			bx, numMsnHBHE);
		_cMsnDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			bx, numMsnHF);
		_cMsnCutDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(1,1),
			bx, numMsnCutHBHE);
		_cMsnCutDatavsBX_TTSubdet.fill(HcalTrigTowerDetId(29,1),
			bx, numMsnCutHF);
	}
	//	^^^ONLINE ONLY!
}

/* virtual */ void TPTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	/*
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		//	first, set all the flags are inapplicable	
		HcalElectronicsId eid = HcalElectronicsId(*it);
		for (int flag=fOcpUniSlotData; flag<nTPFlag; flag++)
			_cSummary.setBinContent(eid, flag, fNA);
		//	for now just skip VME - but there gonna be HO TPs soon
		if (eid.isVMEid())
			continue;

		//	second, check if this FED is @cDAQ
		//	if not, leave status as inapplicable
		std::vector<uint32_t> jt=std::find(_vcdaqEids.begin(),
			_vcdaqEids.end(), *it);
		if (jt==_vcdaqEids.end())
			continue;

		//	loop over and check the status
		bool ocpUniSlotData = false;
		bool ocpUniSlotEmul = false;
		bool etMsmUniSlot = false;
		bool fgMsmUniSlot = false;
		bool etcorrratio = false;
		bool etmsmnum = false;
		bool fgmsmnum = false;
		if (eid.isVMEid())
		{
			//	VME
			for (int is=SPIGOT_MIN; is<=SPIGOT_MAX; is++)
			{
				//	NOTE: non Trigger Constructor
				eid = HcalElectronicsId(FIBERCH_MIN,
					FIBER_VME_MIN, is, eid.dccid());
				HcalElectronicsId ejd = HcalElectronicsId(FIBERCH_MIN,
					FIBER_VME_MIN, is==SPIGOT_MAX?SPIGOT_MIN:is+1, eid.dccid());

				//	get Contents
				int iocpd = _cOccupancyData_ElectronicsVME.getBinContent(eid);
				int iocpe = _cOccupancyEmul_ElectronicsVME.getBinContent(eid);
				int jocpd = _cOccupancyData_ElectronicsVME.getBinContent(ejd);
				int jocpe = _cOccupancyEmul_ElectronicsVME.getBinContent(ejd);
				int ietmsm = _cEtMsm_ElectronicsVME.getBinContent(eid);
				int jetmsm = _cEtMsm_ElectronicsVME.getBinContent(ejd);
				int ifgmsm = _cFGMsm_ElectronicsVME.getBinContent(eid);
				int jfgmsm = _cFGMsm_ElectronicsVME.getBinContent(ejd);
				double etcorr = _cEtCorrRatio_ElectronicsVME.getBinContent(eid);
				int etmsm = _cEtMsm_ElectronicsVME.getBinContent(eid);
				int fgmsm = _cFGMsm_ElectronicsVME.getBinContent(eid);

				//	check and set if over threshold...
				//	HARDCODED CUTS
				double rocpd = iocpd==0 && jocpd==0?1:
					double(std::min(iocpd, jocpd))/
					double(std::max(iocpd, jocpd));
				double rocpe = iocpe==0 && jocpe==0?1:
					double(std::min(iocpe, jocpe))/
					double(std::max(iocpe, jocpe));
				double retmsm = ietmsm==0 && jetmsm==0?1:
					double(std::min(ietmsm, jetmsm))/
					double(std::max(ietmsm, jetmsm));
				double rfgmsm = ifgmsm==0 && jfgmsm==0?1:
					double(std::min(ifgmsm, jfgmsm))/
					double(std::max(ifgmsm, jfgmsm));

				//	for slot-uniformity - x5 difference...
				if (rocpd<0.2)
					ocpUniSlotData = true;
				if (rocpe<0.2)
					ocpUniSlotEmul = true;
				if (retmsm<0.2)
					etMsmUniSlot = true;
				if (rfgmsm<0.2)
					etMsmUniSlot = true;
				//	correlation ratio should be > 0.92
				if (etcorr<0.92)
					etcorrratio = true;
				//	if #etmismatches/#occupcies > 0.1 - 10%
				if (double(etmsm)/double(iocpd)>0.1)
					etmsmnum = true;
				if (double(fgmsm)/double(iocpd)>0.1)
					fgmsmnum = true;
			}
		}
		else 
		{	
			//	uTCA
			for (int is=SLOT_uTCA_MIN; is<=SLOT_uTCA_MAX; is++)
			{
				//	NOTE: Non Trigger Constructor
				eid = HcalElectronicsId(eid.crateId(), is,
					FIBER_uTCA_MIN1, FIBERCH_MIN, false);
				HcalElectronicsId ejd = HcalElectronicsId(eid.crateId(), 
					is==SLOT_uTCA_MAX?SLOT_uTCA_MIN:is+1, 
					FIBER_uTCA_MIN1, FIBERCH_MIN, false);

				//	get Contents
				int iocpd = _cOccupancyData_ElectronicsuTCA.getBinContent(eid);
				int iocpe = _cOccupancyEmul_ElectronicsuTCA.getBinContent(eid);
				int jocpd = _cOccupancyData_ElectronicsuTCA.getBinContent(ejd);
				int jocpe = _cOccupancyEmul_ElectronicsuTCA.getBinContent(ejd);
				int ietmsm = _cEtMsm_ElectronicsuTCA.getBinContent(eid);
				int jetmsm = _cEtMsm_ElectronicsuTCA.getBinContent(ejd);
				int ifgmsm = _cFGMsm_ElectronicsuTCA.getBinContent(eid);
				int jfgmsm = _cFGMsm_ElectronicsuTCA.getBinContent(ejd);
				double etcorr = _cEtCorrRatio_ElectronicsuTCA.getBinContent(
					eid);
				int etmsm = _cEtMsm_ElectronicsuTCA.getBinContent(eid);
				int fgmsm = _cFGMsm_ElectronicsuTCA.getBinContent(eid);

				//	check and set if over threshold...
				//	HARDCODED CUTS
				double rocpd = iocpd==0 && jocpd==0?1:
					double(std::min(iocpd, jocpd))/
					double(std::max(iocpd, jocpd));
				double rocpe = iocpe==0 && jocpe==0?1:
					double(std::min(iocpe, jocpe))/
					double(std::max(iocpe, jocpe));
				double retmsm = ietmsm==0 && jetmsm==0?1:
					double(std::min(ietmsm, jetmsm))/
					double(std::max(ietmsm, jetmsm));
				double rfgmsm = ifgmsm==0 && jfgmsm==0?1:
					double(std::min(ifgmsm, jfgmsm))/
					double(std::max(ifgmsm, jfgmsm));

				//	for slot-uniformity - x5 difference...
				if (rocpd<0.2)
					ocpUniSlotData = true;
				if (rocpe<0.2)
					ocpUniSlotEmul = true;
				if (retmsm<0.2)
					etMsmUniSlot = true;
				if (rfgmsm<0.2)
					fgMsmUniSlot = true;
				//	correlation ratio should be > 0.92
				if (etcorr<0.92 && 
					_cEtCorrRatio_ElectronicsuTCA.getBinEntries(eid)>0)
					etcorrratio = true;
				//	if #etmismatches/#occupcies > 0.1 - 10%
				if (double(etmsm)/double(iocpd)>0.1)
					etmsmnum = true;
				if (double(fgmsm)/double(iocpd)>0.1)
					fgmsmnum = true;
			}
		}

		ocpUniSlotData?
			_cSummary.setBinContent(eid, fOcpUniSlotData, fLow):
			_cSummary.setBinContent(eid, fOcpUniSlotData, fGood);
		ocpUniSlotEmul?
			_cSummary.setBinContent(eid, fOcpUniSlotEmul, fLow):
			_cSummary.setBinContent(eid, fOcpUniSlotEmul, fGood);
		etMsmUniSlot?
			_cSummary.setBinContent(eid, fEtMsmUniSlot, fLow):
			_cSummary.setBinContent(eid, fEtMsmUniSlot, fGood);
		fgMsmUniSlot?
			_cSummary.setBinContent(eid, fFGMsmUniSlot, fLow):
			_cSummary.setBinContent(eid, fFGMsmUniSlot, fGood);
		etcorrratio?
			_cSummary.setBinContent(eid, fEtCorrRatio, fLow):
			_cSummary.setBinContent(eid, fEtCorrRatio, fGood);
		etmsmnum?
			_cSummary.setBinContent(eid, fEtMsmNumber, fLow):
			_cSummary.setBinContent(eid, fEtMsmNumber, fGood);
		fgmsmnum?
			_cSummary.setBinContent(eid, fFGMsmNumber, fLow):
			_cSummary.setBinContent(eid, fFGMsmNumber, fGood);
	}
*/
	
	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(TPTask);

