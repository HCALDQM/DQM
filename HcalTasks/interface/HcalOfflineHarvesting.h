#ifndef HcalOfflineHarvesting_h
#define HcalOfflineHarvesting_h

/**
 *	file:		HcalOffineHarvesting.h
 *	Author:		VK
 *	Date:		..
 *	Description: 
 *		This is DQMEDAnalyzer which is a edm::one module. edm::one enforces
 *		that only 1 run is being processed.
 *		https://twiki.cern.ch/twiki/bin/view/CMSPublic/FWMultithreadedFrameworkOneModuleInterface - for details.
 *
 *		___________
 *		Offline:
 *		For Offline only Run Summary is being generated. As it is meaningless
 *		to have current LS information being delivered. Only Total Summary
 *		makes sense
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
#include "DQM/HcalCommon/interface/ContainerXXX.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

using namespace hcaldqm;

class HcalOfflineHarvesting : public DQHarvester
{
	public:
		HcalOfflineHarvesting(edm::ParameterSet const&);
		virtual ~HcalOfflineHarvesting(){}

		virtual void beginRun(edm::Run const&,
			edm::EventSetup const&);

	protected:
		virtual void _dqmEndLuminosityBlock(DQMStore::IBooker&,
			DQMStore::IGetter&, edm::LuminosityBlock const&,
			edm::EventSetup const&);
		virtual void _dqmEndJob(DQMStore::IBooker&,
			DQMStore::IGetter&);

		//	Electronics Hash Map
		electronicsmap::ElectronicsMap _ehashmapD;
		electronicsmap::ElectronicsMap _ehashmapT;
		filter::HashFilter _filter_FEDHF;
			
		//	flags to harvest...
		int _modules[4];
		bool _rawHarvesting;
		bool _digiHarvesting;
		bool _recoHarvesting;
		bool _tpHarvesting;

		//	flag names
		std::vector<std::string> _frawnames;
		std::vector<std::string> _fdiginames;
		std::vector<std::string> _freconames;
		std::vector<std::string> _ftpnames;

		//	reportSummaryMap
		MonitorElement *_reportSummaryMap;
};

#endif
