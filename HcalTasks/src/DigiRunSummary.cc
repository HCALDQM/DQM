#include "DQM/HcalTasks/interface/DigiRunSummary.h"

namespace hcaldqm
{
	DigiRunSummary::DigiRunSummary(std::string const& name, 
		edm::ParameterSet const& ps) :
		DQClient(name, ps)
	{}

	/* virtual */ void DigiRunSummary::beginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		DQClient::beginRun(r,es);
	}

	/* virtual */ void DigiRunSummary::endLuminosityBlock(DQMStore::IBooker& ib,
		DQMStore::IGetter& ig, edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		DQClient::endLuminosityBlock(ib, ig, lb, es);
	}

	/* virtual */ std::vector<flag::Flag> DigiRunSummary::endJob(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig)
	{
		//	FILTERS, some useful vectors, hash maps
		std::vector<uint32_t> vhashFEDHF;
		vhashFEDHF.push_back(HcalElectronicsId(22, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		vhashFEDHF.push_back(HcalElectronicsId(29, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		vhashFEDHF.push_back(HcalElectronicsId(32, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		filter::HashFilter filter_FEDHF;
		filter_FEDHF.initialize(filter::fPreserver, hashfunctions::fFED,
			vhashFEDHF);	// preserve only HF FEDs
		electronicsmap::ElectronicsMap ehashmap;
		ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);

		// INITIALIZE 
		Container2D cOccupancy_depth;
		Container1D cDigiSize_FED;
		Container2D cOccupancyCut_depth;
		ContainerXXX<double> xDead, xDigiSize, xUniHF, xUni; 
		xDead.initialize(hashfunctions::fFED);
		xDigiSize.initialize(hashfunctions::fFED);
		xUni.initialize(hashfunctions::fFED);
		xUniHF.initialize(hashfunctions::fFEDSlot);
		cOccupancy_depth.initialize(_taskname, "Occupancy",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN));
		cOccupancyCut_depth.initialize(_taskname, "OccupancyCut",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN));
		_cDigiSize_FED.initialize(_name, "DigiSize",
			hashfunctions::fFED,
			new quantity::ValueQuantity(quantity::fDigiSize),
			new quantity::ValueQuantity(quantity::fN));

		//	BOOK
		xDead.book(_emap); xDigiSize.book(_emap); xUniHF.book(_emap);
		xUni.book(_emap, _filter_FEDHF);

		//	LOAD
		cOccupancy_depth.load(ig, _emap, _subsystem);
		cOccupancyCut_depth.load(ig, _emap, _subsystem);
		cDigiSize_FED.load(ig, _emap, _subsystem);

		//	iterate over all channels
		std::vector<HcalGenericDetId> gids = _emap.allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;

			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(ehashmap.lookup(did));

			cOccupancy_depth.getBinContent(did)<1?
				xDead.get(eid)++:xDead.get(eid)+=0;
			if (did.subdet()==HcalForward)
				xUniHF.get(eid)+=cOccupancyCut_depth(did);
			_cDigiSize_FED.getMean(eid)!=
				constants::DIGISIZE[did.subdet()-1]?
				xDigiSize.get(eid)++:xDigiSize.get(eid)+=0;
			_cDigiSize_FED.getRMS(eid)!=0?
				xDigiSize.get(eid)++:xDigiSize.get(eid)+=0;
		}

		//	iterate over all slots in HF
		for (doubleCompactMap::const_iterator it=xUniHF.begin();
			it!=xUniHF.end(); ++it)
		{
			uint32_t hash1 = it->first;
			HcalElectronicsId eid1(hash1);
			double x1 = it->second;

			for (doubleCompactMap::const_iterator jt=xUniHF.begin();
				jt!=xUniHF.end(); ++jt)
			{
				if (jt==it)
					continue;

				uint32_t hash2 = jt->first;
				double x2 = jt->second;
				if (x2==0)
					continue;
				if (x1/x2<0.2)
					xUni.get(eid)++;
			}
		}

		//	iterate over all FEDs
		std::vector<flag::Flag> sumflags;
		for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
			it!=_vhashFEDs.end(); ++it)
		{
			flag::Flag fSum("DIGI");
			flag::Flag fDead("Dead");
			flag::Flag fUni("UniSlotHF");
			flag::Flag fDigiSize("DigiSize");
			HcalElectronicsId eid(*it);

			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(); *it);
			if (cit==_vcdaqEids.end())
			{
				//	not registered @cDAQ
				sumflags.push_back(fSum);
				continue;
			}

			//	registered @cDAQ
			if (xDead.get(eid)>0)
				fDead._state = state::fBAD;
			else
				fDead._state = state::fGOOD;
			if (xUni.get(eid)>0)
				fUni._state = state::fBAD;
			else
				fUni._state = state::fGOOD;
			if (xDigiSize.get(eid)>0)
				fDigiSize._state = state::fBAD;
			else
				fDigiSize._state = state::fGOOD;

			fSum = fDead+fUni+fDigiSize;
			sumflags.push_back(fSum);
		}

		return sumflags;
	}
}
