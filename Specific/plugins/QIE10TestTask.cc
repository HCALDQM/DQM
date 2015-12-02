
#include "DQM/Specific/interface/QIE10TestTask.h"

using namespace hcaldqm;
QIE10TestTask::QIE10TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 10, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 11, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 12, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 13, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 10, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 11, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 12, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 13, 1));

	char name[200];
	for (unsigned int i=0; i<_cDetIds.size(); i++)
	{
		sprintf(name, "_iphi%d_ieta%d", _cDetIds[i].iphi(), _cDetIds[i].ieta());
		_vcShape.push_back(ContainerSingle1D(_name+"/Shape",
			"Shape"+std::string(name), 
			new axis::ValueAxis(axis::fXaxis, fTimeTS),
			new axis::ValueAxis(axis::fYaxis, fNomFC)));
	}

	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void QIE10TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);

	for (unsigned int i=0; i<_cDetIds.size(); i++)
		_vcShape[i].book(ib, _subsystem);
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

//	std::cout << "Event" << std::endl;
	for (int i=0; i<cqie10->size(); i++)
	{
		HcalDetId did = (*cqie10)[i].detid();

		for (unsigned int ii=0; ii<_cDetIds.size(); ii++)
			if (did==_cDetIds[i])
			{
//				std::cout << did.iphi() << " " << did.ieta() << " " << did.depth()
//				<< std::endl;
				for (int j=0; j<(*cqie10)[i].samples(); j++)
				{
					_vcShape[i].fill(j, adc2fC[(*cqie10)[i][j].adc()]);	
//					std::cout << (*cqie10)[i][j].adc() << "  ";
				}
			}
			else 
				std::cout << "Didn't match" << std::endl;
	}
}

/* virtual */ void QIE10TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10TestTask);
