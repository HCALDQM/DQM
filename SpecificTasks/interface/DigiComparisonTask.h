#ifndef DigiComparisonTask_h
#define DigiComparisonTask_h

/**
 *	file:		DigiComparisonTask.h
 *	Author:		Viktor Khristenko
 *	Date:		08.12.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

using namespace hcaldqm;
class DigiComparisonTask : public DQTask
{
	public: 
		DigiComparisonTask(edm::ParameterSet const&);
		virtual ~DigiComparisonTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		//	Tags and corresponding Tokens
		edm::InputTag	_tagHBHE1;
		edm::InputTag	_tagHBHE2;
		edm::EDGetTokenT<HBHEDigiCollection>	_tokHBHE1;
		edm::EDGetTokenT<HBHEDigiCollection>	_tokHBHE2;

		/**
		 *	Containers
		 */

		//	ADC
		Container2D			_cADC_SubDet[10];

		//	Mismatched
		Cotainer2D			_cMsm_depth;

		//	Missing Completely
		Container2D			_cMsn_depth;
};

#endif
