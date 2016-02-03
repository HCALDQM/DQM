#include "DQM/HcalCommon/interface/DQHarvester.h"

namespace hcaldqm
{
	DQHarvester::DQHarvester(edm::ParameterSet const& ps) :
		DQModule(ps)
	{}

	/* virtual */ void DQHarvester::dqmEndLuminosityBlock(DQMStore::IGetter&,
		edm::LuminosityBlock const&, edm::EventSetup const&)
	{}
	/* virtual */ void DQHarvester::dqmEndJob(DQMStore::IBooker&, 
		DQMStore::IGetter&)
	{}
}
