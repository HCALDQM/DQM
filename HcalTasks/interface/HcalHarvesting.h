#ifndef HcalHarvesting_h
#define HcalHarvesting_h

/**
 *	file:		HcalHarvesting.h
 *	Author:		VK
 *	Date:		..
 *	Description: 
 *		This is DQMEDAnalyzer which is a edm::one module. edm::one enforces
 *		that only 1 run is being processed.
 *		https://twiki.cern.ch/twiki/bin/view/CMSPublic/FWMultithreadedFrameworkOneModuleInterface - for details.
 *
 *		HcalHarvesting is responsible for Status Evaluation and Summary
 *		Generation. In this step RAW, DIGI, RECO + TP Data Tiers 
 *		are evaluated and Summary is generated. 
 *		___________
 *		Online:
 *		There is always a Current Summary - Summary for the Current LS 
 *		being Evaluated. It might and might not include the information 
 *		from previous LSs, depending on the Quantity.
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
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

using namespace hcaldqm;

class HcalHarvesting : public DQHarvester
{
	public:
		HcalHarvesting(edm::ParameterSet const&);
		virtual ~HcalHarvesting(){}

	protected:
		virtual void _dqmEndLuminosityBlock(DQMStore::IBooker&,
			DQMStore::IGetter&, edm::LuminosityBlock const&,
			edm::EventSetup const&);
		virtual void _dqmEndJob(DQMStore::IBooker&,
			DQMStore::IGetter&);

		//	flags to harvest...
		int _modules[4];
		bool _rawHarvesting;
		bool _digiHarvesting;
		bool _recoHarvesting;
		bool _tpHarvesting;

		//	electronicsmap hash
		electronicsmap::ElectronicsMap _emaphash;

		//	flag names
		std::vector<std::string> _frawnames;
		std::vector<std::string> _fdiginames;
		std::vector<std::string> _freconames;
		std::vector<std::string> _ftpnames;

		//	reportSummaryMap
		MonitorElement *_reportSummaryMap;
};

#endif
