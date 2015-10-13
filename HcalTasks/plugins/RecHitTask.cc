
#include "DQM/HcalTasks/interface/RecHitTask.h"

	using namespace hcaldqm;
	RecHitTask::RecHitTask(edm::ParameterSet const& ps) :
		DQTask(ps),
		_cEnergy("Hcal/RecHitTask", "Energy", mapper::fSubDet, 
			axis::fEnergy, axis::fEntries)
	{
		_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
			edm::InputTag("hbhereco"));
		_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
			edm::InputTag("horeco"));
		_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
			edm::InputTag("hfreco"));
	}

	/* virtual */ void RecHitTask::bookHistograms(DQMStore::IBooker & ib,
		edm::Run const& r, edm::EventSetup const& es)
	{
		_cEnergy.book(ib);
	}

	/* virtual */ void RecHitTask::_process(edm::Event const& e,
		edm::EventSetup const& es)
	{
		edm::Handle<HBHERecHitCollection>	chbhe;
		edm::Handle<HORecHitCollection>		cho;
		edm::Handle<HFRecHitCollection>		chf;

		if (!(e.getByLabel(_tagHBHE, chbhe)))
			this->_throw("Collection HBHERecHitCollection not available",
				" " + _tagHBHE.label() + " " + _tagHBHE.instance());
		if (!(e.getByLabel(_tagHO, cho)))
			this->_throw("Collection HORecHitCollection not available",
				" " + _tagHO.label() + " " + _tagHO.instance());
		if (!(e.getByLabel(_tagHF, chf)))
			this->_throw("Collection HFRecHitCollection not available",
				" " + _tagHF.label() + " " + _tagHF.instance());

		//	Processing
		for (HBHERecHitCollection::const_iterator it=chbhe->begin();
			it!=chbhe->end(); ++it)
		{
			const HBHERecHit rh = (const HBHERecHit)(*it);
			double energy = rh.energy();
			_cEnergy.fill(rh.id(), energy);
		}
		for (HORecHitCollection::const_iterator it=cho->begin();
			it!=cho->end(); ++it)
		{
			const HORecHit rh = (const HORecHit)(*it);
			double energy = rh.energy();
			_cEnergy.fill(rh.id(), energy);
		}
		for (HFRecHitCollection::const_iterator it=chf->begin();
			it!=chf->end(); ++it)
		{
			const HFRecHit rh = (const HFRecHit)(*it);
			double energy = rh.energy();
			_cEnergy.fill(rh.id(), energy);
		}
	}

	/* virtual */ void RecHitTask::_resetMonitors(int pflag)
	{
		DQTask::_resetMonitors(pflag);
		if (pflag==0)
		{
			for (unsigned int i=0; i<constants::SUBDET_NUM; i++)
				_numRecHits[i] = 0;
		}
	}

DEFINE_FWK_MODULE(RecHitTask);




