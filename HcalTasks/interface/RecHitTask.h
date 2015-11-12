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

			//	tags
			edm::InputTag	_tagHBHE;
			edm::InputTag	_tagHO;
			edm::InputTag	_tagHF;

			//	counters
			int	 _numRecHits[constants::SUBDET_NUM];

			//	cuts
			double _cutE_HBHE, _cutE_HO, _cutE_HF;

			//	Energy
			Container1D		_cEnergy_SubDet;
			Container1D		_cEnergy_SubDet_ieta;
			Container1D		_cEnergy_SubDetPM_iphi;
			ContainerProf1D _cEnergyvsieta_SubDet;
			ContainerProf1D _cEnergyvsiphi_SubDet;
			ContainerProf2D	_cEnergy_depth;

			ContainerProf1D _cEnergyvsietaCut_SubDet;
			ContainerProf1D _cEnergyvsiphiCut_SubDet;
			ContainerProf2D	_cEnergyCut_depth;

			//	Timing
			Container1D		_cTimingCut_SubDet;
			Container1D		_cTimingCut_SubDetPM_iphi;
			ContainerProf1D _cTimingCutvsLS_SubDetPM_iphi;
			Container1D		_cTimingCut_SubDet_ieta;
			ContainerProf1D _cTimingvsietaCut_SubDet_iphi;
			ContainerProf1D	_cTimingvsiphiCut_SubDet_ieta;
			ContainerProf2D _cTimingCut_depth;

			//	Occupancy
			Container2D		_cOccupancy_depth;
			Container2D		_cOccupancyCut_depth;

			//	Energy vs Timing
			Container2D		_cEnergyvsTiming_SubDet;
			Container2D		_cEnergyvsTimingCut_SubDet;
	};

#endif




