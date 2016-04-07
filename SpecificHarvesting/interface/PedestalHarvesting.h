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

		//	current run handle
		int _runnum;

		//	new containers
		//	vs run# container
		//	2D for each run comparison
		//	1D for each run comparison
		Container2D _cPedestalDiff_SubdetPM;
		std::vector<Container1D> _vcPedestalDiff_SubdetPM;
		std::vector<ContainerProf2D> _vcPedestalDiff_depth;

		//	containers to be loaded
		//	vector of means for each run
		//	1 container for the current run histograms
		std::vector<ContainerProf2D> _vcPedestalMean_depth;
		ContainerProf2D _cPedestalMean_depth;

		virtual void _dqmEndLuminosityBlock(DQMStore::IBooker&,
			DQMStore::IGetter&,
			edm::LuminosityBlock const&, edm::EventSetup const&) {}
		virtual void _dqmEndJob(DQMStore::IBooker&,
			DQMStore::IGetter&) {}
};

#endif
