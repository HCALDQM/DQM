#ifndef PedestalTask_h
#define PedestalTask_h

/*
 *	file:			PedestalTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerCompact.h"
#include "DQM/HcalCommon/interface/ContainerXXX.h"

using namespace hcaldqm;
class PedestalTask : public DQTask
{
	public:
		PedestalTask(edm::ParameterSet const&);
		virtual ~PedestalTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&)
		{this->_dump();}

	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);
		virtual bool _isApplicable(edm::Event const&);
		virtual void _dump();

		//	tags and tokens
		edm::InputTag	_tagHBHE;
		edm::InputTag	_tagHO;
		edm::InputTag	_tagHF;
		edm::InputTag	_tagTrigger;
		edm::EDGetTokenT<HBHEDigiCollection> _tokHBHE;
		edm::EDGetTokenT<HODigiCollection> _tokHO;
		edm::EDGetTokenT<HFDigiCollection> _tokHF;
		edm::EDGetTokenT<HcalTBTriggerData> _tokTrigger;

		//	emap
		HcalElectronicsMap const* _emap;

		ContainerCompact	_cPedestals;
		ContainerXXX		_cPeds;

		//	1D
		Container1D		_cPedestalMeans_Subdet;
		Container1D		_cPedestalRMSs_Subdet;

		//	2D
		Container2D		_cPedestalMeans_depth;
		Container2D		_cPedestalRMSs_depth;
};

#endif







