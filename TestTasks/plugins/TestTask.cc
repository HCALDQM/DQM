
#include "DQM/TestTasks/interface/TestTask.h"

using namespace hcaldqm;
TestTask::TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	/*
	char name[200];
	for (unsigned int i=0; i<_cDetIds.size(); i++)
	{
		sprintf(name, "_iphi%d_ieta%d", _cDetIds[i].iphi(), _cDetIds[i].ieta());
		_vcShape.push_back(ContainerSingle1D(_name+"/Shape",
			"Shape"+std::string(name), 
			new axis::ValueAxis(axis::fXaxis, fTimeTS),
			new axis::ValueAxis(axis::fYaxis, fNomFC)));
		for (unsigned int j=0; j<10; j++)
		{
			char aux[10];
			sprintf(aux, "_TS%d", j);
			_vcLETDCvsADC[j].push_back(ContainerSingle2D(_name+"/LETDCvsADC",
				"LETDCvsADC"+std::string(name)+std::string(aux),
				new axis::ValueAxis(axis::fXaxis, axis::fQIE10ADC256),
				new axis::ValueAxis(axis::fYaxis, axis::fQIE10TDC64),
				new axis::ValueAxis(axis::fZaxis, axis::fEntries)));
			_vcTETDCvsADC[j].push_back(ContainerSingle2D(_name+"/TETDCvsADC",
				"TETDCvsADC"+std::string(name)+std::string(aux),
				new axis::ValueAxis(axis::fXaxis, axis::fQIE10ADC256),
				new axis::ValueAxis(axis::fYaxis, axis::fQIE10TDC64),
				new axis::ValueAxis(axis::fZaxis, axis::fEntries)));
			_vcLETDC[j].push_back(ContainerSingle1D(_name+"/LETDC",
				"LETDC"+std::string(name)+std::string(aux),
				new axis::ValueAxis(axis::fXaxis, axis::fQIE10TDC64),
				new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)));
		}
	}
*/
	_cTest.initialize(_name, hashfunctions::fSubdet,
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::DetectorQuantity(quantity::fieta));
	_cTest1.initialize(_name, hashfunctions::fSubdetPMiphi,
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::DetectorQuantity(quantity::fieta));

	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);

	edm::ESHandle<HcalDbService> dbService;
	es.get<HcalDbRecord>().get(dbService);
	_emap = dbService->getHcalMapping();
	_cTest.book(ib, _emap);
	_cTest1.book(ib, _emap);

/*
	for (unsigned int i=0; i<_cDetIds.size(); i++)
	{
		_vcShape[i].book(ib, _subsystem);
		for (unsigned int j=0; j<10; j++)
		{
			_vcLETDCvsADC[j][i].book(ib, _subsystem);
			_vcTETDCvsADC[j][i].book(ib, _subsystem);
			_vcLETDC[j][i].book(ib, _subsystem);
		}
	}
	*/
}

/* virtual */ void TestTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{

	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void TestTask::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
//	edm::Handle<QIE10DigiCollection> cqie10;
//	if (!e.getByLabel(_tagHF, cqie10))
//		std::cout << "Collection isn't availalbe" << std::endl;

}

/* virtual */ void TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(TestTask);
