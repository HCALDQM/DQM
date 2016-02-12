#include "DQM/HcalCommon/interface/DQHarvester.h"

namespace hcaldqm
{
	DQHarvester::DQHarvester(edm::ParameterSet const& ps) :
		DQModule(ps)
	{}

	/* virtual */ void DQHarvester::beginRun(edm::Run const&,
		edm::EventSetup const& es)
	{
		edm::ESHandle<HcalDbService> dbs;
		es.get<HcalDbRecord>().get(dbs);
		_emap = dbs->getHcalMapping();
	}

	/* virtual */ void DQHarvester::dqmEndLuminosityBlock(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig,
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
	{
		_currentLS = lb.luminosityBlock();
		_dqmEndLuminosityBlock(ib, ig, lb, es);
	}
	/* virtual */ void DQHarvester::dqmEndJob(DQMStore::IBooker& ib, 
		DQMStore::IGetter& ig)
	{
		_dqmEndJob(ib, ig);
	}
}
