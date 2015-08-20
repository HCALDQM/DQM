#ifndef HCALTPCLIENT_H
#define HCALTPCLIENT_H

/*
 *	file:				HcalTPClient.h
 *	Author:				VK
 *	Start Date:			05/06/2015
 */

#include "DQM/HcalCommon/interface/HcalDQClient.h"

/*
 *	HcalTPClient class - Base class for the TPClients
 */
class HcalTPClient : public hcaldqm::HcalDQClient
{
	public:
		HcalTPClient(edm::ParameterSet const&);
		virtual ~HcalTPClient();

		//	Generic doWork functions for all TPClients
		//	per LS or per Run
		virtual void doWork(DQMStore::IGetter&,
				edm::LuminosityBlock const&, edm::EventSetup const&);

	protected:
		//	Apply Reset/Update if neccessary
		//	periodfalg: 0 for per Event Reset and 1 for per LS
		virtual void reset(int const periodflag);
};

#endif 
