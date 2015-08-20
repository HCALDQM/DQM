#include "DQM/HcalClients/interface/HcalTPClient.h"

HcalTPClient::HcalTPClient(edm::ParameterSet const& ps)
	: hcaldqm::HcalDQClient(ps)
{}

/* virtual */HcalTPClient::~HcalTPClient()
{}

//	reset
/* virtual */ void HcalTPClient::reset(int const periodflag)
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

/* virtual */ void HcalTPClient::doWork(DQMStore::IGetter& ig,
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{

}

DEFINE_FWK_MODULE(HcalTPClient);










