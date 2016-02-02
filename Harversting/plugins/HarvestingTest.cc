#include "DQM/Harvesting/interface/HarvestingTest.h"

HarvestingTest::HarvestingTest(edm::ParameterSet const&)
{
	std::cout << "CONSTRUCTOR" << std::endl;
}

DEFINE_FWK_MODULE(HarvestingTest);
