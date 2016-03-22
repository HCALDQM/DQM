#include "DQM/Harvesting/interface/PedestalHarvesting.h"

PedestalHarvesting::PedestalHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
	_vruns = ps.getUntrackedParameter<std::vector<int> >("runs");

	_cPedestalDiff_SubdetPM.initialize("PedestalTrend", "PedestalDiff", 
		hashfunctions::fSubdetPM,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fAroundZero),
		new quantity::ValueQuantity(quantity::fN, true));

	//	initialize vectors of containers
	_vcPedestalDiff_depth.resize(_vruns.size());
	_vcPedestalDiff_SubdetPM.resize(_vruns.size());
	_vcPedestalMean_depth.resize(_vruns.size());
	for (uint32_t i=0; i<_vruns.size(); i++)
	{
		//	init containers to load
		_vcPedestalMean_depth[i].initialize("PedestalTask",
			"Mean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fADC_15));
		
		//	 init new containers
		_vcPedestalDiff_SubdetPM[i].initialize("PedestalTrend", "PedestalDiff",
			hashfunctions::fSubdetPM,
			new quantity::ValueQuantity(quantity::fAroundZero),
			new quantity::ValueQuantity(quantity::fN, true));
		_vcPedestalDiff_depth[i].initialize("PedestalTrend", "PedestalDiff",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fAroundZero));
	}
}

void PedestalHarvesting::book(DQMStore* store)
{
	_cPedestalDiff_SubdetPM.book(store, _emap);
	char runname[10];
	for (uint32_t i=0; i<_vruns.size(); i++)
	{
		sprintf(runname, "vs%d", _vruns[i]);
		_vcPedestalDiff_SubdetPM[i].book(store, _emap,
			"Hcal", runname);
		_vcPedestalDiff_depth[i].book(store, _emap,
			"Hcal", runname);
	}
}

/* virtual */ void PedestalHarvesting::endRun(edm::Run const& r, 
	edm::EventSetup const& es)
{
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
		_vcPedestalMean_depth[irun].load(store, _emap, "Hcal", "",
			std::string(runnumber), DQMStore::KeepRunDirs);

		std::vector<HcalGenericDetId> dids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=dids.begin();
			it!=dids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			double s = _vcPedestalMean_depth[irun].getBinContent(did);
			double sref = _vcPedestalMean_depth[0].getBinContent(did);
			double entref = _vcPedestalMean_depth[0].getBinEntries(did);
			double ent = _vcPedestalMean_depth[irun].getBinEntries(did);
			if (entref==0 || ent==0)
				continue;

			_cPedestalDiff_SubdetPM.fill(did, _vruns[irun], s-sref);
			_vcPedestalDiff_SubdetPM[irun].fill(did, s-sref);
			_vcPedestalDiff_depth[irun].fill(did, s-sref);
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
