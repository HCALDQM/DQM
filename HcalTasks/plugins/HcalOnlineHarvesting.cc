#include "DQM/HcalTasks/interface/HcalOnlineHarvesting.h"

HcalOnlineHarvesting::HcalOnlineHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps), _reportSummaryMap(NULL)
{
	_vsumgen.push_back(new RawRunSummary("RawRunSummary", ps));
	_vsumgen.push_back(new DigiRunSummary("DigiRunSummary", ps));
	_vsumgen.push_back(new RecoRunSummary("RecoRunSummary", ps));
	_vsumgen.push_back(new TPRunSummary("TPRunSummary", ps));
	_vnames.push_back("RawTask");
	_vnames.push_back("DigiTask");
	_vnames.push_back("RecHitTask");
	_vnames.push_back("TPTask");

	for (uint32_t i=0; i<_vsumgen.size(); i++)
		_vmarks.push_back(false);
}

/* virtual */ void HcalOnlineHarvesting::beginRun(
	edm::Run const& r, edm::EventSetup const& es)
{
	DQHarvester::beginRun(r,es);
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
		(*it)->beginRun(r,es);
}

/* virtual */ void HcalOnlineHarvesting::_dqmEndLuminosityBlock(
	DQMStore::IBooker& ib,
	DQMStore::IGetter& ig, edm::LuminosityBlock const&, 
	edm::EventSetup const&)
{
	//	DETERMINE WHICH MODULES ARE PRESENT IN DATA
	if (ig.get(_subsystem+"/RawTask/EventsTotal")!=NULL)
		_vmarks[fRaw]=true;
	if (ig.get(_subsystem+"/DigiTask/EventsTotal")!=NULL)
		_vmarks[fDigi]=true;
	if (ig.get(_subsystem+"/TPTask/EventsTotal")!=NULL)
		_vmarks[fTP]=true;
	if (ig.get(_subsystem+"/RecHitTask/EventsTotal")!=NULL)
		_vmarks[fReco]=true;

	//	CREATE SUMMARY REPORT MAP FED vs LS and LOAD MODULE'S SUMMARIES
	//	NOTE: THIS STATEMENTS WILL BE EXECUTED ONLY ONCE!
	if (!_reportSummaryMap)
	{
		ig.setCurrentFolder(_subsystem+"/EventInfo");
		_reportSummaryMap = ib.book2D("reportSummaryMap", "reportSummaryMap",
			_maxLS, 0, _maxLS, _vFEDs.size(), 0, _vFEDs.size());
		for (uint32_t i=0; i<_vFEDs.size(); i++)
		{
			char name[5];
			sprintf(name, "%d", _vFEDs[i]);
			_reportSummaryMap->setBinLabel(i+1, name, 2);
		}

		// INITIALIZE ALL THE MODULES
		for (uint32_t i=0; i<_vnames.size(); i++)
			_vcSummary.push_back(ContainerSingle2D(_vnames[i],
				"SummaryvsLS",
				new quantity::LumiSection(_maxLS),
				new quantity::FEDQuantity(_vFEDs),
				new quantity::FlagQuantity()));

		//	LOAD ONLY THOSE MODULES THAT ARE PRESENT IN DATA
		for (uint32_t i=0; i<_vmarks.size(); i++)
		{
			if (_vmarks[i])
				_vcSummary[i].load(ig, _subsystem);
		}
	}

	int ifed=0;
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		flag::Flag fSum("Status");
		for (uint32_t im=0; im<_vmarks.size(); im++)
			if (_vmarks[im])
				fSum+=flag::Flag("Status", 
					(int)_vcSummary[im].getBinContent(eid,_currentLS))
		_reportSummaryMap->setBinContent(_currentLS, fSum._state);
	}
}

/* virtual */ void HcalOnlineHarvesting::_dqmEndJob(DQMStore::IBooker&,
	DQMStore::IGetter&)
{
	//	iterate over Run Summary Clients and generate Run Summary
	int ii=0;
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
	{
		if (!_vmarks[ii])
		{ii++;continue;}
		(*it)->endJob(ib,ig);
	}
}

DEFINE_FWK_MODULE(HcalOnlineHarvesting);
