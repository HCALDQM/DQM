#ifndef DQHarvester_h
#define DQHarvester_h

/*
 *	file:		DQHarvester.h
 *	Author:		VK
 */

#include "DQM/HcalCommon/interface/DQModule.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class DQHarvester : public DQMEDHarvester, public DQModule
	{
		public:
			DQHarvester(edm::ParameterSet const&);
			virtual ~DQHarvester() {}

			virtual void beginRun(edm::Run const&, edm::EventSetup const&);
			virtual void dqmEndLuminosityBlock(
				DQMStore::IBooker&, DQMStore::IGetter&, 
				edm::LuminosityBlock const&, edm::EventSetup const&);
			virtual void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&);

		protected:
			HcalElectronicsMap const* _emap;

			virtual void _dqmEndLuminosityBlock(
				DQMStore::IBooker&, DQMStore::IGetter&, 
				edm::LuminosityBlock const&, edm::EventSetup const&) = 0;
			virtual void _dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) = 0;
	};
}

#endif









