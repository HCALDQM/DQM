#ifndef TPComparisonTask_h
#define TPComparisonTask_h

/**
 *	file:		TPComparisonTask.h
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
class TPComparisonTask : public DQTask
{
	public: 
		TPComparisonTask(edm::ParameterSet const&);
		virtual ~TPComparisonTask()
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
		edm::InputTag	_tag1;
		edm::InputTag	_tag2;
		edm::EDGetTokenT<HcalTrigPrimDigiCollection>	_tok1;
		edm::EDGetTokenT<HcalTrigPrimDigiCollection>	_tok2;

		/**
		 *	Containers
		 */

		//	Et
		Container2D			_cEt_TPSubDet[4];

		//	FG
		Container2D			_cFG_TPSubDet[4];

		//	Missing
		Container2D			_cMsn_depth;

		//	mismatches
		Conatiner2D			_cEtMsm_depth;
		Container2D			_cFGMsm_depth;
};

#endif
