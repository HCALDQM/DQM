#ifndef HCALRAWCLIENT_H
#define HCALRAWCLIENT_H

/*
 *	file:				HcalRawClient.h
 *	Author:				VK
 *	Start Date:			05/06/2015
 */

#include "DQM/HcalCommon/interface/HcalDQClient.h"

/*
 *	HcalRawClient class - Base class for the RawClients
 */
class HcalRawClient : public hcaldqm::HcalDQClient
{
	public:
		HcalRawClient(edm::ParameterSet const&);
		virtual ~HcalRawClient();

		//	Generic doWork functions for all RawClients
		//	per LS or per Run
		virtual void doWork(DQMStore::IGetter&,
				edm::LuminosityBlock const&, edm::EventSetup const&);

	protected:
		//	Apply Reset/Update if neccessary
		//	periodfalg: 0 for per Event Reset and 1 for per LS
		virtual void reset(int const periodflag);
};

#endif 
