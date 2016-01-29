
#include "DQM/TestTasks/interface/PedestalTask.h"

using namespace hcaldqm;
PedestalTask::PedestalTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	Containers
	_cPedestalMeans_Subdet.initialize(_name, "Mean",hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::fADC_15),
		new quantity::ValueQuantity(quantity::fN));
	_cPedestalRMSs_Subdet.initialize(_name, "RMS", hashfunctions::fSubdet, 
		new quantity::ValueQuantity(quantity::fADC_5),
		new quantity::ValueQuantity(quantity::fN));
	_cPedestalMeans_depth.initialize(_name, "Mean", hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fADC_15));
	_cPedestalRMSs_depth.initialize(_name, "RMS", hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fADC_5));
	_cPeds.initialize(hashfunctions::fChannel);
	_cPedRef.initialize(hashfunctions::fChannel, compact::fMeanRMS2, 10);
	_cMeanRef_Subdet.initialize(_name, "MeanRef", hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fAroundZero),
		new quantity::ValueQuantity(quantity::fN));
	_cRMSRef_Subdet.initialize(_name, "RMSRef", hashfunctions::fSubdet,
		new quantity::ValueQuantity(quantity::fAroundZero),
		new quantity::ValueQuantity(quantity::fN));
	_cMeanRef_depth.initialize(_name, "MeanRef", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fAroundZero));
	_cRMSRef_depth.initialize(_name, "RMSRef", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fAroundZero));
	_cAbsent_depth.initialize(_name, "Absent", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));
	_cBad_depth.initialize(_name, "Bad", hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fN));


	//	tags
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
	_tagTrigger = ps.getUntrackedParameter<edm::InputTag>("tagTrigger",
		edm::InputTag("tbunpacker"));
	_tokHBHE = consumes<HBHEDigiCollection>(_tagHBHE);
	_tokHO = consumes<HODigiCollection>(_tagHO);
	_tokHF = consumes<HFDigiCollection>(_tagHF);
	_tokTrigger = consumes<HcalTBTriggerData>(_tagTrigger);
}

/* virtual */ void PedestalTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	std::cout << "RUN NUMBER: " << r.runAuxiliary().run() << std::endl;
	if (r.runAuxiliary().run()==1)
		return;
	DQTask::bookHistograms(ib, r, es);

	edm::ESHandle<HcalDbService> dbService;
	es.get<HcalDbRecord>().get(dbService);
	_emap = dbService->getHcalMapping();

	_cPedestalMeans_Subdet.book(ib, _emap);
	_cPedestalRMSs_Subdet.book(ib, _emap);
	_cPedestalMeans_depth.book(ib, _emap);
	_cPedestalRMSs_depth.book(ib, _emap);
	_cPeds.book(_emap);
	_cPedRef.book(_emap);
	_cMeanRef_Subdet.book(ib, _emap);
	_cRMSRef_Subdet.book(ib, _emap);
	_cMeanRef_depth.book(ib, _emap);
	_cRMSRef_depth.book(ib, _emap);
	_cAbsent_depth.book(ib, _emap);
	_cBad_depth.book(ib, _emap);

	//	for testing purposes....
	_cPedestalMeans_Subdet.print();
	_cPedestalRMSs_Subdet.print();
	_cPedestalMeans_depth.print();
	_cPedestalRMSs_depth.print();
}

/* virtual */ void PedestalTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void PedestalTask::endRun(edm::Run const& r, 
	edm::EventSetup const&)
{	
	if (r.runAuxiliary().run()==1)
		return;

	this->_dump();

	/////////
	DQMStore *store = edm::Service<DQMStore>().operator->();
	store->showDirStructure();
	std::cout <<"CURRENT DIRECTORY: "<< store->pwd() << std::endl;
	store->open("DQM_V0001_Hcal_R000262665.root",
		false, "", "HCAL", DQMStore::StripRunDirs);
//	store->load("DQM_V0001_Hcal_R000262665.root");
//	store->setCurrentFolder("./Run 262665");
	store->showDirStructure();
//	store->cd("Run ");
	std::cout <<"11111111 CURRENT DIRECTORY: "<< store->pwd() << std::endl;
//	store->cd("../");
	std::cout <<"2222222 CURRENT DIRECTORY: "<< store->pwd() << std::endl;
//	sprintf(cutstr, "_sumQHBHE%dHO%dHF%d", int(20),
//		int(20), int(20));
	Container2D	cMeans;
	Container2D cRMSs;
	cMeans.initialize(_name, "Mean", hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fADC_15));
	cRMSs.initialize(_name, "RMS", hashfunctions::fdepth, 
		new quantity::DetectorQuantity(quantity::fieta), 
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::ValueQuantity(quantity::fADC_5));
	cMeans.load(store, _emap);
	cRMSs.load(store, _emap);
	cMeans.print();
	cRMSs.print();
	std::cout << "SIZE=" << _cPedRef.size() << std::endl;
	_cPedRef.print();
	_cPedRef.load(&cMeans);
	_cPedRef.print();
	_cPedRef.compare(_cPeds, &_cMeanRef_Subdet, &_cAbsent_depth,
		&_cBad_depth, &hcaldqm::diff, &hcaldqm::pedestal_quality);
	_cPedRef.compare(_cPeds, &_cMeanRef_depth, &_cAbsent_depth,
		&_cBad_depth, &hcaldqm::diff, &hcaldqm::pedestal_quality);
//	_cPeds.compare(_cPedRef, &_cMeanRef_Subdet);
//	_cPeds.compare(_cPedRef, &_cMeanRef_depth);
	_cPedRef.print();
}

/* virtual */ void PedestalTask::_dump()
{
	_cPedestalMeans_Subdet.reset();
	_cPedestalRMSs_Subdet.reset();
	_cPedestalMeans_depth.reset();
	_cPedestalRMSs_depth.reset();
	_cPeds.dump(&_cPedestalMeans_Subdet, true);
	_cPeds.dump(&_cPedestalRMSs_Subdet, false);
	_cPeds.dump(&_cPedestalMeans_depth, true);
	_cPeds.dump(&_cPedestalRMSs_depth, false);
}

/* virtual */ void PedestalTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>		chbhe;
	edm::Handle<HODigiCollection>		cho;
	edm::Handle<HFDigiCollection>		chf;

	if (!e.getByToken(_tokHBHE, chbhe))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByToken(_tokHO, cho))
		_logger.dqmthrow("Collection HODigiCollection isn't available"
			+ _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByToken(_tokHF, chf))
		_logger.dqmthrow("Collection HFDigiCollection isn't available"
			+ _tagHF.label() + " " + _tagHF.instance());

	for (HBHEDigiCollection::const_iterator it=chbhe->begin();
		it!=chbhe->end(); ++it)
	{
		const HBHEDataFrame digi = (const HBHEDataFrame)(*it);
		HcalDetId did = digi.id();
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		for (int i=0; i<digiSizeToUse; i++)
		{
			_cPeds.fill(did, it->sample(i).adc());
		}
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		HcalDetId did = digi.id();
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		for (int i=0; i<digiSizeToUse; i++)
		{
			_cPeds.fill(did, it->sample(i).adc());
		}
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		HcalDetId did = digi.id();
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		for (int i=0; i<digiSizeToUse; i++)
		{
			_cPeds.fill(did, it->sample(i).adc());
		}
	}

	if (_ptype==fOnline && _evsTotal>0 && 
		_evsTotal%constants::CALIBEVENTS_MIN==0)
		this->_dump();
}

/* virtual */ bool PedestalTask::_isApplicable(edm::Event const& e)
{
	if (_ptype==fOnline)
	{
		//	online-global
		return this->_getCalibType(e)==hc_Pedestal;
	}
	else 
	{
		//	local
		edm::Handle<HcalTBTriggerData>	ctrigger;
		if (!e.getByToken(_tokTrigger, ctrigger))
			_logger.dqmthrow("Collection HcalTBTriggerData isn't available"
				+ _tagTrigger.label() + " " + _tagTrigger.instance());
		return ctrigger->wasSpillIgnorantPedestalTrigger();
	}

	return false;
}

DEFINE_FWK_MODULE(PedestalTask);


