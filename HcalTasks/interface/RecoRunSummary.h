#ifndef DQM_HcalTasks_RecoRunSummary_h
#define DQM_HcalTasks_RecoRunSummary_h

#include "DQM/HcalCommon/interface/DQClient.h"

namespace hcaldqm
{
	class RecoRunSummary : public DQClient
	{
		public:
			RecoRunSummary(std::string const&, edm::ParameterSet const&);
			virtual ~RecoRunSummary() {}

			virtual void beginRun(edm::Run const&, edm::EventSetup const&);
			virtual void endLuminosityBlock(DQMStore::IBooker&,
				DQMStore::IGetter&, edm::LuminosityBlock const&,
				edm::EventSetup const&);
			virtual std::vector<flag::Flag> endJob(
				DQMStore::IBooker&, DQMStore::IGetter&);

		protected:
	};
}

#endif
