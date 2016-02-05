#include "DQM/Harvesting/interface/HarvestingTest.h"

HarvestingTest::HarvestingTest(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	std::cout << "CONSTRUCTOR" << std::endl;

	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
	_vruns = ps.getUntrackedParameter<std::vector<int> >("runs");

	_cTest_Subdet.initialize("Trend", "Test", hashfunctions::fSubdet,
		new quantity::RunNumber(_vruns),
		new quantity::ValueQuantity(quantity::fTiming_TS200),
		new quantity::ValueQuantity(quantity::fN, true));

	//	initialize the containers in which we will load data
	_vcTimingMean_depth.resize(_vruns.size());
	for (uint32_t i=0; i<_vruns.size(); i++)
		_vcTimingMean_depth[i].initialize("LEDTask",
			"TimingMean", hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fTiming_TS200));
}

/* virtual */ void HarvestingTest::endRun(edm::Run const&, 
	edm::EventSetup const& es)
{
	//	get the emap
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();

	//	get the DQMStore
	DQMStore *store = edm::Service<DQMStore>().operator->();
	_cTest_Subdet.book(store, _emap);
	store->showDirStructure();
/*	store->open(_vfiles[0], false, "", "", DQMStore::KeepRunDirs);
	store->showDirStructure();
	store->open(_vfiles[1], false, "", "", DQMStore::KeepRunDirs);
	store->showDirStructure();
	*/

	//	load all the files
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

		std::vector<HcalGenericDetId> dids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=dids.begin();
			it!=dids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			double t = _vcTimingMean_depth[irun].getBinContent(did);
			_cTest_Subdet.fill(did, _vruns[irun], t);
		}

		//	delet the folder you just loaded
		store->rmdir(runnumber);
	
		//	inc
		irun++;
	}
	store->showDirStructure();

}

DEFINE_FWK_MODULE(HarvestingTest);
