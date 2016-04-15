#include "DQM/HcalTasks/interface/HcalOfflineHarvesting.h"

HcalOfflineHarvesting::HcalOfflineHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps), _reportSummaryMap(NULL)
{
	_vsumgen.resize(nSummary);
	_vmarks.resize(nSummary);
	_vnames.resize(nSummary);
	_vnames[fRaw]="RawTask";
	_vnames[fDigi]="DigiTask";
	_vnames[fReco]="RecHitTask";
	_vnames[fTP]="TPTask";
	for (uint32_t i=0; i<_vmarks.size(); i++)
		_vmarks[i]=false;

	_vsumgen[fRaw]=new RawRunSummary("RawRunSummary", _vnames[fRaw],ps);
	_vsumgen[fDigi]=new DigiRunSummary("DigiRunSummary", _vnames[fDigi],ps);
	_vsumgen[fReco]=new RecoRunSummary("RecoRunSummary", _vnames[fReco],ps);
	_vsumgen[fTP]=new TPRunSummary("TPRunSummary", _vnames[fTP],ps);
}

/* virtual */ void HcalOfflineHarvesting::beginRun(
	edm::Run const& r, edm::EventSetup const& es)
{
	DQHarvester::beginRun(r,es);

	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
		(*it)->beginRun(r,es);
}

//
//	For OFFLINE there is no per LS evaluation
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndLuminosityBlock(
	DQMStore::IBooker& ib,
	DQMStore::IGetter& ig, edm::LuminosityBlock const& lb, 
	edm::EventSetup const& es)
{
	int ii=0;
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
	{	
		//	run only if have to
		if (_vmarks[ii])
			(*it)->endLuminosityBlock(ib,ig,lb,es);
		ii++;
	}

	if (!me)
	{
		ib.setCurrentFolder("Hcal/Folder1");
		me = ib.book1D("ME", "ME", 10, 0, 10);
		me->setLumiFlag();
	}
}

//
//	Evaluate and Generate Run Summary
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndJob(DQMStore::IBooker& ib,
	DQMStore::IGetter& ig)
{
	//	OBTAIN/SET WHICH MODULES ARE PRESENT
	int num=0; std::map<std::string, int> datatiers;
	if (ig.get(_subsystem+"/"+_vnames[fRaw]+"/EventsTotal")!=NULL)
	{
		datatiers.insert(std::pair<std::string, int>("RAW",num));
		_vmarks[fRaw]=true;
		num++;
	}
	if (ig.get(_subsystem+"/"+_vnames[fDigi]+"/EventsTotal")!=NULL)
	{
		_vmarks[fDigi]=true;
		datatiers.insert(std::pair<std::string, int>("DIGI",num));
		num++;
	}
	if (ig.get(_subsystem+"/"+_vnames[fTP]+"/EventsTotal")!=NULL)
	{
		_vmarks[fTP]=true;
		datatiers.insert(std::pair<std::string, int>("TP",num));
		num++;
	}
	if (ig.get(_subsystem+"/"+_vnames[fReco]+"/EventsTotal")!=NULL)
	{
		datatiers.insert(std::pair<std::string, int>("RECO",num));
		_vmarks[fReco]=true;
		num++;
	}
	
	//	CREATE THE REPORT SUMMARY MAP
	//	num is #modules
	//	datatiers - std map [DATATIER_NAME] -> [positional value [0,num-1]]
	//	-> bin wise +1 should be
	if (!_reportSummaryMap)
	{
		ib.setCurrentFolder(_subsystem+"/EventInfo");
		_reportSummaryMap = ib.book2D("reportSummaryMap", "reportSummaryMap",
			_vFEDs.size(), 0, _vFEDs.size(), num,0,num);
		//	x axis labels
		
		for (uint32_t i=0; i<_vFEDs.size(); i++)
		{
			char name[5];
			sprintf(name, "%d", _vFEDs[i]);
			_reportSummaryMap->setBinLabel(i+1, name, 1);
		}
		//	y axis lables
		for (std::map<std::string, int>::const_iterator
			it=datatiers.begin(); it!=datatiers.end(); ++it)
		{
			std::string name = it->first;
			int value = it->second;
			_reportSummaryMap->setBinLabel(value+1, name, 2);
		}
	}

	//	iterate over all summary generators and get the flags
	int ii=0;
	for (std::vector<DQClient*>::const_iterator it=_vsumgen.begin();
		it!=_vsumgen.end(); ++it)
	{
		//	IF MODULE IS NOT PRESENT IN DATA SKIP
		if (!_vmarks[ii])
		{ii++;continue;}

		//	OBTAIN ALL THE FLAGS FOR THIS MODULE
		//	AND SET THE REPORT STATUS MAP
		//	NOTE AGAIN: datatiers map [DATATIER]->[value not bin!]+1 therefore
		std::vector<flag::Flag> flags = (*it)->endJob(ib,ig);
		for (uint32_t ifed=0; ifed<_vFEDs.size(); ifed++)
		{
			_reportSummaryMap->setBinContent(ifed+1, 
				datatiers[flags[ifed]._name]+1, (int)flags[ifed]._state);
			std::cout << "FED=" << _vFEDs[ifed] << std::endl;
			std::cout << flags[ifed]._name << "  " << flags[ifed]._state
				<<std::endl;
		}
		ii++;
	}
}

DEFINE_FWK_MODULE(HcalOfflineHarvesting);