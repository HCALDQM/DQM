#ifndef HarvestingTest_h
#define HarvestingTest_h

/**
 *	file:		
 *	Author:	
 *	Date:
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/DQHarvester.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf1D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf2D.h"

using namespace hcaldqm;

class HarvestingTest : public DQHarvester
{
	public:
		HarvestingTest(edm::ParameterSet const&);
		virtual ~HarvestingTest(){}

		virtual void endRun(edm::Run const&, edm::EventSetup const&);

	protected:
		//	electronicsmap
		HcalElectronicsMap const* _emap;

		//	ROOT files to be processed
		std::vector<std::string>	_vfiles;
		std::vector<int>			_vruns;

		//	Containers
		Container2D _cTest_Subdet;
		std::vector<ContainerProf2D> _vcTimingMean_depth;

		//	DQHarvester is abstract
		virtual void _dqmEndLuminosityBlock(
			DQMStore::IBooker&, DQMStore::IGetter&,
			edm::LuminosityBlock const&, edm::EventSetup const&) {}
		virtual void _dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) {}
};

#endif
