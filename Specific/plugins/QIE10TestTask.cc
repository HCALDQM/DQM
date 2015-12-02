
#include "DQM/Specific/interface/QIE10TestTask.h"

using namespace hcaldqm;
QIE10TestTask::QIE10TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void QIE10TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
}

/* virtual */ void QIE10TestTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{

	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE10TestTask::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<QIE10DigiCollection> cqie10;
	if (!e.getByLabel(_tagHF, cqie10))
		std::cout << "Collection isn't availalbe" << std::endl;

	for (int i=0; i<cqie10->size(); i++)
	{
		HcalDetId did = (*cqie10)[i].detid();
		std::cout << did.iphi() << " " << did.ieta() << " " << did.depth()
			<< std::endl;
	}
}

/* virtual */ void QIE10TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10TestTask);
