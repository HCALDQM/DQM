#ifndef DQTask_h
#define DQTask_h

/*
 *	file:		DQTask.h
 *	Author:		VK
 *	Date:		13.10.2015
 */

#include "DQM/HcalCommon/interface/DQModule.h"
#include "DQM/HcalCommon/interface/ContainerI.h"

namespace hcaldqm
{
	class DQTask : public DQMEDAnalyzer, public DQModule
	{
		public:
			DQTask(edm::ParameterSet const&);
			virtual ~DQTask(){}

			//	base inheritance to override from DQMEDAnalyzer
			virtual void analyze(edm::Event const&, edm::EventSetup const&);
			virtual void bookHistograms(DQMStore::IBooker &, edm::Run const&,
				edm::EventSetup const&);
			virtual void dqmBeginRun(edm::Run const&, edm::EventSetup const&);
			virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
				edm::EventSetup const&);
			virtual void endLuminosityBlock(edm::LuminosityBlock const&,
				edm::EventSetup const&);

		protected:
			// protected funcs
			virtual void _resetMonitors(int);
			virtual void _process(edm::Event const&, edm::EventSetup const&) = 0;
			virtual void _isApplicable(edm::Event const&) {return true;}
			virtual int _getCalibType(edm::Event const&);

			//	protected vars
			ContainerI _cEvsTotal;
			ContainerI _cEvsPerLS;

			int			_evsTotal, _evsPerLS;

			edm::InputTag		_tagRaw;
	};
}

#endif





