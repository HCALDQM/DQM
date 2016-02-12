#include "DQM/HcalHarvesting/interface/HcalHarvesting.h"

HcalHarvesting::HcalHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps)
{
}

/* virtual */ void HcalHarvesting::_dqmEndLuminosityBlock(DQMStore::IBooker&,
	DQMStore::IGetter& ig, edm::LuminosityBlock const&, 
	edm::EventSetup const&)
{
	std::cout << "Harvesting LS" << _currentLS << std::endl;
	//	to save memory, all we need right now is
	//	1) get MEs with LS on the axis
	//	2) check if currentLS%numLSstart==0 
	//	3) if yes, extend once again
}

/* virtual */ void HcalHarvesting::_dqmEndJob(DQMStore::IBooker&,
	DQMStore::IGetter&)
{}

DEFINE_FWK_MODULE(HcalHarvesting);
