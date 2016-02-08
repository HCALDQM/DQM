
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
	_cEnergy_Subdet.initialize(_name, "Energy",hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fEnergy),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTiming_SubdetPMiphi.initialize(_name, "Timing",
		hashfunctions::fSubdetPMiphi,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN, true));
	_cEnergyvsiphi_Subdetieta.initialize(_name, "Energy", 
		hashfunctions::fSubdetieta,
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fEnergy));
	_cEnergy_depth.initialize(_name, "Energy", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fEnergy));
	_cTiming_depth.initialize(_name, hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fTiming_ns));
	_cTiming_FEDSlot.initialize(_name, hashfunctions::fFEDSlot,
		new quantity::ValueQuantity(quantity::fTiming_ns),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cEnergy_CrateSpigot.initialize(_name, hashfunctions::fCrateSpigot,
		new quantity::ValueQuantity(quantity::fEnergy),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cEnergy_FED.initialize(_name, hashfunctions::fFED,
		new quantity::ValueQuantity(quantity::fEnergy),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cEt_TTSubdetPM.initialize(_name,"Et", hashfunctions::fTTSubdetPM,
		new quantity::ValueQuantity(quantity::fEt_256),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cEt_TTSubdetPMiphi.initialize(_name, "Et", hashfunctions::fTTSubdetPMiphi,
		new quantity::ValueQuantity(quantity::fEt_256),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cEt_TTSubdetieta.initialize(_name,"Et", hashfunctions::fTTSubdetieta,
		new quantity::ValueQuantity(quantity::fEt_256),
		new quantity::ValueQuantity(quantity::fN), 10);
	_cSummary.initialize(_name, "Summary",
		new quantity::ElectronicsQuantity(quantity::fFED),
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ValueQuantity(quantity::fN));
	_cPerformance.initialize(_name, "Performance",
		new quantity::ValueQuantity(quantity::fLS),
		new quantity::ValueQuantity(quantity::fEvents));
	_cTiming_FEDuTCA.initialize(_name, "Timing",
		hashfunctions::fFED,
		new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
		new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
		new quantity::ValueQuantity(quantity::fTiming_ns));

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
	std::vector<int> vFEDs = utilities::getFEDList(_emap);
	for (std::vector<int>::const_iterator it=vFEDs.begin(); it!=vFEDs.end();
		++it)
		std::cout << "FED: " << *it << std::endl;

	std::vector<uint32_t> vVME;
	vVME.push_back(
		HcalElectronicsId(constants::FIBERCH_MIN, constants::FIBER_VME_MIN,
		SPIGOT_MIN, CRATE_VME_MIN).rawId());
	filter_Electronics.initialize(filter::fFilter, hashfunctions::fElectronics,
		vVME);

	_cEnergy_Subdet.book(ib, _emap);
	_cTiming_SubdetPMiphi.book(ib, _emap);
	_cEnergyvsiphi_Subdetieta.book(ib, _emap);
	_cEnergy_depth.book(ib, _emap);
	_cTiming_depth.book(ib, _emap);
	_cTiming_FEDuTCA.book(ib, _emap, filter_Electronics);
	_cTiming_FEDSlot.book(ib, _emap);
	_cEnergy_CrateSpigot.book(ib, _emap);
	_cEnergy_FED.book(ib, _emap);
	_cEt_TTSubdetPM.book(ib, _emap);
	_cEt_TTSubdetPMiphi.book(ib, _emap);
	_cEt_TTSubdetieta.book(ib, _emap);
	_cSummary.book(ib);
	_cPerformance.book(ib);

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
	_cPerformance.fill(_currentLS, _evsPerLS);

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

/* virtual */ void TestTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

DEFINE_FWK_MODULE(TestTask);
