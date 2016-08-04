
#include "DQM/HcalTasks/interface/UMNioTask.h"

using namespace hcaldqm;
UMNioTask::UMNioTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	for (uint32_t type=constants::tNull;
		type<constants::nOrbitGapType; type++)
	{
		std::cout << "CURRENT TYPE = "<< (unsigned)type << std::endl;
		_eventtypes.push_back(type);
	}
	_taguMN = ps.getUntrackedParameter<edm::InputTag>("taguMN",
		edm::InputTag("hcalDigis"));
	_tokuMN = consumes<HcalUMNioDigi>(_taguMN);
}
	
/* virtual */ void UMNioTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	if (_ptype==fLocal)
		if (r.runAuxiliary().run()==1)
			return;

	DQTask::bookHistograms(ib, r, es);
	
	edm::ESHandle<HcalDbService> dbService;
	es.get<HcalDbRecord>().get(dbService);
	_emap = dbService->getHcalMapping();

	_cEventType.initialize(_name, "EventType",
		new quantity::LumiSection(_maxLS),
		new quantity::EventType(_eventtypes),
		new quantity::ValueQuantity(quantity::fN));
	_cEventType.book(ib, _subsystem);
}

/* virtual */ void UMNioTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HcalUMNioDigi> cumn;
	if (!e.getByToken(_tokuMN, cumn))
		std::cout << "Collection HcalUMNioDigi is not found" << std::endl;

	uint32_t eventtype = cumn->valueUserWord(0);
	_cEventType.fill(_currentLS, (int)eventtype);
}
/* virtual */ void UMNioTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(UMNioTask);
