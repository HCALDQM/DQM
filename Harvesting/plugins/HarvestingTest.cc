#include "DQM/Harvesting/interface/HarvestingTest.h"

HarvestingTest::HarvestingTest(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
	std::cout << "CONSTRUCTOR" << std::endl;
}

DEFINE_FWK_MODULE(HarvestingTest);
