
#include "DQM/SpecificTasks/interface/DigiComparisonTask.h"

using namespace hcaldqm;
DigiComparisonTask::DigiComparisonTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags and tokens
	_tagHBHE1 = ps.getUntrackedParameter<edm::InputTag>("tagHBHE1",
		edm::InputTag("hcalDigis"));
	_tagHBHE2 = ps.getUntrackedParameter<edm::InputTag>("tagHBHE2",
		edm::InputTag("utcaDigis"));
	_tokHBHE1 = consumes<HBHEDigiCollection>(_tagHBHE1);
	_tokHBHE2 = consumes<HBHEDigiCollection>(_tagHBHE2);
}
/* virtual */ void DigiComparisonTask::bookHistograms(DQMStore::IBooker &ib,
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
	
	//	INITIALIZE
	char aux[20];
	for  (unsigned int i=0; i<10; i++)
	{
		sprintf(aux, "_TS%d", i);
		_cADC_Subdet[i].initialize(_name, "ADC",
			hashfunctions::fSubdet,
			new quantity::ValueQuantity(quantity::fADC_128),
			new quantity::ValueQuantity(quantity::fADC_128));
	}
	_cMsm_depth.initialize(_name, "Mismatched",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cMsm_FEDVME.initialize(_name, "Mismatched",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cMsm_FEDuTCA.initialize(_name, "Mismatched",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cMsn_depth.initialize(_name, "Missing",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cMsn_FEDVME.initialize(_name, "Missing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSpigot),
		new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
		new quantity::ValueQuantity(quantity::fN));
	_cMsn_FEDuTCA.initialize(_name, "Missing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fN));

	//	BOOK
	for (unsigned int i=0; i<10; i++)
		_cADC_Subdet[i].book(ib, _emap);
	_cMsm_depth.book(ib, _emap);
	_cMsn_depth.book(ib, _emap);
	_cMsm_FEDVME.book(ib, _emap, _filter_uTCA);
	_cMsn_FEDVME.book(ib, _emap, _filter_uTCA);
	_cMsm_FEDuTCA.book(ib, _emap, _filter_VME);
	_cMsn_FEDuTCA.book(ib, _emap, _filter_VME);
}

/* virtual */ void DigiComparisonTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void DigiComparisonTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>	chbhe1;
	edm::Handle<HBHEDigiCollection>	chbhe2;
	
	if (!e.getByToken(_tokHBHE1, chbhe1))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE1.label() + " " + _tagHBHE1.instance());
	if (!e.getByToken(_tokHBHE2, chbhe2))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE2.label() + " " + _tagHBHE2.instance());

	for (HBHEDigiCollection::const_iterator it1=chbhe1->begin();
		it1!=chbhe1->end(); ++it1)
	{
		HcalDetId did = it1->id();
		HBHEDigiCollection::const_iterator it2 = chbhe2->find(did);
		if (it2==chbhe2->end())
		{
			_cMsn_depth.fill(did);
			HcalElectronicsId eid = it1->elecId();
			if (eid.isVMEid())
				_cMsn_FEDVME.fill(eid);
			else
				_cMsn_FEDuTCA.fill(eid);
		}
		else
			for (int i=0; i<it1->size(); i++)
			{
				_cADC_Subdet[i].fill(did, double(it1->sample(i).adc()),
					double(it2->sample(i).adc()));
				if (it1->sample(i).adc()!=it2->sample(i).adc())
				{
					_cMsm_depth.fill(did);
					HcalElectronicsId eid = it1->elecId();
					if (eid.isVMEid())
						_cMsm_FEDVME.fill(eid);
					else
						_cMsm_FEDuTCA.fill(eid);
				}
			}
	}
}

/* virtual */ void DigiComparisonTask::endLuminosityBlock(
	edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	//	in the end always
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(DigiComparisonTask);

