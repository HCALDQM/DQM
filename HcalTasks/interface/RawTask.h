#ifndef RawTask_h
#define RawTask_h

/*
 *	file:			RawTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"

using namespace hcaldqm;
class RawTask : public DQTask
{
	public:
		RawTask(edm::ParameterSet const&);
		virtual ~RawTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

		enum RawFlag_Event
		{
			fEvnMsm = 0,
			fOrnMsm = 1,
			fBcnMsm = 2,

			nRawFlag = 3
		};

	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		//	Tags
		edm::InputTag	_tagFEDs;

		//	Flag Names
		std::vector<std::string> _fNames;

		//	Counters

		//	Cuts

		// Containers by quantities

		//	Evn/Bcn/Orn Mismatch
		ContainerSingle2D _cVMEEvnMsm;
		ContainerSingle2D _cVMEBcnMsm;
		ContainerSingle2D _cVMEOrnMsm;
		ContainerSingle2D _cuTCAEvnMsm;
		ContainerSingle2D _cuTCABcnMsm;
		ContainerSingle2D _cuTCAOrnMsm;

		//	Occupancy
		ContainerSingle2D _cVMEOccupancy;
		ContainerSingle2D _cuTCAOccupancy;

		//	Summary Containers
		ContainerSingle2D		_cSummary;
		Container2D		_cSummaryvsLS_FED;
};

#endif







