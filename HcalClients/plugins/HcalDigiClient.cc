#include "DQM/HcalClients/interface/HcalDigiClient.h"

HcalDigiClient::HcalDigiClient(edm::ParameterSet const& ps)
	: hcaldqm::HcalDQClient(ps)
{}

/* virtual */HcalDigiClient::~HcalDigiClient()
{}

//	reset
/* virtual */ void HcalDigiClient::reset(int const periodflag)
{
	//	Collection Class determines itself who needs a reset and when
	//	Do it only for Monitor Modules which have been booked in this client
	HcalDQClient::reset(periodflag);
	if (periodflag==0)
	{
		//	each event 
	}
	else if (periodflag==1)
	{
		//	each LS
	}
}

/* virtual */ void HcalDigiClient::doWork(DQMStore::IGetter& ig,
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{

}

DEFINE_FWK_MODULE(HcalDigiClient);










