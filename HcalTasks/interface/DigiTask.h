#ifndef DigiTask_h
#define DigiTask_h

/*
 *	file:			DigiTask.h
 *	Author:			Viktor Khristenko
 *	Date:			16.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
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

		//	1D 
		Container1D		_cADCperTS_SubDet;
		Container1D		_cfCperTS_SubDet;
		Container1D		_cShape_SubDet_iphi;
		Container1D		_cShapeSumQcut_SubDet_iphi;
		Container1D		_cSumQ_SubDet_iphi;
		Container1D		_cTimingSumQcut_SubDet_iphi;
		Container1D		_cOccupancyvsiphi_SubDet;

		//	Prof1D
		ContainerProf1D	_cSumQvsLS_SubDet_iphi;
		ContainerProf1D	_cTimingSumQcutvsieta_SubDet_iphi;
		ContainerProf1D	_cTimingSumQcutvsiphi_SubDet_ieta;

		//	2D
		Container2D		_cOccupancy_depth;

		//	Prof2D
		ContainerProf2D	_cOccupancyiphivsLS_SubDet;
		ContainerProf2D	_cSumQ_depth;
		ContainerProf2D	_cTimingSumQcut_depth;
};

#endif







