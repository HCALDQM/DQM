
#include "DQM/SpecificTasks/interface/QIE10TestTask.h"

using namespace hcaldqm;
QIE10TestTask::QIE10TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	
	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}
/* virtual */ void QIE10TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);

	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	s.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	std::vector<uint32_t> vhashC36;
	vhashC36.push_back(HcalElectronicsId(36, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN).rawId());
	_filter_C36.initialize(filter::fPreserver, hashfunctions::fCrate,
		vhashC36);

	//	INITIALIZE what you need
	_cShape_EChannel.initialize(ContainerSingle1D(_name,
		"Shape", hashfunctions::EChannel,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::ffC_10000)));
	for (unsigned int j=0; j<10; j++)
	{
		_cLETDCvsADC[j].initialize(_name,
			"LETDCvsADC", hashfunctions::EChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN));
		_cTETDCvsADC[j].initialize(_name,
			"TETDCvsADC", hashfunctions::EChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN));
		_cADC[j].initialize(_name,
			"ADC", hashfunctions::EChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN));
		_cTDC[j].initialize(_name,
			"TDC", hashfunctions::EChannel,
			new quantity::ValueQuantity(quantity::fQIE10TDC_256),
			new quantity::ValueQuantity(quantity::fN));
	}

	_vcShape.book(ib, _emap, _filter_C36);
	for (unsigned int i=0; i<10; i++)
	{
		char aux[10];
		sprintf(aux, "TS%d", j);
		_cLETDCvsADC[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cTETDCvsADC[j][i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cLETDC[j][i].book(ib, _emap, _filter_C36, _subsystem, aux);
	}

	_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap, _filter_C36);
}

/* virtual */ void QIE10TestTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{

	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE10TestTask::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<QIE10DigiCollection> cqie10;
	if (!e.getByLabel(_tagHF, cqie10))
		std::cout << "Collection isn't availalbe" << std::endl;

	for (int i=0; i<cqie10->size(); i++)
	{
		HcalDetId did = (*cqie10)[i].detid();
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

		std::cout << "#unpacked=" << cqie10->size() << std::endl;
		std::cout << did << std::endl;
		std::cout << eid << std::endl;

		for (int j=0; j<(*cqie10)[i].samples(); j++)
		{
			_cShape.fill(eid, j, adc2fC[(*cqie10)[i][j].adc()]);	
			_cLETDCvsADC[j].fill(eid, (*cqie10)[i][j].adc(), 
				(*cqie10)[i][j].le_tdc());
			_cTETDCvsADC[i].fill(eid, (*cqie10)[i][j].adc(), 
				(*cqie10)[i][j].te_tdc());
			_cLETDC[i].fill((eid, *cqie10)[i][j].le_tdc());
			_cADC[i].fill((eid, *cqie10)[i][j].adc());
		}
}

/* virtual */ void QIE10TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10TestTask);
