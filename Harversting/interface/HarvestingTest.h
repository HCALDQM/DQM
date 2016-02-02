#ifndef HarvestingTest_h
#define HarvestingTest_h

/**
 *	file:		
 *	Author:	
 *	Date:
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

using namespace hcaldqm;

class HarvestingTest : public edm::EDAnalyzer
{
	public:
		HarvestingTest(edm::ParameterSet const&);
		virtual ~HarvestingTest(){}

	protected:
};

#endif
