#include "DQM/Harvesting/interface/HarvestingTest.h"

HarvestingTest::HarvestingTest(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	std::cout << "CONSTRUCTOR" << std::endl;

	_vfiles = ps.getUntrackedParameter<std::vector<std::string> >("files");
}

/* virtual */ void HarvestingTest::endRun(edm::Run const&, 
	edm::EventSetup const&)
{
	DQMStore *store = edm::Service<DQMStore>().operator->();
	store->ShowDirStructure();
}

DEFINE_FWK_MODULE(HarvestingTest);
