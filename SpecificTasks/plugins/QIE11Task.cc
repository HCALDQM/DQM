
#include "DQM/SpecificTasks/interface/QIE11Task.h"

using namespace hcaldqm;
QIE11Task::QIE11Task(edm::ParameterSet const& ps):
	DQTask(ps)
{
	
	//	tags
	_tagQIE11 = ps.getUntrackedParameter<edm::InputTag>("tagQIE11",
		edm::InputTag("hcalDigis"));
	_tokQIE11 = consumes<QIE11DigiCollection>(_tagQIE11);

	//	cuts
	_cut = ps.getUntrackedParameter<double>("cut", 50.0);
	_ped = ps.getUntrackedParameter<int>("ped", 4);
}
/* virtual */ void QIE11Task::bookHistograms(DQMStore::IBooker &ib,
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
	vhashC36.push_back(HcalElectronicsId(36, 4,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_C36.initialize(filter::fPreserver, hashfunctions::fCrateSlot,
		vhashC36);

	//	INITIALIZE what you need
	_cShapeCut_EChannel.initialize(_name,
		"ShapeCut", hashfunctions::fEChannel,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fQIE11fC_10000));
	_cShapeCut.initialize(_name,
		"ShapeCut", 
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fQIE11fC_10000));
	_cLETDCvsADC.initialize(_name, "LETDCvsADC",
		new quantity::ValueQuantity(quantity::fQIE11ADC_256),
		new quantity::ValueQuantity(quantity::fQIE11TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTETDCvsADC.initialize(_name, "TETDCvsADC",
		new quantity::ValueQuantity(quantity::fQIE11ADC_256),
		new quantity::ValueQuantity(quantity::fQIE11TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cLETDC.initialize(_name, "LETDC",
		new quantity::ValueQuantity(quantity::fQIE11TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cADC.initialize(_name, "ADC",
		new quantity::ValueQuantity(quantity::fQIE11ADC_256),
		new quantity::ValueQuantity(quantity::fN, true))
	for (unsigned int j=0; j<10; j++)
	{
		_cLETDCvsADC_EChannel[j].initialize(_name,
			"LETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE11ADC_256),
			new quantity::ValueQuantity(quantity::fQIE11TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
		_cTETDCvsADC_EChannel[j].initialize(_name,
			"TETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE11ADC_256),
			new quantity::ValueQuantity(quantity::fQIE11TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
		_cADC_EChannel[j].initialize(_name,
			"ADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE11ADC_256),
			new quantity::ValueQuantity(quantity::fN, true));
		_cLETDC_EChannel[j].initialize(_name,
			"TDC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE11TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
	}

	_cShapeCut_EChannel.book(ib, _emap, _filter_C36, subsystem);
	_cShapeCut.book(ib, _subsystem);
	_cLETDCvsADC.book(ib, _subsystem);
	_cTETDCvsADC.book(ib, _subsystem);
	_cLETDC.book(ib, _subsystem);
	_cADC.book(ib, _subsystem);
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
}

/* virtual */ void QIE11Task::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE11Task::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<QIE11DigiCollection> cqie10;
	if (!e.getByToken(_tokQIE11, cqie10))
		std::cout << "Collection isn't available" << std::endl;

	for (uint32_t i=0; i<cqie10->size(); i++)
	{
		QIE11DataFrame frame = static_cast<QIE11DataFrame>((*cqie10)[i]);
		DetId did = frame.detid();
		std::cout << did << std::endl;
//		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

		//	compute the signal, ped subracted
//		double q = utilities::aveTS_v10<QIE11DataFrame>(frame,
//			constants::adc2fC[_ped], 0, frame.samples()-1);

		/*
		//	iterate thru all TS and fill
		for (int j=0; j<frame.samples(); j++)
		{
			//	shapes are after the cut
			if (q>_cut)
			{
				_cShape_EChannel.fill(eid, j, adc2fC[frame[j].adc()]);	
				_cShape.fill(eid, j, adc2fC[frame[j].adc()]);
			}

			//	w/o a cut
			_cLETDCvsADC_EChannel[j].fill(eid, frame[j].adc(), 
				frame[j].le_tdc());
			_cLETDCvsADC.fill(eid, frame[j].adc(), frame[j].le_tdc());
			_cTETDCvsADC_EChannel[j].fill(eid, frame[j].adc(), 
				frame[j].te_tdc());
			_cTETDCvsADC.fill(eid, frame[j].adc(), frame[j].te_tdc());
			_cLETDC_EChannel[j].fill(eid, frame[j].le_tdc());
			_cLETDC.fill(eid, frame[j].le_tdc());
			_cADC_EChannel[j].fill(eid, frame[j].adc());
			_cADC.fill(eid, frame[j].adc());

		}
		*/
	}
}

/* virtual */ void QIE11Task::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE11Task);
