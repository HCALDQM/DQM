#ifndef HarvestingTest_h
#define HarvestingTest_h

/**
 *	file:		
 *	Author:	
 *	Date:
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/DQHarvester.h"

using namespace hcaldqm;

class HarvestingTest : public hcaldqm::DQHarvester
{
	public:
		HarvestingTest(edm::ParameterSet const&);
		virtual ~HarvestingTest(){}

		virtual void endRun(edm::Run const&, edm::EventSetup const& es)
		{
			edm::ESHandle<HcalDbService> dbs;
			es.get<HcalDbRecord>().get(dbs);
			HcalElectronicsMap const* emap = dbs->getHcalMapping();

			std::vector<HcalGenericDetId> dids = emap->allPrecisionId();
			for (std::vector<HcalGenericDetId>::const_iterator it=
				dids.begin(); it!=dids.end(); ++it)
			{
				std::cout << *it << std::endl;
			}
		}

	protected:
};

#endif
