#ifndef DigiTask_h
#define DigiTask_h

/*
 *	file:			DigiTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

using namespace hcaldqm;
class DigiTask : public DQTask
{
	public:
		DigiTask(edm::ParameterSet const&);
		virtual ~DigiTask()
		{}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
	protected:
		//	funcs
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(int);

		//	vars
		int				_numDigis[constants::SUBDET_NUM];
		edm::InputTag	_tagHBHE;
		edm::InputTag	_tagHO;
		edm::InputTag	_tagHF;

		Container2D		_cOccupancy2D_depth;
		ContainerProf1D _OccupancyVSls_SubDet;
};

#endif







