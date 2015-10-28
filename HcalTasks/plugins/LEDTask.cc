
#include "DQM/HcalTasks/interface/LEDTask.h"

using namespace hcaldqm;
LEDTask::LEDTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Containers
	_cSignalMeans1D_SubDet(_name+"/1D/Signal", "SignalMeans",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC_3000)),
	_cSignalRMSs1D_SubDet(_name+"/1D/Signal", "SignalRMSs",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC_1000)),
	_cTimingMeans1D_SubDet(_name+"/1D/Timing", "TimingMeans",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS_200)),
	_cTimingRMSs1D_SubDet(_name+"/1D/Timing", "TimingRMSs",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS_200)),
	_cShape_SubDet_iphi(_name+"/Shapes", "Shape", mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC_3000)),
	_cSignalMeans2D_depth(_name+"/2D/Signal", "SignalMeans",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cSignalRMSs2D_depth(_name+"/2D/Signal", "SignalRMSs",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cTimingMeans2D_depth(_name+"/2D/Timing", "TimingMeans",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cTimingRMSs2D_depth(_name+"/2D/Timing", "TimingRMSs",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi))
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
	_tagTrigger = ps.getUntrackedParameter<edm::InputTag>("tagTrigger",
		edm::InputTag("tbunpacker"));
}

/* virtual */ void LEDTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cSignalMeans1D_SubDet.book(ib, _subsystem);
	_cSignalRMSs1D_SubDet.book(ib, _subsystem);
	_cTimingMeans1D_SubDet.book(ib, _subsystem);
	_cTimingRMSs1D_SubDet.book(ib, _subsystem);
	_cSignalMeans2D_depth.book(ib, _subsystem);
	_cSignalRMSs2D_depth.book(ib, _subsystem);
	_cTimingMeans2D_depth.book(ib, _subsystem);
	_cTimingRMSs2D_depth.book(ib, _subsystem);
	_cShape_SubDet_iphi.book(ib, _subsystem);
}

/* virtual */ void LEDTask::_resetMonitors(int pflag)
{
	DQTask::_resetMonitors(pflag);
}

/* virtual */ void LEDTask::_dump()
{
	_cSignalMeans1D_SubDet.reset();
	_cSignalRMSs1D_SubDet.reset();
	_cTimingMeans1D_SubDet.reset();
	_cTimingRMSs1D_SubDet.reset();
	_cSignalMeans2D_depth.reset();
	_cSignalRMSs2D_depth.reset();
	_cTimingMeans2D_depth.reset();
	_cTimingRMSs2D_depth.reset();
	_cSignals.dump(&_cSignalMeans1D_SubDet, true);
	_cSignals.dump(&_cSignalRMSs1D_SubDet, false);
	_cTiming.dump(&_cTimingMeans1D_SubDet, true);
	_cTiming.dump(&_cTimingRMSs1D_SubDet, false);
	_cSignals.dump(&_cSignalMeans2D_depth, true);
	_cSignals.dump(&_cSignalRMSs2D_depth, false);
	_cTiming.dump(&_cTimingMeans2D_depth, true);
	_cTiming.dump(&_cTimingRMSs2D_depth, false);
}

/* virtual */ void LEDTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>		chbhe;
	edm::Handle<HODigiCollection>		cho;
	edm::Handle<HFDigiCollection>		chf;

	if (!e.getByLabel(_tagHBHE, chbhe))
		this->_throw("Collection HBHEDigiCollection isn't available",
			" " + _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByLabel(_tagHO, cho))
		this->_throw("Collection HODigiCollection isn't available",
			" " + _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByLabel(_tagHF, chf))
		this->_throw("Collection HFDigiCollection isn't available",
			" " + _tagHF.label() + " " + _tagHF.instance());

	for (HBHEDigiCollection::const_iterator it=chbhe->begin();
		it!=chbhe->end(); ++it)
	{
		const HBHEDataFrame digi = (const HBHEDataFrame)(*it);
		_cSignals.fill(digi.id(), utilities::sumQ<HBHEDataFrame>(digi, 2.5, 0,
			digi.size()-1));
		_cTiming.fill(digi.id(), utilities::aveTS<HBHEDataFrame>(digi, 2.5, 0,
			digi.size()-1));

		for (int i=0; i<digi.size(); i++)
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		_cSignals.fill(digi.id(), utilities::sumQ<HODataFrame>(digi, 8.5, 0,
			digi.size()-1));
		_cTiming.fill(digi.id(), utilities::aveTS<HODataFrame>(digi, 8.5, 0,
			digi.size()-1));

		for (int i=0; i<digi.size(); i++)
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		_cSignals.fill(digi.id(), utilities::sumQ<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1));
		_cTiming.fill(digi.id(), utilities::aveTS<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1));

		for (int i=0; i<digi.size(); i++)
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);
	}

	if (_ptype==fOnline && _evsTotal>0 &&
		_evsTotal%constants::CALIBEVENTS_MIN==0)
		this->_dump();
}

/* virtual */ bool LEDTask::_isApplicable(edm::Event const& e)
{
	if (_ptype!=fOnline)
	{
		//	local
		edm::Handle<HcalTBTriggerData> ctrigger;
		if (!e.getByLabel(_tagTrigger, ctrigger))
			this->_throw("Collection HcalTBTriggerData isn't available",
				" " + _tagTrigger.label() + " " + _tagTrigger.instance());
		return ctrigger->wasLEDTrigger();
	}

	return false;
}

DEFINE_FWK_MODULE(LEDTask);

