#include "DQM/HcalClients/interface/HcalRawClient.h"

HcalRawClient::HcalRawClient(edm::ParameterSet const& ps)
	: hcaldqm::HcalDQClient(ps)
{}

/* virtual */HcalRawClient::~HcalRawClient()
{}

//	reset
/* virtual */ void HcalRawClient::reset(int const periodflag)
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

/* virtual */ void HcalRawClient::doWork(DQMStore::IGetter& ig,
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{

}

DEFINE_FWK_MODULE(HcalRawClient);










