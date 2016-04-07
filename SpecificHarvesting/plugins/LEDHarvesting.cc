#include "DQM/SpecificHarvesting/interface/LEDHarvesting.h"

LEDHarvesting::LEDHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
	_vruns = ps.getUntrackedParameter<std::vector<int> >("runs");
}

void LEDHarvesting::book(DQMStore *store)
{
	_cTimingMean_SubdetPM.initialize(_name, "TimingMean", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN, true));
	_cSignalRatio_SubdetPM.initialize(_name, "SignalRatio", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fRatio_0to2),
		new quantity::ValueQuantity(quantity::fN, true));
	_cSignalMean_depth.initialize("LEDTask", "SignalMean",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::ffC_3000));
	_cTimingMean_depth.initialize("LEDTask", "TimingMean",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fTiming_TS200));

	//	initialize vectors of containers
	_vcTimingMean_depth.resize(_vfiles.size());
	_vcSignalMean_depth.resize(_vfiles.size());
	_vcSignalRatio_SubdetPM.resize(_vfiles.size());
	_vcSignalRatio_depth.resize(_vfiles.size());
	for (uint32_t i=0; i<_vfiles.size(); i++)
	{
		//	init containers to load
		_vcTimingMean_depth[i].initialize("LEDTask",
			"TimingMean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fTiming_TS200));
		_vcSignalMean_depth[i].initialize("LEDTask",
			"SignalMean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::ffC_3000));
		
		//	 init new containers
		_vcSignalRatio_SubdetPM[i].initialize(_name, "SignalRatio",
			hashfunctions::fSubdetPM,
			new quantity::ValueQuantity(quantity::fRatio_0to2),
			new quantity::ValueQuantity(quantity::fN, true));
		_vcSignalRatio_depth[i].initialize(_name, "SignalRatio",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fRatio_0to2));
	}

	_cTimingMean_SubdetPM.book(store, _emap);
	_cSignalRatio_SubdetPM.book(store, _emap);
	char runname[10];
	for (uint32_t i=0; i<_vfiles.size(); i++)
	{
		sprintf(runname, "vs%d", _vruns[i]);
		_vcSignalRatio_SubdetPM[i].book(store, _emap,
			_subsystem, runname);
		_vcSignalRatio_depth[i].book(store, _emap,
			_subsystem, runname);
	}
}

/* virtual */ void LEDHarvesting::endRun(edm::Run const& r, 
	edm::EventSetup const& es)
{
	_runnum = r.runAuxiliary().run();
	if (_runnum==1)
		return;
	_vruns.push_back(_runnum);

	//	get the emap
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	_emaphash.initialize(_emap, hcaldqm::electronicsmap::fD2EHashMap);

	//	get the DQMStore and book
	DQMStore *store = edm::Service<DQMStore>().operator->();
	this->book(store);
	store->showDirStructure();

	//	load the current run histograms
	_cSignalMean_depth.load(store, _emap);
	_cTimingMean_depth.load(store, _emap);

	//	load all the files and process
	char runnumber[20];
	int irun=0;
	for (std::vector<std::string>::const_iterator it=_vfiles.begin();
		it!=_vfiles.end(); ++it)
	{
		store->open(*it, false, "", "", DQMStore::KeepRunDirs);
		sprintf(runnumber,"Run %d", _vruns[irun]);
		//	fill containers with data
		_vcTimingMean_depth[irun].load(store, _emap, _subsystem, "",
			std::string(runnumber), DQMStore::KeepRunDirs);
		_vcSignalMean_depth[irun].load(store, _emap, _subsystem, "",
			std::string(runnumber), DQMStore::KeepRunDirs);

		std::vector<HcalGenericDetId> dids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=dids.begin();
			it!=dids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			double t = _vcTimingMean_depth[irun].getBinContent(did);
			double scur = _cSignalMean_depth.getBinContent(did);
			double tcur = _cTimingMean_depth.getBinContent(did);
			double s = _vcSignalMean_depth[irun].getBinContent(did);
			double sref = _vcSignalMean_depth[0].getBinContent(did);
			double entcur = _cSignalMean_depth.getBinEntries(did);
			double entref = _vcSignalMean_depth[0].getBinEntries(did);
			double ent = _vcSignalMean_depth[irun].getBinEntries(did);
			if (entref==0 || ent==0 || entcur==0)
				continue;

			_cTimingMean_SubdetPM.fill(did, _vruns[irun], t);
			_cSignalRatio_SubdetPM.fill(did, _vruns[irun], s/sref);
			if (irun==0)
			{
				_cTimingMean_SubdetPM.fill(did, _vruns[_vruns.size()-1],
					tcur);
				_cSignalRatio_SubdetPM.fill(did, _vruns[_vruns.size()-1],
					scur/sref);
			}

			_vcSignalRatio_SubdetPM[irun].fill(did, scur/s);
			_vcSignalRatio_depth[irun].fill(did, scur/s);
		}

		//	delet the folder you just loaded
		if (irun>0)
			store->rmdir(runnumber);
	
		//	inc
		irun++;
	}
	//	delete the reference guy
	sprintf(runnumber, "Run %d", _vruns[0]);
	store->rmdir(runnumber);
	store->showDirStructure();

}

DEFINE_FWK_MODULE(LEDHarvesting);
