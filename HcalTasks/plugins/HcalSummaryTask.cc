#include "DQM/HcalTasks/interface/HcalSummaryTask.cc"

//	Constructor
HcalSummaryTask::HcalSummaryTask(edm::ParameterSet const&):
	hcaldqm::HcalDQSource(ps)
{}

/* virtual */ void HcalSummaryTask::~HcalSummarTask()
{}

/* virtua */void HcalSummaryTask::beginLuminosityBlock(
		edm::LuminosityBlock const& ls, edm::EventSetup const& es)
{
	HcalDQSource::beginLuminosityBlock(lb ,es);
}

/* virtua */void HcalSummaryTask::endLuminosityBlock(
		edm::LuminosityBlock const& ls, edm::EventSetup const& es)
{
	HcalDQSource::endLuminosityBlock(lb ,es);
}

/* virtual */ void HcalSummaryTask::doWork(edm::Event const& e,
		edm::EventSetup const& es)
{
	//	declare
	edm::Handle<DcsStatusCollection>			cdcs;
	
	//	Init the collection with throw
	INITCOLL(_labels["DCS"], cdcs);

	_dcs.initDcs(*cdcs);
}

DEFINE_FWK_MODULE(HcalSummaryTask);
