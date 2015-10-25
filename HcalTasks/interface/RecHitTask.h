#ifndef RecHitTask_h
#define RecHitTask_h

/*
 *	file:		RecHitTask.h
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
	class RecHitTask : public DQTask
	{
		public:
			RecHitTask(edm::ParameterSet const& ps);
			virtual ~RecHitTask()
			{}

			virtual void bookHistograms(DQMStore::IBooker &,
				edm::Run const&, edm::EventSetup const&);

		protected:
			//	protected funcs
			virtual void _process(edm::Event const&, edm::EventSetup const&);
			virtual void _resetMonitors(int);

			//	protected vars
			int				_numRecHits[constants::SUBDET_NUM];
			edm::InputTag	_tagHBHE;
			edm::InputTag	_tagHO;
			edm::InputTag	_tagHF;

			//	HcalElectronicsMap
			HcalElectronicsMap const *_emap;

			Container1D		_cEnergy_SubDet;
			Container1D		_cEnergy_iphi;
			Container1D		_cEnergy_ieta;
			Container1D		_cEnergy_Crate;
			Container1D		_cTime_SubDet;
			Container1D		_cTime_SubDet_iphi;
			Container1D		_cTime_SubDet_ieta;
			Container1D		_cTime_Crate;
			Container1D		_cTime_Crate_Slot;
			Container1D		_cEnergy_SubDet_ieta;
			Container1D		_cEnergy_SubDet_iphi;

			ContainerProf1D _cEnergyieta_SubDet;
			ContainerProf1D _cEnergyiphi_SubDet;

			Container2D		_cOccupancy_depth;
			Container2D		_cOccupancy_Crate;
			Container2D		_cEnergyTime_SubDet;

			ContainerProf2D	_cEnergy2D_depth;
			
			ContainerSingle2D _cOccupancy_SubDet_Crate;
			ContainerSingle2D _cOccupancy_Crate_Slot;
	};

#endif




