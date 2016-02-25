
#include "DQM/SpecificTasks/interface/QIE10TestTask.h"

using namespace hcaldqm;
QIE10TestTask::QIE10TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	
	//	tags
	_tagQIE10 = ps.getUntrackedParameter<edm::InputTag>("tagQIE10",
		edm::InputTag("hcalDigis"));
	_tokQIE10 = consumes<QIE10DigiCollection>(_tagQIE10);
}
/* virtual */ void QIE10TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	if (_ptype==fLocal)
		if (r.runAuxiliary().run()==1)
			return;

	DQTask::bookHistograms(ib, r, es);

	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	std::vector<uint32_t> vhashC36;
	vhashC36.push_back(HcalElectronicsId(36, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_C36.initialize(filter::fPreserver, hashfunctions::fCrate,
		vhashC36);

	//	INITIALIZE what you need
	_cShape_EChannel.initialize(_name,
		"Shape", hashfunctions::fEChannel,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::ffC_10000));
	for (unsigned int j=0; j<10; j++)
	{
		_cLETDCvsADC_EChannel[j].initialize(_name,
			"LETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN));
		_cTETDCvsADC_EChannel[j].initialize(_name,
			"TETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN));
		_cADC_EChannel[j].initialize(_name,
			"ADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN));
		_cLETDC_EChannel[j].initialize(_name,
			"TDC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN));
	}

	_cShape_EChannel.book(ib, _emap, _filter_C36);
	for (unsigned int i=0; i<10; i++)
	{
		char aux[10];
		sprintf(aux, "TS%d", i);
		_cLETDCvsADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cTETDCvsADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cLETDC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
	}

	_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap, _filter_C36);
	_ehashmap.print();
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
	if (!e.getByToken(_tokQIE10, cqie10))
		std::cout << "Collection isn't availalbe" << std::endl;

	for (uint32_t i=0; i<cqie10->size(); i++)
	{
		QIE10DataFrame frame = static_cast<QIE10DataFrame>((*cqie10)[i]);
		HcalDetId did = frame.detid();
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

		for (int j=0; j<frame.samples(); j++)
		{
			_cShape_EChannel.fill(eid, j, adc2fC[frame[j].adc()]);	
			_cLETDCvsADC_EChannel[j].fill(eid, frame[j].adc(), 
				frame[j].le_tdc());
			_cTETDCvsADC_EChannel[i].fill(eid, frame[j].adc(), 
				frame[j].te_tdc());
			_cLETDC_EChannel[i].fill(eid, frame[j].le_tdc());
			_cADC_EChannel[i].fill(eid, frame[j].adc());
		}
	}
}

/* virtual */ void QIE10TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10TestTask);
