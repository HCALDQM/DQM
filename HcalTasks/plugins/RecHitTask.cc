
#include "DQM/HcalTasks/interface/RecHitTask.h"

	using namespace hcaldqm;
	RecHitTask::RecHitTask(edm::ParameterSet const& ps) :
		DQTask(ps),

		//	List all the Containers to use
		_cEnergy(_name, "Energy", mapper::fSubDet, 
			axis::fEnergy, axis::fEntries),
		_cEnergy_iphi(_name+"/Energy_iphi", "Energy", mapper::fiphi,
			axis::fEnergy, axis::fEntries),
		_cEnergy_ieta(_name+"/Energy_ieta", "Energy", mapper::fieta,
			axis::fEnergy, axis::fEntries),
		_cEnergy_Crate(_name+"/Energy_Crate", "Energy", mapper::fCrate,
			axis::fEnergy, axis::fEntries),
		_cTime_SubDet(_name+"/Time_SubDet", "Time", 
			mapper::fSubDet, axis::fTime, axis::fEntries),
		_cTime_SubDet_iphi(_name+"/Time_SubDet_iphi", "Time", 
			mapper::fSubDet_iphi, axis::fTime, axis::fEntries),
		_cTime_SubDet_ieta(_name+"/Time_SubDet_ieta", "Time", 
			mapper::fSubDet_ieta, axis::fTime, axis::fEntries),
		_cTime_Crate(_name+"/Time_Crate", "Time", mapper::fCrate,
			axis::fTime, axis::fEntries),
		_cTime_Crate_Slot(_name+"/Time_Crate_Slot", "Time", mapper::fCrate_Slot,
			axis::fTime, axis::fEntries),
		_cEnergy_SubDet_ieta(_name+"/Energy_SubDet_ieta", "Energy",
			mapper::fSubDet_ieta, axis::fEnergy, axis::fEntries),
		_cEnergy_SubDet_iphi(_name+"/Energy", "Energy", mapper::fSubDet_iphi,
			axis::fEnergy, axis::fEntries),
		_cEnergyieta_SubDet(_name+"/EnergyVSieta_SubDet", "EnergyVSieta",
			mapper::fSubDet, axis::fieta, axis::fEnergy),
		_cEnergyiphi_SubDet(_name+"/EnergyVSiphi_SubDet", "EnergyVSiphi",
			mapper::fSubDet, axis::fiphi, axis::fEnergy),
		_cOccupancy_depth(_name, "Occupancy", mapper::fdepth,
			axis::fieta, axis::fiphi),
		_cOccupancy_Crate(_name+"/Occupancy_Crate", "Occupancy", 
			mapper::fCrate, axis::fSlot, axis::fFiber),
		_cEnergyTime_SubDet(_name+"/EnergyVSTime_SubDet", "EnergyVSTime", 
			mapper::fSubDet, axis::fTime, axis::fEnergy),
		_cEnergy2D_depth(_name+"/Energy2D_depth", "Energy",
			mapper::fdepth, axis::fieta, axis::fiphi)

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
		DQTask::bookHistograms(ib, r, es);

		edm::ESHandle<HcalDbService> dbr;
		es.get<HcalDbRecord>().get(dbr);
		_emap = dbr->getHcalMapping();

		_cEnergy.book(ib);
		_cEnergy_SubDet_iphi.book(ib);
		_cEnergy_iphi.book(ib);
		_cEnergy_ieta.book(ib);
		_cEnergy_Crate.book(ib);
		_cTime_SubDet_iphi.book(ib);
		_cTime_SubDet_ieta.book(ib);
		_cTime_Crate.book(ib);
		_cTime_Crate_Slot.book(ib);
		_cTime_SubDet.book(ib);
		_cEnergy_SubDet_ieta.book(ib);
		_cEnergyieta_SubDet.book(ib);
		_cEnergyiphi_SubDet.book(ib);
		_cOccupancy_depth.book(ib);
		_cOccupancy_Crate.book(ib);
		_cEnergyTime_SubDet.book(ib);
		_cEnergy2D_depth.book(ib);
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
			double time = rh.time();
			_cEnergy.fill(rh.id(), energy);
			_cOccupancy_depth.fill(rh.id());
			_cEnergy_SubDet_iphi.fill(rh.id(), energy);
			_cEnergy_iphi.fill(rh.id(), energy);
			_cEnergy_ieta.fill(rh.id(), energy);
			_cEnergy_Crate.fill(_emap->lookup(rh.id()), energy);
			_cTime_Crate.fill(_emap->lookup(rh.id()), time);
			_cTime_Crate_Slot.fill(_emap->lookup(rh.id()), time);
			_cEnergy_SubDet_ieta.fill(rh.id(), energy);
			_cTime_SubDet_iphi.fill(rh.id(), time);
			_cTime_SubDet_ieta.fill(rh.id(), time);
			_cTime_SubDet.fill(rh.id(), time);
			_cEnergyieta_SubDet.fill(rh.id(), energy);
			_cEnergyiphi_SubDet.fill(rh.id(), energy);
			_cOccupancy_Crate.fill(_emap->lookup(rh.id()));
			_cEnergyTime_SubDet.fill(rh.id(), time, energy);
			_cEnergy2D_depth.fill(rh.id(), energy);
		}
		for (HORecHitCollection::const_iterator it=cho->begin();
			it!=cho->end(); ++it)
		{
			const HORecHit rh = (const HORecHit)(*it);
			double energy = rh.energy();
			double time = rh.time();
			_cEnergy.fill(rh.id(), energy);
			_cOccupancy_depth.fill(rh.id());
			_cEnergy_SubDet_iphi.fill(rh.id(), energy);
			_cEnergy_iphi.fill(rh.id(), energy);
			_cEnergy_ieta.fill(rh.id(), energy);
			_cEnergy_Crate.fill(_emap->lookup(rh.id()), energy);
			_cTime_Crate.fill(_emap->lookup(rh.id()), time);
			_cTime_Crate_Slot.fill(_emap->lookup(rh.id()), time);
			_cEnergy_SubDet_ieta.fill(rh.id(), energy);
			_cTime_SubDet_iphi.fill(rh.id(), time);
			_cTime_SubDet_ieta.fill(rh.id(), time);
			_cTime_SubDet.fill(rh.id(), time);
			_cEnergyieta_SubDet.fill(rh.id(), energy);
			_cEnergyiphi_SubDet.fill(rh.id(), energy);
			_cOccupancy_Crate.fill(_emap->lookup(rh.id()));
			_cEnergyTime_SubDet.fill(rh.id(), time, energy);
			_cEnergy2D_depth.fill(rh.id(), energy);
		}
		for (HFRecHitCollection::const_iterator it=chf->begin();
			it!=chf->end(); ++it)
		{
			const HFRecHit rh = (const HFRecHit)(*it);
			double energy = rh.energy();
			double time = rh.time();
			_cEnergy.fill(rh.id(), energy);
			_cOccupancy_depth.fill(rh.id());
			_cEnergy_SubDet_iphi.fill(rh.id(), energy);
			_cEnergy_iphi.fill(rh.id(), energy);
			_cEnergy_ieta.fill(rh.id(), energy);
			_cEnergy_Crate.fill(_emap->lookup(rh.id()), energy);
			_cTime_Crate.fill(_emap->lookup(rh.id()), time);
			_cTime_Crate_Slot.fill(_emap->lookup(rh.id()), time);
			_cEnergy_SubDet_ieta.fill(rh.id(), energy);
			_cTime_SubDet_iphi.fill(rh.id(), time);
			_cTime_SubDet_ieta.fill(rh.id(), time);
			_cTime_SubDet.fill(rh.id(), time);
			_cEnergyieta_SubDet.fill(rh.id(), energy);
			_cEnergyiphi_SubDet.fill(rh.id(), energy);
			_cOccupancy_Crate.fill(_emap->lookup(rh.id()));
			_cEnergyTime_SubDet.fill(rh.id(), time, energy);
			_cEnergy2D_depth.fill(rh.id(), energy);
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




