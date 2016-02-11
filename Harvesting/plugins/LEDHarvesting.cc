#include "DQM/Harvesting/interface/LEDHarvesting.h"

LEDHarvesting::LEDHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
	_vruns = ps.getUntrackedParameter<std::vector<int> >("runs");

	_cTimingMean_SubdetPM.initialize("LEDTrend", "TimingMean", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN, true));
	_cSignalRatio_SubdetPM.initialize("LEDTrend", "SignalRatio", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fRatio_0to2),
		new quantity::ValueQuantity(quantity::fN, true));

	//	initialize vectors of containers
	_vcTimingMean_depth.resize(_vruns.size());
	_vcSignalMean_depth.resize(_vruns.size());
	_vcSignalRatio_SubdetPM.resize(_vruns.size());
	_vcSignalRatio_depth.resize(_vruns.size());
	for (uint32_t i=0; i<_vruns.size(); i++)
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
		_vcSignalRatio_SubdetPM[i].initialize("LEDTrend", "SignalRatio",
			hashfunctions::fSubdetPM,
			new quantity::ValueQuantity(quantity::fRatio_0to2),
			new quantity::ValueQuantity(quantity::fN, true));
		_vcSignalRatio_depth[i].initialize("LEDTrend", "SignalRatio",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fRatio_0to2));
	}

	//	when processing after in ClientLike manner you have also
	//	contents in the DQMStore by default without just loading in...
/*	if (_htype==fClientLike)
	{
		_vcTimingMean_depth[_vruns.size()].initialize("LEDTask",
			"TimingMean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fTiming_TS200));
		_vcSignalMean_depth[_vruns.size()].initialize("LEDTask",
			"SignalMean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::ffC_3000));
	}
	*/
}

void LEDHarvesting::book(DQMStore* store)
{
	_cTimingMean_SubdetPM.book(store, _emap);
	_cSignalRatio_SubdetPM.book(store, _emap);
	char runname[10];
	for (uint32_t i=0; i<_vruns.size(); i++)
	{
		sprintf(runname, "vs%d", _vruns[i]);
		_vcSignalRatio_SubdetPM[i].book(store, _emap,
			"Hcal", runname);
		_vcSignalRatio_depth[i].book(store, _emap,
			"Hcal", runname);
	}
}

/* virtual */ void LEDHarvesting::endRun(edm::Run const& r, 
	edm::EventSetup const& es)
{
	/*
	int run = r.runAuxiliary().run();
	if (run==1 && _htype==fClientLike)
		return;
	else if (run!=1 && _htype==fStandalone)
		_logger.dqmthrow("Standalone Harvester Mode but Run Number isn't 1!");
		*/

	//	get the emap
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	_emaphash.initialize(_emap, hcaldqm::electronicsmap::fDHashMap);

	//	get the DQMStore and book
	DQMStore *store = edm::Service<DQMStore>().operator->();
	this->book(store);
	store->showDirStructure();

	//	load all the files and process
	char runnumber[20];
	int irun=0;
	for (std::vector<std::string>::const_iterator it=_vfiles.begin();
		it!=_vfiles.end(); ++it)
	{
		store->open(*it, false, "", "", DQMStore::KeepRunDirs);
		sprintf(runnumber,"Run %d", _vruns[irun]);
		//	fill containers with data
		_vcTimingMean_depth[irun].load(store, _emap, "Hcal", "",
			std::string(runnumber), DQMStore::KeepRunDirs);
		_vcSignalMean_depth[irun].load(store, _emap, "Hcal", "",
			std::string(runnumber), DQMStore::KeepRunDirs);

		std::vector<HcalGenericDetId> dids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=dids.begin();
			it!=dids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			double t = _vcTimingMean_depth[irun].getBinContent(did);
			double s = _vcSignalMean_depth[irun].getBinContent(did);
			double sref = _vcSignalMean_depth[0].getBinContent(did);
			double entref = _vcSignalMean_depth[0].getBinEntries(did);
			double ent = _vcSignalMean_depth[irun].getBinEntries(did);
			if (entref==0 || ent==0)
				continue;

			_cTimingMean_SubdetPM.fill(did, _vruns[irun], t);
			_cSignalRatio_SubdetPM.fill(did, _vruns[irun], s/sref);
			_vcSignalRatio_SubdetPM[irun].fill(did, s/sref);
			_vcSignalRatio_depth[irun].fill(did, s/sref);
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
