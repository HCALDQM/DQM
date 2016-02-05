#ifndef PedestalHarvesting_h
#define PedestalHarvesting_h

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
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

using namespace hcaldqm;

class PedestalHarvesting : public DQHarvester
{
	public:
		PedestalHarvesting(edm::ParameterSet const&);
		virtual ~PedestalHarvesting(){}

		virtual void endRun(edm::Run const&, edm::EventSetup const&);

	protected:
		void book(DQMStore*);

		//	electronicsmap
		HcalElectronicsMap const* _emap;
		electronicsmap::ElectronicsMap _emaphash;

		//	ROOT files to be processed
		//	0 is the one with respect to which all ratios are computed
		std::vector<std::string>	_vfiles;
		std::vector<int>			_vruns;

		//	new containers
		Container2D _cPedestalDiff_SubdetPM;
		std::vector<Container1D> _vcPedestalDiff_SubdetPM;
		std::vector<ContainerProf2D> _vcPedestalDiff_depth;

		//	containers to be loaded
		std::vector<ContainerProf2D> _vcPedestalMean_depth;
};

#endif
