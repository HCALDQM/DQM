#include "DQM/HcalTasks/interface/RecoRunSummary.h"

namespace hcaldqm
{
	RecoRunSummary::RecoRunSummary(std::string const& name, 
		std::string const& taskname, edm::ParameterSet const& ps) :
		DQClient(name, taskname, ps)
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
		bool tcdsshift = false;
		filter::HashFilter filter_VME;
		filter::HashFilter filter_uTCA;
		std::vector<uint32_t> vhashVME,vhashuTCA;
		vhashVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
			constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
		vhashuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
			vhashuTCA);
		filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
			vhashVME);
		std::vector<flag::Flag> vflags; vflags.resize(nRecoFlag);
		vflags[fDead]=flag::Flag("Dead");
		vflags[fUniSlotHF]=flag::Flag("UniSlotHF");
		vflags[fDigiSize]=flag::Flag("DigiSize");

		//	INITIALIZE
		Container2D cOccupancy_depth, cOccupancyCut_depth;
		ContainerSingle2D cSummary;
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

		_cDead_depth.initialize(_name, "Dead",
			hashfunctions::fdepth,
			new quantity::DetectorQuantity(quantity::fieta),
			new quantity::DetectorQuantity(quantity::fiphi),
			new quantity::ValueQuantity(quantity::fN));
		_cDead_FEDVME.initialize(_name, "Dead",
			hashfunctions::fFED,
			new quantity::ElectronicsQuantity(quantity::fSpigot),
			new quantity::ElectronicsQuantity(quantity::fFiberVMEFiberCh),
			new quantity::ValueQuantity(quantity::fN));
		_cDead_FEDuTCA.initialize(_name, "Dead",
			hashfunctions::fFED,
			new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
			new quantity::ElectronicsQuantity(quantity::fFiberuTCAFiberCh),
			new quantity::ValueQuantity(quantity::fN));
		cSummary.initialize(_name, "Summary",
			new quantity::FEDQuantity(_vFEDs),
			new quantity::FlagQuantity(vflags),
			new quantity::ValueQuantity(quantity::fState));

		//	BOOK
		xDead.book(_emap); xUni.book(_emap);
		xUniHF.book(_emap, filter_FEDHF);

		//	LOAD
		cOccupancy_depth.load(ig, _emap, _subsystem);
		cOccupancyCut_depth.load(ig, _emap, _subsystem);
		cTimingCut_HBHEPartition.book(ib, _emap, _subsystem);
		_cDead_depth.book(ib, _emap, _subsystem);
		_cDead_FEDVME.book(ib, _emap, filter_uTCA, _subsystem);
		_cDead_FEDuTCA.book(ib, _emap, filter_VME, _subsystem);
		cSummary.book(ib, _subsystem);

		//	iterate over all channels
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())			
				continue;

			HcalDetId did(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(ehashmap.lookup(did));

			if (cOccupancy_depth.getBinContent(did)<1)
			{
				xDead.get(eid)++;
				_cDead_depth.fill(did);
				eid.isVMEid()?_cDead_FEDVME.fill(eid):_cDead_FEDuTCA.fill(eid);
			}
			else
				xDead.get(eid)+=0;
			if (did.subdet()==HcalForward)
				xUniHF.get(eid)+=cOccupancyCut_depth.getBinContent(did);
		}

		//	iphi/slot HF non uniformity
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

				double x2 = jt->second;
				if (x2==0)
					continue;
				if (x1/x2<0.2)
					xUni.get(eid1)++;
			}
		}
		//	TCDS shift
		double a = cTimingCut_HBHEPartition.getMean(
			HcalDetId(HcalBarrel,1,5,1));
		double b = cTimingCut_HBHEPartition.getMean(
			HcalDetId(HcalBarrel,1,30,1));
		double c = cTimingCut_HBHEPartition.getMean(
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
			HcalElectronicsId eid(*it);

			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(),*it);
			if (cit==_vcdaqEids.end())
			{
				//	not registered @cDAQ
				sumflags.push_back(flag::Flag("RECO", flag::fNCDAQ));
				continue;
			}

			//	registered @cDAQ
			if (utilities::isFEDHBHE(eid))
			{
				if (tcdsshift)
					vflags[fTCDS]._state = flag::fBAD;
				else
					vflags[fTCDS]._state = flag::fGOOD;
			}
			if (utilities::isFEDHF(eid))
			{
				if (xUni.get(eid)>0)
					vflags[fUniSlotHF]._state = flag::fBAD;
				else
					vflags[fUniSlotHF]._state = flag::fGOOD;
			}
			if (xDead.get(eid)>0)
				vflags[fDead]._state = flag::fBAD;
			else
				vflags[fDead]._state = flag::fGOOD;

			//	combine
			int iflag=0;
			for (std::vector<flag::Flag>::iterator ft=vflags.begin();
				ft!=vflags.end(); ++ft)
			{
				cSummary.setBinContent(eid, iflag, ft->_state);
				fSum+=(*ft);
				ft->reset();
			}
				sumflags.push_back(fSum);
		}

		return sumflags;
	}
}
