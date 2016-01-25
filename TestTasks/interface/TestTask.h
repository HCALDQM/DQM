#ifndef TestTask_h
#define TestTask_h

/*
 *	file:			TestTask.h
 *	Author:			Viktor KHristenko
 *	Description:
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container1D.h"

using namespace hcaldqm;
class TestTask : public DQTask
{
	public:
		TestTask(edm::ParameterSet const&);
		virtual ~TestTask(){}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		//	tags
		edm::InputTag	_tagHF;

		//	Electronics Map
		HcalElectronicsMap const *_emap;

		//	Containers
		Container1D		_cTest;
		Container1D		_cTest1;
};

#endif



