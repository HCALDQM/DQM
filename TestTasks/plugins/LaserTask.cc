
#include "DQM/TestTasks/interface/LaserTask.h"

using namespace hcaldqm;
LaserTask::LaserTask(edm::ParameterSet const& ps):
	DQTask(ps)
{

	//	Containers
	_cSignalMean_Subdet.initialize(_name, "SignalMean",
		hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::ffC_3000),
		new quantity::ValueQuantity(quantity::fN));
	_cSignalRMS_Subdet.initialize(_name, "SignalRMS",
		hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::ffC_1000));
	_cTimingMean_Subdet.initialize(_name, "TimingMean",
		hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingRMS_Subdet.initialize(_name, "TimingRMS",
		hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cShapeCut_SubdetPMiphi.initialize(_name, "Shape", 
		hashfunctions::fSubdetPMiphi,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::ffC_3000));
	_cSignalMean_depth.initialize(_name, "SignalMean",
		hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::ffC_3000));
	_cSignalRMS_depth.initialize(_name, "SignalRMS",
		hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::ffC_1000));
	_cTimingMean_depth.initialize(_name, "TimingMean",
		hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cTimingRMS_depth.initialize(_name, "TimingRMS",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fTiming_TS200));
	_cSignals.initialize(hashfunctions::fChannel);
	_cTiming.initialize(hashfunctions::fChannel);

	//	tags
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
	_tagTrigger = ps.getUntrackedParameter<edm::InputTag>("tagTrigger",
		edm::InputTag("tbunpacker"));
	_tokHBHE = consumes<HBHEDigiCollection>(_tagHBHE);
	_tokHO = consumes<HODigiCollection>(_tagHO);
	_tokHF = consumes<HFDigiCollection>(_tagHF);
	_tokTrigger = consumes<HcalTBTriggerData>(_tagTrigger);

	//	constants
	_lowHBHE = ps.getUntrackedParameter<double>("lowHBHE",
		20);
	_lowHO = ps.getUntrackedParameter<double>("lowHO",
		20);
	_lowHF = ps.getUntrackedParameter<double>("lowHF",
		20);
}

/* virtual */ void LaserTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	char cutstr[20];
	sprintf(cutstr, "_sumQHBHE%dHO%dHF%d", int(_lowHBHE),
		int(_lowHO), int(_lowHF));

	DQTask::bookHistograms(ib, r, es);

	edm::ESHandle<HcalDbService> dbService;
	es.get<HcalDbRecord>().get(dbService);
	_emap = dbService->getHcalMapping();

	_cSignalMean_Subdet.book(ib, _emap, _subsystem, std::string(cutstr));
	_cSignalRMS_Subdet.book(ib, _emap, _subsystem, std::string(cutstr));
	_cTimingMean_Subdet.book(ib, _emap, _subsystem, std::string(cutstr));
	_cTimingRMS_Subdet.book(ib, _emap, _subsystem, std::string(cutstr));
	_cSignalMean_depth.book(ib, _emap, _subsystem, std::string(cutstr));
	_cSignalRMS_depth.book(ib, _emap, _subsystem, std::string(cutstr));
	_cTimingMean_depth.book(ib, _emap, _subsystem, std::string(cutstr));
	_cTimingRMS_depth.book(ib, _emap, _subsystem, std::string(cutstr));
	_cShapeCut_SubdetPMiphi.book(ib, _emap, _subsystem, std::string(cutstr));
	_cSignals.book(_emap);
	_cTiming.book(_emap);
}

/* virtual */ void LaserTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void LaserTask::_dump()
{
	_cSignalMean_Subdet.reset();
	_cSignalRMS_Subdet.reset();
	_cTimingMean_Subdet.reset();
	_cTimingRMS_Subdet.reset();
	_cSignalMean_depth.reset();
	_cSignalRMS_depth.reset();
	_cTimingMean_depth.reset();
	_cTimingRMS_depth.reset();
	_cSignals.dump(&_cSignalMean_Subdet, true);
	_cSignals.dump(&_cSignalRMS_Subdet, false);
	_cTiming.dump(&_cTimingMean_Subdet, true);
	_cTiming.dump(&_cTimingRMS_Subdet, false);
	_cSignals.dump(&_cSignalMean_depth, true);
	_cSignals.dump(&_cSignalRMS_depth, false);
	_cTiming.dump(&_cTimingMean_depth, true);
	_cTiming.dump(&_cTimingRMS_depth, false);
}

/* virtual */ void LaserTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>		chbhe;
	edm::Handle<HODigiCollection>		cho;
	edm::Handle<HFDigiCollection>		chf;

	if (!e.getByToken(_tokHBHE, chbhe))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available "
			+ _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByToken(_tokHO, cho))
		_logger.dqmthrow("Collection HODigiCollection isn't available "
			+ _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByToken(_tokHF, chf))
		_logger.dqmthrow("Collection HFDigiCollection isn't available "
			+ _tagHF.label() + " " + _tagHF.instance());

	for (HBHEDigiCollection::const_iterator it=chbhe->begin();
		it!=chbhe->end(); ++it)
	{
		const HBHEDataFrame digi = (const HBHEDataFrame)(*it);
		double sumQ = utilities::sumQ<HBHEDataFrame>(digi, 2.5, 0, 
			digi.size()-1);
		if (sumQ<_lowHBHE)
			continue;

		double aveTS = utilities::aveTS<HBHEDataFrame>(digi, 2.5, 0,
			digi.size()-1);
		_cSignals.fill(digi.id(), sumQ>0 ? sumQ : GARBAGE_VALUE);
		_cTiming.fill(digi.id(), sumQ>0 ? aveTS : GARBAGE_VALUE);

		for (int i=0; i<digi.size(); i++)
			_cShapeCut_SubdetPMiphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		double sumQ = utilities::sumQ<HODataFrame>(digi, 8.5, 0, 
			digi.size()-1);
		if (sumQ<_lowHO)
			continue;

		double aveTS = utilities::aveTS<HODataFrame>(digi, 8.5, 0,
			digi.size()-1);
		_cSignals.fill(digi.id(), sumQ>0 ? sumQ : GARBAGE_VALUE);
		_cTiming.fill(digi.id(), sumQ>0 ? aveTS : GARBAGE_VALUE);

		for (int i=0; i<digi.size(); i++)
			_cShapeCut_SubdetPMiphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-8.5);
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		double sumQ = utilities::sumQ<HFDataFrame>(digi, 2.5, 0, 
			digi.size()-1);
		if (sumQ<_lowHF)
			continue;

		double aveTS = utilities::aveTS<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1);
		_cSignals.fill(digi.id(), sumQ>0 ? sumQ : GARBAGE_VALUE);
		_cTiming.fill(digi.id(), sumQ>0 ? aveTS : GARBAGE_VALUE);

		for (int i=0; i<digi.size(); i++)
			_cShapeCut_SubdetPMiphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);
	}

	if (_ptype==fOnline && _evsTotal>0 &&
		_evsTotal%constants::CALIBEVENTS_MIN==0)
		this->_dump();
}

/* virtual */ bool LaserTask::_isApplicable(edm::Event const& e)
{
	if (_ptype!=fOnline)
	{
		//	local
		edm::Handle<HcalTBTriggerData> ctrigger;
		if (!e.getByToken(_tokTrigger, ctrigger))
			_logger.dqmthrow("Collection HcalTBTriggerData isn't available "
				+ _tagTrigger.label() + " " + _tagTrigger.instance());
		return ctrigger->wasLaserTrigger();
	}

	return false;
}

DEFINE_FWK_MODULE(LaserTask);


