#ifndef RecHitTask_h
#define RecHitTask_h

/*
 *	file:		RecHitTask.h
 *	Author:		Viktor Khristenko
 *	Date:		13.10.2015
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Container1D.h"

namespace hcaldqm
{
	using namespace constants;
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
			int				_numRecHits[SUBDET_NUM];
			edm::InputTag	_tagHBHE;
			edm::InputTag	_tagHO;
			edm::InputTag	_tagHF;

			Container1D		_cEnergy;
	};
}

#endif




