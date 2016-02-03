#ifndef HarvestingTest_h
#define HarvestingTest_h

/**
 *	file:		
 *	Author:	
 *	Date:
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/DQModule.h"

using namespace hcaldqm;

class HarvestingTest : public DQMEDHarvester, public DQModule
{
	public:
		HarvestingTest(edm::ParameterSet const&);
		virtual ~HarvestingTest(){}

		virtual void endRun(edm::Run const&, edm::EventSetup const& es);

	protected:
		//	electronicsmap
		HcalElectronicsMap const* _emap;

		//	ROOT files to be processed
		std::vector<std::string> _vfiles;
};

#endif
