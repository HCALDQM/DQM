#include "DQM/SpecificHarvesting/interface/PedestalHarvesting.h"

PedestalHarvesting::PedestalHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
	_vruns = ps.getUntrackedParameter<std::vector<int> >("runs");
}

void PedestalHarvesting::book(DQMStore *store)
{
	_cPedestalDiff_SubdetPM.initialize(_name, "MeanDiff", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fAroundZero),
		new quantity::ValueQuantity(quantity::fN, true));
	_cPedestalMean_depth.initialize("PedestalTask", "Mean",
		hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fADC_15));

	//	initialize vectors of containers
	_vcPedestalDiff_depth.resize(_vfiles.size());
	_vcPedestalDiff_SubdetPM.resize(_vfiles.size());
	_vcPedestalMean_depth.resize(_vfiles.size());
	for (uint32_t i=0; i<_vfiles.size(); i++)
	{
		//	init containers to load
		_vcPedestalMean_depth[i].initialize("PedestalTask",
			"Mean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fADC_15));
		
		//	 init new containers
		_vcPedestalDiff_SubdetPM[i].initialize(_name, "MeanDiff",
			hashfunctions::fSubdetPM,
			new quantity::ValueQuantity(quantity::fAroundZero),
			new quantity::ValueQuantity(quantity::fN, true));
		_vcPedestalDiff_depth[i].initialize(_name, "MeanDiff",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fAroundZero));
	}
	
	_cPedestalDiff_SubdetPM.book(store, _emap);
	char runname[10];
	for (uint32_t i=0; i<_vfiles.size(); i++)
	{
		sprintf(runname, "vs%d", _vruns[i]);
		_vcPedestalDiff_SubdetPM[i].book(store, _emap,
			_subsystem, runname);
		_vcPedestalDiff_depth[i].book(store, _emap,
			_subsystem, runname);
	}
}

/* virtual */ void PedestalHarvesting::endRun(edm::Run const& r, 
	edm::EventSetup const& es)
{
	if (r.runAuxiliary().run()==1)
		return;
	else
		_runnum = r.runAuxiliary().run();
	//	now, #runs = #files+1, the last run# is the current run
	_vruns.push_back(_runnum);

	//	get the emap
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	_emaphash.initialize(_emap, hcaldqm::electronicsmap::fD2EHashMap);

	//	get the DQMStore and book
	DQMStore *store = edm::Service<DQMStore>().operator->();
	store->showDirStructure();
	this->book(store);
	store->showDirStructure();

	//	load the current run histograms
	_cPedestalMean_depth.load(store, _emap);

	//	1. load file by file
	//	2. produce pedestal difference vs run# (irun 0 is ref)
	//	3. compare all runs with the current run
	char runnumber[20];
	int irun=0;
	for (std::vector<std::string>::const_iterator it=_vfiles.begin();
		it!=_vfiles.end(); ++it)
	{
		//	open the file and load container
		store->open(*it, false, "", "", DQMStore::KeepRunDirs);
		sprintf(runnumber,"Run %d", _vruns[irun]);
		_vcPedestalMean_depth[irun].load(store, _emap, _subsystem, "",
			std::string(runnumber), DQMStore::KeepRunDirs);

		//	go thru all the channels and compare
		std::vector<HcalGenericDetId> dids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=dids.begin();
			it!=dids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			double s = _vcPedestalMean_depth[irun].getBinContent(did);
			double scur = _cPedestalMean_depth.getBinContent(did);
			double sref = _vcPedestalMean_depth[0].getBinContent(did);
			double entref = _vcPedestalMean_depth[0].getBinEntries(did);
			double ent = _vcPedestalMean_depth[irun].getBinEntries(did);
			double entcur = _cPedestalMean_depth.getBinEntries(did);
			if (entref==0 || ent==0 || entcur==0)
				continue;

			//	fill vs run# plot 
			//	fill current mean - irun mean by depth and SubdetPM
			//	for irun=0 fill current mean - ref mean vs run# plot
			_cPedestalDiff_SubdetPM.fill(did, _vruns[irun], s-sref);
			if (irun==0)
				_cPedestalDiff_SubdetPM.fill(did, _vruns[_vruns.size()-1], scur-sref);
			_vcPedestalDiff_SubdetPM[irun].fill(did, scur-s);
			_vcPedestalDiff_depth[irun].fill(did, scur-s);
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

DEFINE_FWK_MODULE(PedestalHarvesting);
