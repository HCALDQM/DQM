
#include "DQM/Specific/interface/QIE10Test.h"

using namespace hcaldqm;
QIE10Test::QIE10Test(edm::ParameterSet const&):
	DQTask(ps),

{
	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void QIE10Test::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
}

/* virtual */ void QIE10Test::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{

	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE10Test::_process(edm::Event const&, 
	edm::EventSetup const&)
{}

/* virtual */ void QIE10Test::_resetMonitors(UpdateFreq)
{
}

