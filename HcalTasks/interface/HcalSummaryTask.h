#ifndef HCALSUMMARYTASK_H
#define HCALSUMMARYTASK_H

/*
 *	file:			HcalSummaryTask.h
 *	Author:			Viktor Khristenko
 *	Start Date:		03/04/2015
 */

#include "DQM/HcalCommon/interface/HcalMECollection.h"
#include "DQM/HcalCommon/interface/HcalDQSource.h"
#include "DQM/HcalCommon/interface/HcalMasters.h"

class HcalSummaryTask : public hcaldqm::HcalDQSource
{
	public:
		HcalSummaryTask(edm::ParameterSet const&);
		virtual ~HcalSummaryTask();

		virtual void doWork(edm::Event const&e,
				edm::EventSetup const& es);

		virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
				edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
				edm::EventSetup const&);

		virtual void reset(int const);
	
	private:
		//	Use DcsMaster
		hcaldqm::HcalDcsMaster	_dcs;
};

#endif
