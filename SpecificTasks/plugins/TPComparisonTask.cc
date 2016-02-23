
#include "DQM/SpecificTasks/interface/TPComparisonTask.h"

using namespace hcaldqm;
TPComparisonTask::TPComparisonTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags and tokens
	_tag1 = ps.getUntrackedParameter<edm::InputTag>("tag1",
		edm::InputTag("hcalDigis"));
	_tag2 = ps.getUntrackedParameter<edm::InputTag>("tag2",
		edm::InputTag("utcaDigis"));
	_tok1 = consumes<HcalTrigPrimDigiCollection>(_tag1);
	_tok2 = consumes<HcalTrigPrimDigiCollection>(_tag2);

	//	tmp flags
	_skip1x1 = ps.getUntrackedParameter<bool>("skip1x1", true);
}

/* virtual */ void TPComparisonTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	
	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	std::vector<int> vFEDs = utilities::getFEDList(_emap);
	std::vector<int> vFEDsVME = utilities::getFEDVMEList(_emap);
	std::vector<int> vFEDsuTCA = utilities::getFEDuTCAList(_emap);
	std::vector<uint32_t> vhashVME;
	std::vector<uint32_t> vhashuTCA;
	vhashVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
		constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
	vhashuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
		vhashVME);
	_filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
		vhashuTCA);

	//	INTIALIZE CONTAINERS
	char aux[20];
	for (unsigned int i=0; i<4; i++)
	{
		sprintf(aux, "_TS%d", i);
		_cEt_TTSubdet[i].initialize(_name, "Et",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fEt_128),
			new quantity::ValueQuantity(quantity::fEt_128),
			new quantity::ValueQuantity(quantity::fN));
		_cFG_TTSubdet[i].initialize(_name, "FG",
			hashfunctions::fTTSubdet,
			new quantity::ValueQuantity(quantity::fFG),
			new quantity::ValueQuantity(quantity::fFG),
			new quantity::ValueQuantity(quantity::fN));
	}
	_cMsn_FEDVME.initialize(_name, "Missing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fSLBSLBCh),
		new quantity::ValueQuantity(quantity::fN));
	_cMsn_FEDuTCA.initialize(_name, "Missing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCATPFiberChuTCATP),
		new quantity::ValueQuantity(quantity::fN));
	_cEtMsm_FEDVME.initialize(_name, "EtMsm",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fSLBSLBCh),
		new quantity::ValueQuantity(quantity::fN));
	_cEtMsm_FEDuTCA.initialize(_name, "EtMsm",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCATPFiberChuTCATP),
		new quantity::ValueQuantity(quantity::fN));
	_cFGMsm_FEDVME.initialize(_name, "FGMsm",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fSLBSLBCh),
		new quantity::ValueQuantity(quantity::fN));
	_cFGMsm_FEDuTCA.initialize(_name, "FGMsm",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCATPFiberChuTCATP),
		new quantity::ValueQuantity(quantity::fN));

	for (unsigned int i=0; i<4; i++)
	{
		_cEt_TTSubdet[i].book(ib, _emap);
		_cFG_TTSubdet[i].book(ib, _emap);
	}
	_cMsn_FEDVME.book(ib, _emap, _filter_uTCA);
	_cEtMsm_FEDVME.book(ib, _emap, _filter_uTCA);
	_cFGMsm_FEDVME.book(ib, _emap, _filter_uTCA);
	_cMsn_FEDuTCA.book(ib, _emap, _filter_VME);
	_cEtMsm_FEDuTCA.book(ib, _emap, _filter_VME);
	_cFGMsm_FEDuTCA.book(ib, _emap, _filter_VME);

//	_cMsn_depth.book(ib);
//	_cEtMsm_depth.book(ib);
//	_cFGMsm_depth.book(ib);
}

/* virtual */ void TPComparisonTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void TPComparisonTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HcalTrigPrimDigiCollection>	coll1;
	edm::Handle<HcalTrigPrimDigiCollection>	coll2;

	if (!e.getByToken(_tok1, coll1))
		_logger.dqmthrow(
			"Collection HcalTrigPrimDigiCollection isn't available" + 
			_tag1.label() + " " + _tag1.instance());
	if (!e.getByToken(_tok2, coll2))
		_logger.dqmthrow(
			"Collection HcalTrigPrimDigiCollection isn't available" + 
			_tag2.label() + " " + _tag2.instance());
/*
	for (HcalTrigPrimDigiCollection::const_iterator it1=coll1->begin();
		it1!=coll1->end(); ++it1)
	{
		//	tmp
		if (_skip1x1)
			if (it1->id().depth()==10)
				continue;
		//	\tmp

		HcalTrigTowerDetId tid = it1->id();
		HcalTrigPrimDigiCollection::const_iterator it2=coll2->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), 0));

		if (it2==coll2->end())
			_cMsn_depth.fill(tid);
		else
			for (int i=0; i<it1->size(); i++)
			{
				_cEt_TPSubDet[i].fill(tid, 
					it1->sample(i).compressedEt(), 
					it2->sample(i).compressedEt());
				_cFG_TPSubDet[i].fill(tid,
					it1->sample(i).fineGrain(),
					it2->sample(i).fineGrain());
				if (it1->sample(i).compressedEt()!=
					it2->sample(i).compressedEt())
					_cEtMsm_depth.fill(tid);
				if (it1->sample(i).fineGrain()!=
					it2->sample(i).fineGrain())
					_cFGMsm_depth.fill(tid);
			}
	}
	*/
}

/* virtual */ void TPComparisonTask::endLuminosityBlock(
	edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	//	in the end always
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(TPComparisonTask);

