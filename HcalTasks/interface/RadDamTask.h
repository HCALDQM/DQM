#ifndef RadDamTask_h
#define RadDamTask_h

/*
 *	file:			RadDamTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/ContainerCompact.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSignle1D.h"

using namespace hcaldqm;
class RadDamTask : public DQTask
{
	public:
		RadDamTask(edm::ParameterSet const&);
		virtual ~RadDamTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);

	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(int);
		virtual bool _isApplicable(edm::Event const&);

		//	vars
		edm::InputTag	_tagHF;

		//	vector of Detector Ids for RadDam
		std::vector<HcalDetId>	_vDetIds;

		//	Cuts

		//	Compact

		//	1D
		std::vector<ContainerSingle1D> _vcShape;
};

#endif







