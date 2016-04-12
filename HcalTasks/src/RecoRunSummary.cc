#include "DQM/HcalTasks/interface/RecoRunSummary.h"

namespace hcaldqm
{
	RecoRunSummary::RecoRunSummary(std::string const& name, 
		edm::ParameterSet const& ps) :
		DQClient(name, ps)
	{}

	/* virtual */ void RecoRunSummary::beginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		DQClient::beginRun(r,es);
	}

	/* virtual */ void RecoRunSummary::endLuminosityBlock(DQMStore::IBooker& ib,
		DQMStore::IGetter& ig, edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		DQClient::endLuminosityBlock(ib, ig, lb, es);
	}

	/* virtual */ std::vector<flag::Flag> RecoRunSummary::endJob(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig)
	{
		// FILTERS, some useful vectors, hash maps
		std::vector<uint32_t> vhashFEDHF;
		vhashFEDHF.push_back(HcalElectronicsId(22, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		vhashFEDHF.push_back(HcalElectronicsId(29, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		vhashFEDHF.push_back(HcalElectronicsId(32, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		filter::HashFilter filter_FEDHF;
		filter_FEDHF.initialize(filter::fPreserver, hashfunctions::fFED,
			vhashFEDHF);    // preserve only HF FEDs
		electronicsmap::ElectronicsMap ehashmap;
		ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);

		//	INITIALIZE
		Container2D cOccupancy_depth, cOccupancyCut_depth;
		Container1D cTimingCut_HBHEPartition;
		ContainerXXX<double> xDead, xUniHF, xUni;
		xDead.initialize(hashfunctions::fFED);
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
		cTimingCut_HBHEPartition.initialize(_taskname, "TimingCut",
			hashfunctions::fHBHEPartition,
			new quantity::ValueQuantity(quantity::fTiming_ns),
			new quantity::ValueQuantity(quantity::fN));

		//	BOOK
		xDead.book(_emap); xUni.book(_emap);
		xUniHF.book(_emap, _filter_FEDHF);

		//	LOAD
		cOccupancy_depth.load(ig, _emap, _subsystem);
		cOccupancyCut_depth.load(ig, _emap, _subsystem);
		cTimingCut_HBHEPartition.book(ib, _emap, _subsystem);

		//	iterate over all channels
		std::vector<HcalGenericDetId> gids = _emap.allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())			
				continue;

			HcalDetId did(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(ehashmap.lookup(did));

			cOccupancy_depth.getBinContent(did)<1?
				xDead.get(eid)++:xDead.get(eid)+=0;
			if (did.subdet()==HcalForward)
				xUniHF.get(eid)++cOccupancyCut_depth(did);
		}

		//	iphi/slot HF non uniformity
		for (doubleCompactMap::const_iterator it=xUniHF.begin();
			it!=xUniHF.end(); ++it)
		{
			uint32_t hash = it->first;
			HcalElectronicsId eid1(hash1);
			double x1 = it->second;
			for (doubleCompactMap::const_iterator jt=xUniHF.begin();
				jt!=xUniHF.edn(); ++jt)
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
		//	TCDS shift
		double a = cTimingCut_HBHEPartition.getMean(
			HcalDetId(HcalBarrel,1,5,1));
		double b = cTimingCut_HBHEPartition.getMean(
			HcalDetId(HcalBarrel,1,30,1));
		double x = cTimingCut_HBHEPartition.getMean(
			HcalDetId(HcalBarrel,1,55,1));
		double dab = fabs(a-b);
		double dac = fabs(a-c);
		double dbc = fabs(b-c);
		if (dab>=1.5 || dac>=1.5 || dbc>=1.5)
			tcdsshift = true;

		//	summary flags
		std::vector<flag::Flag> sumflags;
		for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
			it!=_vhashFEDs.end(); ++it)
		{
			flag::Flag fSum("RECO");
			flag::Flag fDead("Dead");
			flag::Flag fUni("HFUniSlot");
			flag::Flag fTCDS("TCDS");
			HcalElectronicsId eid(*it);

			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(),*it);
			if (cit==_vcdaqEids.end())
			{
				//	not registered @cDAQ
				sumflags.push_back(fSum);
				continue
			}

			//	registered @cDAQ
			if (isHBHE(eid))
			{
				if (tcdsshift)
					fTCDS._state = state::fBAD;
				else
					fTCDS._state = state::fGOOD;
			}
			if (isHF(eid))
			{
				if (xUni.get(eid)>0)
					fUni._state = state::fBAD;
				else
					fUni._state = state::fGOOD;
			}
			if (xDead.get(eid)>0)
				fDead._state = state::fBAD;
			else
				fDead._state = state::fGOOD;

			//	combine
			fSum = fDead+fUni+fTCDS;
			sumflags.push_back(fSum);
		}

		return sumflags;
	}
}
