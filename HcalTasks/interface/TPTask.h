#ifndef TPTask_h
#define TPTask_h

/*
 *	file:		TPTask.h
 *	Author:		Viktor Khristenko
 *	Date:		13.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"

	using namespace hcaldqm;
	class TPTask : public DQTask
	{
		public:
			TPTask(edm::ParameterSet const& ps);
			virtual ~TPTask()
			{}

			virtual void bookHistograms(DQMStore::IBooker &,
				edm::Run const&, edm::EventSetup const&);

		protected:
			//	protected funcs
			virtual void _process(edm::Event const&, edm::EventSetup const&);
			virtual void _resetMonitors(int);

			//	tags
			edm::InputTag	_tagData;
			edm::InputTag	_tagEmul;

			//	counters

			//	Et
			Container1D		_cEtData_SubDet;
			Container1D		_cEtEmul_SubDet;	
			Container2D		_cEtCorr_SubDet;
			ContainerSingle2D	_cEtMsm;

			Container1D		_cEtData_SubDetPM_iphi;
			Container1D		_cEtData_SubDet_ieta;
			Container2D		_cEtCorr_SubDetPM_iphi;

			//	FG
			Container2D		_cFGCorr_SubDet;
			ContainerSingle2D	_cFGMsm;

			//	Occupancy
			ContainerSingle2D		_cOccupancyData;
			ContainerSingle2D		_cOccupancyEmul;
			ContainerSingle2D		_cMsData;
			ContainerSingle2D		_cMsEmul;
	};

#endif




