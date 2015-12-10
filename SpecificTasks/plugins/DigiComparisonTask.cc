
#include "DQM/SpecificTasks/interface/DigiComparisonTask.h"

using namespace hcaldqm;
DigiComparisonTask::DigiComparisonTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags and tokens
	_tagHBHE1 = ps.getUntrackedParameter<edm::InputTag>("tagHBHE1",
		edm::InputTag("hcalDigis"));
	_tagHBHE2 = ps.getUntrackedParameter<edm::InputTag>("tagHBHE2",
		edm::InputTag("utcaDigis"));
	_tokHBHE1 = consumes<HBHEDigiCollection>(_tagHBHE1);
	_tokHBHE2 = consumes<HBHEDigiCollection>(_tagHBHE2);

	char aux[20];
	for  (unsigned int i=0; i<10; i++)
	{
		sprintf(aux, "_TS%d", i);
		_cADC_SubDet[i].initialize(_name+"/ADC/SubDet", 
			"ADC" + std::string(aux),
			mapper::fSubDet, 
			new axis::ValueAxis(axis::fXaxis, axis::fADC),
			new axis::ValueAxis(axis::fYaxis, axis::fADC));
	}
	_cMsm_depth.initialize(_name+"/Mismatches/depth",
		"Mismatched", mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries));
	_cMsn_depth.initialize(_name+"/Missing/depth",
		"Missing", mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries));
}

/* virtual */ void DigiComparisonTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	
	for (unsigned int i=0; i<10; i++)
		_cADC_SubDet[i].book(ib, _subsystem);
	_cMsm_depth.book(ib);
	_cMsn_depth.book(ib);
}

/* virtual */ void DigiComparisonTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void DigiComparisonTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>	chbhe1;
	edm::Handle<HBHEDigiCollection>	chbhe2;
	
	if (!e.getByToken(_tokHBHE1, chbhe1))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE1.label() + " " + _tagHBHE1.instance());
	if (!e.getByToken(_tokHBHE2, chbhe2))
		_logger.dqmthrow("Collection HBHEDigiCollection isn't available"
			+ _tagHBHE2.label() + " " + _tagHBHE2.instance());

	std::cout << "1111111111" << std::endl;
	for (HBHEDigiCollection::const_iterator it1=chbhe1->begin();
		it1!=chbhe1->end(); ++it1)
	{
		HcalDetId did = it1->id();
		std::cout << did.iphi() << "  " << did.ieta() << std::endl;
		HBHEDigiCollection::const_iterator it2 = chbhe2->find(did);
		if (it2==chbhe2->end())
			_cMsn_depth.fill(did);
		else
			for (int i=0; i<it1->size(); i++)
			{
				_cADC_SubDet[i].fill(did, double(it1->sample(i).adc()),
					double(it2->sample(i).adc()));
				if (it1->sample(i).adc()!=it2->sample(i).adc())
					_cMsm_depth.fill(did);
			}
	}
}

/* virtual */ void DigiComparisonTask::endLuminosityBlock(
	edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	//	in the end always
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(DigiComparisonTask);

