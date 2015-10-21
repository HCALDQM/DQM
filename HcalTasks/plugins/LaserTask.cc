
#include "DQM/HcalTasks/interface/LaserTask.h"

using namespace hcaldqm;
LaserTask::LaserTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Containers
	_cOccupancy2D_depth(_name+"/Occupancy/2D", "Occupancy",
		mapper::fdepth, axis::fieta, axis::fiphi)
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void LaserTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cOccupancy2D_depth.book(ib);
}

/* virtual */ void LaserTask::_resetMonitors(int pflag)
{
	DQTask::_resetMonitors(pflag);
}

/* virtual */ void LaserTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEPDigiCollection>		chbhe;
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
		_cOccupancy2D_depth.fill(digi.id());
		_cSignals.fill(digi.id(), utilities::sumQ<HBHEDataFrame>(digi, 2.5));
		_cTiming.fill(digi.id(), utilities::aveTS<HBHEDataFrame>(digi, 2.5));
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		_cOccupancy2D_depth.fill(digi.id());
		_cSignals.fill(digi.id(), utilities::sumQ<HODataFrame>(digi, 8.5));
		_cTiming.fill(digi.id(), utilities::aveTS<HODataFrame>(digi, 8.5));
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		_cOccupancy2D_depth.fill(digi.id());
		_cSignals.fill(digi.id(), utilities::sumQ<HFDataFrame>(digi, 2.5));
		_cTiming.fill(digi.id(), utilities::aveTS<HFDataFrame>(digi, 2.5));
	}

	if (_ptype==fOnline && _evsTotal>0 &&
		_evsTotal%constants::CALIBEVENTS_MIN==0)
	{
		_cSignals.dump(&_cSignalMeans_SubDet, true);
		_cSignals.dump(&_cSignalRMSs_SubDet, false);
		_cTiming.dump(&_cTimingMeans_SubDet, true);
		_cTiming.dump(&_cTimingRMSs_SubDet, false);
	}
}

DEFINE_FWK_MODULE(LaserTask);


