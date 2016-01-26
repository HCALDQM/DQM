#ifndef LEDTask_h
#define LEDTask_h

/*
 *	file:			LEDTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/ContainerCompact.h"
#include "DQM/HcalCommon/interface/ContainerXXX.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

using namespace hcaldqm;
class LEDTask : public DQTask
{
	public:
		LEDTask(edm::ParameterSet const&);
		virtual ~LEDTask()
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

		//	Cuts
		double _lowHBHE;
		double _lowHO;
		double _lowHF;

		//	Compact
		ContainerXXX _cSignals;
		ContainerXXX _cTiming;

		//	1D
		Container1D		_cSignalMean_Subdet;
		Container1D		_cSignalRMS_Subdet;
		Container1D		_cTimingMean_Subdet;
		Container1D		_cTimingRMS_Subdet;

		//	Prof1D
		ContainerProf1D	_cShapeCut_SubdetPMiphi;

		//	2D
		Container2D		_cSignalMean_depth;
		Container2D		_cSignalRMS_depth;
		Container2D		_cTimingMean_depth;
		Container2D		_cTimingRMS_depth;
};

#endif







