#include "DQM/HcalTasks/interface/RawRunSummary.h"

namespace hcaldqm
{
	RawRunSummary::RawRunSummary(std::string const& name, 
		std::string const& taskname, edm::ParameterSet const& ps) :
		DQClient(name, taskname, ps)
	{}

	/* virtual */ void RawRunSummary::beginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		DQClient::beginRun(r,es);
	}

	/* virtual */ void RawRunSummary::endLuminosityBlock(DQMStore::IBooker& ib,
		DQMStore::IGetter& ig, edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		DQClient::endLuminosityBlock(ib, ig, lb, es);
	}

	/* virtual */ std::vector<flag::Flag> RawRunSummary::endJob(
		DQMStore::IBooker& ib, DQMStore::IGetter& ig)
	{
		//	FILTERS, some useful vectors, hash maps
		std::vector<uint32_t> vhashVME, vhashuTCA;
		std::vector<int> vFEDsVME, vFEDsuTCA;
		vhashVME.push_back(HcalElectronicsId(constants::FIBERCH_MIN,
			constants::FIBER_VME_MIN, SPIGOT_MIN, CRATE_VME_MIN).rawId());
		vhashuTCA.push_back(HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
			FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
		filter::HashFilter filter_VME, filter_uTCA;
		filter_VME.initialize(filter::fFilter, hashfunctions::fElectronics,
			vhashVME);	// filter out VME 
		filter_uTCA.initialize(filter::fFilter, hashfunctions::fElectronics,
			vhashuTCA); // filter out uTCA
		vFEDsVME = utilities::getFEDVMEList(_emap);
		vFEDsuTCA= utilities::getFEDuTCAList(_emap);
		electronicsmap::ElectronicsMap ehashmap;
		ehashmap.initialize(_emap, electronicsmap::fD2EHashMap);
		std::vector<flag::Flag> vflags; vflags.resize(nRawFlag);
		vflags[fEvnMsm]=flag::Flag("EvnMsm");
		vflags[fBcnMsm]=flag::Flag("BcnMsm");
		vflags[fBadQ]=flag::Flag("BadQ");

		//	INITIALIZE CONTAINERS AND VARIABLES
		Container2D cEvnMsm_ElectronicsVME,cBcnMsm_ElectronicsVME;
		Container2D cEvnMsm_ElectronicsuTCA,cBcnMsm_ElectronicsuTCA;
		Container2D cBadQuality_depth;
		ContainerSingle2D cSummary;
		ContainerXXX<double> xEvn,xBcn,xBadQ;
		xEvn.initialize(hashfunctions::fFED);
		xBcn.initialize(hashfunctions::fFED);
		xBadQ.initialize(hashfunctions::fFED);
		cEvnMsm_ElectronicsVME.initialize(_taskname, "EvnMsm",
			hashfunctions::fElectronics,
			new quantity::FEDQuantity(vFEDsVME),
			new quantity::ElectronicsQuantity(quantity::fSpigot),
			new quantity::ValueQuantity(quantity::fN));
		cBcnMsm_ElectronicsVME.initialize(_taskname, "BcnMsm",
			hashfunctions::fElectronics,
			new quantity::FEDQuantity(vFEDsVME),
			new quantity::ElectronicsQuantity(quantity::fSpigot),
			new quantity::ValueQuantity(quantity::fN));
		cEvnMsm_ElectronicsuTCA.initialize(_taskname, "EvnMsm",
			hashfunctions::fElectronics,
			new quantity::FEDQuantity(vFEDsuTCA),
			new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
			new quantity::ValueQuantity(quantity::fN));
		cBcnMsm_ElectronicsuTCA.initialize(_taskname, "BcnMsm",
			hashfunctions::fElectronics,
			new quantity::FEDQuantity(vFEDsuTCA),
			new quantity::ElectronicsQuantity(quantity::fSlotuTCA),
			new quantity::ValueQuantity(quantity::fN));
		cBadQuality_depth.initialize(_taskname, "BadQuality",
			 hashfunctions::fdepth,
			 new quantity::DetectorQuantity(quantity::fieta),
			 new quantity::DetectorQuantity(quantity::fiphi),
			 new quantity::ValueQuantity(quantity::fN));
		cSummary.initialize(_name, "Summary",
			new quantity::FEDQuantity(_vFEDs),
			new quantity::FlagQuantity(vflags),
			new quantity::ValueQuantity(quantity::fState));

		//	BOOK CONTAINERSXXX
		xEvn.book(_emap); xBcn.book(_emap); xBadQ.book(_emap);

		//	LOAD CONTAINERS
		cEvnMsm_ElectronicsVME.load(ig, _emap, filter_uTCA, _subsystem);
		cBcnMsm_ElectronicsVME.load(ig, _emap, filter_uTCA, _subsystem);
		cEvnMsm_ElectronicsuTCA.load(ig, _emap, filter_VME, _subsystem);
		cBcnMsm_ElectronicsuTCA.load(ig, _emap, filter_VME, _subsystem);
		cBadQuality_depth.load(ig, _emap, _subsystem);
		cSummary.book(ib, _subsystem);

		// iterate over all channels	
		std::vector<HcalGenericDetId> gids = _emap->allPrecisionId();
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;
			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(ehashmap.lookup(did));

			xBadQ.get(eid)+=cBadQuality_depth.getBinContent(did);
			if (eid.isVMEid())
			{
				xEvn.get(eid)+=cEvnMsm_ElectronicsVME.getBinContent(eid);
				xBcn.get(eid)+=cBcnMsm_ElectronicsVME.getBinContent(eid);
			}
			else
			{
				xEvn.get(eid)+=cEvnMsm_ElectronicsuTCA.getBinContent(eid);
				xBcn.get(eid)+=cBcnMsm_ElectronicsuTCA.getBinContent(eid);
			}
		}

		//	iterate over all FEDs
		std::vector<flag::Flag> sumflags;
		for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
			it!=_vhashFEDs.end(); ++it)
		{
			flag::Flag fSum("RAW");
			HcalElectronicsId eid(*it);
			
			//	check if this FED was @cDAQ
			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(), *it);
			if (cit==_vcdaqEids.end())
			{
				//	was not @cDAQ, set the summary flag as NA and go the next
				sumflags.push_back(flag::Flag("RAW", flag::fNCDAQ));
				continue;
			}

			//	here only if was registered at cDAQ
			if (utilities::isFEDHBHE(eid) || utilities::isFEDHF(eid) ||
				utilities::isFEDHO(eid))
			{
				if (xEvn.get(eid)>0)
					vflags[fEvnMsm]._state = flag::fBAD;
				else
					vflags[fEvnMsm]._state = flag::fGOOD;
				if (xBcn.get(eid)>0)
					vflags[fBcnMsm]._state = flag::fBAD;
				else
					vflags[fBcnMsm]._state = flag::fGOOD;
				if (xBadQ.get(eid)>0)
					vflags[fBadQ]._state = flag::fBAD;
				else
					vflags[fBadQ]._state = flag::fGOOD;
			}

			//	combine all the flags into summary flag
			int iflag=0;
			for (std::vector<flag::Flag>::iterator ft=vflags.begin();
				ft!=vflags.end(); ++ft)
			{
				cSummary.setBinContent(eid, iflag,ft->_state);
				fSum+=(*ft);
				iflag++;
				ft->reset();
			}
			sumflags.push_back(fSum);
		}
	
		return sumflags;
	}
}
