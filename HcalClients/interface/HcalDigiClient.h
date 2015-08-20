#ifndef HCALDIGICLIENT_H
#define HCALDIGICLIENT_H

/*
 *	file:				HcalDigiClient.h
 *	Author:				VK
 *	Start Date:			05/06/2015
 */

#include "DQM/HcalCommon/interface/HcalDQClient.h"

/*
 *	HcalDigiClient class - Base class for the DigiClients
 */
class HcalDigiClient : public hcaldqm::HcalDQClient
{
	public:
		HcalDigiClient(edm::ParameterSet const&);
		virtual ~HcalDigiClient();

		//	Generic doWork functions for all DigiClients
		//	per LS or per Run
		virtual void doWork(DQMStore::IGetter&,
				edm::LuminosityBlock const&, edm::EventSetup const&);

	protected:
		//	Apply Reset/Update if neccessary
		//	periodfalg: 0 for per Event Reset and 1 for per LS
		virtual void reset(int const periodflag);
};

#endif 
