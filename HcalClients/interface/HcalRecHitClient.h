#ifndef HCALRECHITCLIENT_H
#define HCALRECHITCLIENT_H

/*
 *	file:				HcalRecHitClient.h
 *	Author:				VK
 *	Start Date:			05/06/2015
 */

#include "DQM/HcalCommon/interface/HcalDQClient.h"

/*
 *	HcalRecHitClient class - Base class for the RecHitClients
 */
class HcalRecHitClient : public hcaldqm::HcalDQClient
{
	public:
		HcalRecHitClient(edm::ParameterSet const&);
		virtual ~HcalRecHitClient();

		//	Generic doWork functions for all RecHitClients
		//	per LS or per Run
		virtual void doWork(DQMStore::IGetter&,
				edm::LuminosityBlock const&, edm::EventSetup const&);

	protected:
		//	Apply Reset/Update if neccessary
		//	periodfalg: 0 for per Event Reset and 1 for per LS
		virtual void reset(int const periodflag);
};

#endif 
