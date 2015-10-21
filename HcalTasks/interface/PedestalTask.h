#ifndef PedestalTask_h
#define PedestalTask_h

/*
 *	file:			PedestalTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

using namespace hcaldqm;
class PedestalTask : public DQTask
{
	public:
		PedestalTask(edm::ParameterSet const&);
		virtual ~PedestalTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(int);
		virtual void _isApplicable(edm::Event const&);

		//	vars
		edm::InputTag	_tagHBHE;
		edm::InputTag	_tagHO;
		edm::InputTag	_tagHF;

		ContainerCompact _cPedestals;

		Container2D		_cOccupancy2D_depth;
		ContainerProf1D _OccupancyVSls_SubDet;
};

#endif







