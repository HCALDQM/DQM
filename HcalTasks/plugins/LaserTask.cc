
#include "DQM/HcalTasks/interface/LaserTask.h"

using namespace hcaldqm;
LaserTask::LaserTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Containers
	_cSignalMeans1D_SubDet(_name+"/1D/Signal", "SignalMeans",
		mapper::fSubDet, axis::fNomFC_3000, axis::fEntries),
	_cSignalRMSs1D_SubDet(_name+"/1D/Signal", "SignalRMSs",
		mapper::fSubDet, axis::fNomFC_3000, axis::fEntries),
	_cTimingMeans1D_SubDet(_name+"/1D/Timing", "TimingMeans",
		mapper::fSubDet, axis::fTimeTS, axis::fEntries),
	_cTimingRMSs1D_SubDet(_name+"/1D/Timing", "TimingRMSs",
		mapper::fSubDet, axis::fTimeTS, axis::fEntries),
	_cSignalMeans2D_depth(_name+"/2D/Signal", "SignalMeans",
		mapper::fdepth, axis::fieta, axis::fiphi),
	_cSignalRMSs2D_depth(_name+"/2D/Signal", "SignalRMSs",
		mapper::fdepth, axis::fieta, axis::fiphi),
	_cTimingMeans2D_depth(_name+"/2D/Timing", "TimingMeans",
		mapper::fdepth, axis::fieta, axis::fiphi),
	_cTimingRMSs2D_depth(_name+"/2D/Timing", "TimingRMSs",
		mapper::fdepth, axis::fieta, axis::fiphi)
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

/* virtual */ void LaserTask::bookHistograms(DQMStore::IBooker &ib,
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
}

/* virtual */ void LaserTask::_resetMonitors(int pflag)
{
	DQTask::_resetMonitors(pflag);
}

/* virtual */ void LaserTask::_dump()
{
	_cSignals.dump(&_cSignalMeans1D_SubDet, true);
	_cSignals.dump(&_cSignalRMSs1D_SubDet, false);
	_cTiming.dump(&_cTimingMeans1D_SubDet, true);
	_cTiming.dump(&_cTimingRMSs1D_SubDet, false);
	_cSignals.dump(&_cSignalMeans2D_depth, true);
	_cSignals.dump(&_cSignalRMSs2D_depth, false);
	_cTiming.dump(&_cTimingMeans2D_depth, true);
	_cTiming.dump(&_cTimingRMSs2D_depth, false);
}

/* virtual */ void LaserTask::_process(edm::Event const& e,
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
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		_cSignals.fill(digi.id(), utilities::sumQ<HODataFrame>(digi, 8.5, 0, 
			digi.size()-1));
		_cTiming.fill(digi.id(), utilities::aveTS<HODataFrame>(digi, 8.5, 0,
			digi.size()-1));
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		_cSignals.fill(digi.id(), utilities::sumQ<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1));
		_cTiming.fill(digi.id(), utilities::aveTS<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1));
	}

	if (_ptype==fOnline && _evsTotal>0 &&
		_evsTotal%constants::CALIBEVENTS_MIN==0)
		this->_dump();
}

/* virtual */ bool LaserTask::_isApplicable(edm::Event const& e)
{
	if (_ptype==fOnline)
	{
		//	online-global
		int calibType =	this->_getCalibType(e);
		return (calibType==hc_HBHEHPD || calibType==hc_HFPMT);
	}
	else
	{
		//	local
		edm::Handle<HcalTBTriggerData> ctrigger;
		if (!e.getByLabel(_tagTrigger, ctrigger))
			this->_throw("Collection HcalTBTriggerData isn't available",
				" " + _tagTrigger.label() + " " + _tagTrigger.instance());
		return ctrigger->wasLaserTrigger();
	}
}

DEFINE_FWK_MODULE(LaserTask);


