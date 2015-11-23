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
			virtual void endLuminosityBlock(edm::LuminosityBlock const&,
				edm::EventSetup const&);

			enum TPFlag
			{
				fMsmEt = 0,
				fMsmFG = 1,
				fUniphi = 2,

				nTPFlag = 3
			};

		protected:
			//	protected funcs
			virtual void _process(edm::Event const&, edm::EventSetup const&);
			virtual void _resetMonitors(UpdateFreq);

			//	tags
			edm::InputTag	_tagData;
			edm::InputTag	_tagEmul;

			//	counters
			int _nMsmEt[constants::TPSUBDET_NUM];
			int _nMsmFG[constants::TPSUBDET_NUM];
			int _nTPs_Data[constants::TPSUBDET_NUM];
			int _nTPs_Emul[constants::TPSUBDET_NUM];

			// flags
			bool	_skip1x1;

			//	Et
			Container1D		_cEtData_SubDet;
			Container1D		_cEtEmul_SubDet;	
			Container2D		_cEtCorr_SubDet;
			ContainerProf1D	_cEtCorrRatiovsLS_TPSubDet;
			Container1D		_cEtCorrRatio_TPSubDet;
			ContainerSingle2D	_cEtMsm;
			ContainerProf1D	_cNumEtMsmvsLS_TPSubDet;
			Container1D		_cNumEtMsm_TPSubDet;
			Container1D		_cNumEtMsmvsiphi_TPSubDetPM;

			Container1D		_cEtData_SubDetPM_iphi;
			Container1D		_cEtData_SubDet_ieta;
			Container2D		_cEtCorr_SubDetPM_iphi;

			//	FG
			Container2D		_cFGCorr_SubDet;
			ContainerSingle2D	_cFGMsm;

			//	Occupancy
			ContainerSingle2D		_cOccupancyData;
			ContainerSingle2D		_cOccupancyEmul;
			Container1D				_cOccupancyDatavsiphi_TPSubDetPM;
			Container1D				_cOccupancyEmulvsiphi_TPSubDetPM;
			ContainerProf1D			_cOccupancyDatavsLS_TPSubDet;
			ContainerProf1D			_cOccupancyEmulvsLS_TPSubDet;
			ContainerSingle2D		_cMsData;
			ContainerSingle2D		_cMsEmul;
	};

#endif




