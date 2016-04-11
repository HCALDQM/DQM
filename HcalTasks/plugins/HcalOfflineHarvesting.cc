#include "DQM/HcalTasks/interface/HcalOfflineHarvesting.h"

HcalOfflineHarvesting::HcalOfflineHarvesting(edm::ParameterSet const& ps) :
	DQHarvester(ps), _reportSummaryMap(NULL)
{
	//	set labels for summaries
	_frawnames.push_back("EvnMsm");
	_frawnames.push_back("BcnMsm");
	_frawnames.push_back("BadQuality");

	_fdiginames.push_back("UniSlot");
	_fdiginames.push_back("Msn1LS");
	_fdiginames.push_back("CapIdRot");
	_fdiginames.push_back("DigiSize");

	_freconames.push_back("OcpUniSlot");
	_freconames.push_back("TimeUniSlot");
	_freconames.push_back("TCDS");
	_freconames.push_back("Msn1LS");

	_ftpnames.push_back("OcpUniSlotD");
	_ftpnames.push_back("OcpUniSlotE");
	_ftpnames.push_back("EtMsmUniSlot");
	_ftpnames.push_back("FGMsmUniSlot");
	_ftpnames.push_back("MsnUniSlotD");
	_ftpnames.push_back("MsnUniSlotE");
	_ftpnames.push_back("EtCorrRatio");
	_ftpnames.push_back("EtMsmRatio");
	_ftpnames.push_back("FGMsmNumber");
}

/* virtual */ void HcalOfflineHarvesting::beginRun(
	edm::Run const& r, edm::EventSetup const& es)
{
	DQHarvester::beginRun(r,es);

	//	get hash Map
	_ehashmapD.initialize(_emap, electronicsmap::fD2EHashMap);
	_ehashmapT.initialize(_emap, electronicsmap::fT2EHashMap);

	//	HF FEDs filter
	std::vector<uint32_t> vfedsHF;
	vfedsHF.push_back(HcalElectronicsId(22, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	vfedsHF.push_back(HcalElectronicsId(29, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	vfedsHF.push_back(HcalElectronicsId(32, SLOT_uTCA_MIN,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_FEDHF.initialize(filter::fPreserver, hashfunctions::fFED,
		vfedsHF);
}

//
//	For OFFLINE there is no per LS evaluation
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndLuminosityBlock(
	DQMStore::IBooker& ib,
	DQMStore::IGetter& ig, edm::LuminosityBlock const&, 
	edm::EventSetup const&)
{}

//
//	Evaluate and Generate Run Summary
//
/* virtual */ void HcalOfflineHarvesting::_dqmEndJob(DQMStore::IBooker&,
	DQMStore::IGetter&)
{
	//	OBTAIN/SET WHICH MODULES ARE PRESENT
	int ccc=0;
	if (ig.get(_subsystem+"/RawTask/EventsTotal")!=NULL)
	{_rawHarvesting=true;ccc++;}
	if (ig.get(_subsystem+"/DigiTask/EventsTotal")!=NULL)
	{_digiHarvesting=true;ccc++;}
	if (ig.get(_subsystem+"/TPTask/EventsTotal")!=NULL)
	{_tpHarvesting=true;ccc++;}
	if (ig.get(_subsystem+"/RecHitTask/EventsTotal")!=NULL)
	{_recoHarvesting=true;ccc++;}

	//	CREATE REPORT SUMMARY MAP
	//	This is the Histogram is that shown up fron in the Summary Workspace
	//	or when you just access the DQM GUI Offline Server.
	//	It's just a total summary per each Data Tier that we have
	if (!_reportSummaryMap)
	{
		ig.setCurrentFolder(_subsystem+"/EventInfo");
		_reportSummaryMap = ib.book2D("reportSummaryMap", "reportSummaryMap",
			_vFEDs.size(), 0, _vFEDs.size(), ccc, 0, ccc);
		ccc = 1;
		if (_rawHarvesting)
		{_reportSummaryMap->setBinLabel(ccc, "RAW", 2);
		_modules[0] = ccc; ccc++;}
		if (_digiHarvesting)
		{_reportSummaryMap->setBinLabel(ccc, "DIGI", 2);
		_modules[1] = ccc;ccc++;}
		if (_recoHarvesting)
		{_reportSummaryMap->setBinLabel(ccc, "RECO", 2);
		_modules[2] = ccc;ccc++;}
		if (_tpHarvesting)
		{_reportSummaryMap->setBinLabel(ccc, "TP", 2);
		_modules[3] = ccc;ccc++;}
		for (uint32_t i=0; i<_vFEDs.size(); i++)
		{
			char name[5];
			sprintf(name, "%d", _vFEDs[i]);
			_reportSummaryMap->setBinLabel(i+1, name, 1);
		}
	}

	//	initialize some important variables
	std::vector<HcalGenericDetId> gids = _emap.allPrecisionId();
	std::vector<HcalTrigTowerDetId> tids = _emap.allTriggerId();
	ContainerXXX<double> xBcn,xEvn,xBadQ,xDeadD,xUniHFD,xUniHFR,xUniD,xUniR,
		xDeadR, xDeadData,xDeadEmul,xEtMsm,xFGMsm;
	xBcn.initialize(hashfunctions::fFED); xBcn.book(_emap);
	xEvn.initialize(hashfunctions::fFED); xEvn.book(_emap);
	xBadQ.initialize(hashfunctions::fFED); xBadQ.book(_emap);
	xDeadD.initialize(hashfunctions::fFED); xDead.book(_emap);
	xUniHFD.initialize(hashfunctions::fFEDSlot); 
	xUniHFD.book(_emap, _filter_FEDHF);
	xUniHFR.initialize(hashfunctions::fFEDSlot);
	xUniHFR.book(_emap, _filter_FEDHF);
	xUniD.initialize(hashfunctions::fFED); xUniD.book(_emap);
	xUniR.initialize(hashfunctions::fFED); xUniR.book(_emap);
	xDeadR.initialize(hashfunctions::fFED); xDeadR.book(_emap);
	xDeadData.initialize(hashfunctions::fFED); xDeadData.book(_emap);
	xDeadEmul.initialize(hashfunctions::fFED); xDEadEmul.book(_emap);
	xEtMsm.initialize(hashfunctions::fFED); xEtMsm.book(_emap);
	xFGMsm.initialize(hashfunctions::FFED); xFGMsm.book(_emap);
	bool tcdsshift = false;

	//
	//	ITERATE THRU ALL THE DETIds AND EVALUATE
	//
	if (_rawHarvesting || _digiHarvesting || _recoHarvesting)
	{
		for (std::vector<HcalGenericDetId>::const_iterator it=gids.begin();
			it!=gids.end(); ++it)
		{
			if (!it->isHcalDetId())
				continue;
			HcalDetId did = HcalDetId(it->rawId());
			HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

			//	do RAW
			if (_rawHarvesting)
			{
				xBadQ.get(eid)+=_cBadQuality_depth.getBinContent(did);
				if (eid.isVMEid())
				{
					xEvn.get(eid)+=_cEvnMsm_ElectronicsVME.getBinContent(eid);
					xBcn.get(eid)+=_cBcnMsm_ElectronicsVME.getBinContent(eid);
				}
				else
				{
					xEvn.get(eid)+=_cEvnMsm_ElectronicsuTCA.getBinContent(eid);
					xBcn.get(eid)+=_cBcnMsm_ElectronicsuTCA.getBinContent(eid);
				}
			}

			//	do DIGI
			if (_digiHarvesting)
			{
				_cDOccupancy_depth.getBinContent(did)<1?
					xDeadD.get(eid)++:xDeadD.get(eid)+=0;
				if (did.subdet()==HcalForward)
					_xUniHFD.get(eid)+=_cDOccupancyCut_depth(did);
				if (eid.isVMEid())
				{
					_cDigiSize_FEDVME.getMean(eid)!=
						constants::DIGISIZE[did.subdet()-1]?
						xDigiSize.get(eid)++:xDigiSize.get(eid)+=0;
					_cDigiSize_FEDVME.getRMS(eid)!=0?
						xDigiSize.get(eid)++:xDigiSize.get(eid)+=0;
				}
				else
				{
					_cDigiSize_FEDuTCA.getMean(eid);
					_cDigiSize_FEDuTCA.getRMS(eid);
				}
			}

			//	do RECO
			if (_recoHarvesting)
			{
				_cROccupancy_depth.getBinContent(did)<1?
					xDeadR.get(eid)++:xDeadR.get(eid)+=0;
				if (did.subdet()==HcalForward)
					_xUniHFR.get(eid)++_ROccupancyCut_depth(did);
			}
		}

		//	ITERATE THRU ALL HF FEDs AND SLOTs and CHECK THE UNIFORMITY
		//	OF OCCUPANCIES for DIGI
		if (_digiHarvesting)
		{
			for (doubleCompactMap::const_iterator it=xUniHFD.begin();
				it!=xUniHFD.end(); ++it)
			{
				uint32_t hash1 = it->first;
				HcalElectronicsId eid1(hash1);
				double x1 = it->second;

				for (doubleCompactMap::const_iterator jt=xUniHFD.begin();
					jt!=xUniHFD.end(); ++jt)
				{
					if (it==jt)
						continue;
					uint32_t hash2 = jt->first;
					double x2 = jt->second;
					if (x1/x2<0.2)
						xUniD.get(eid)++;
				}
			}
		}
		//	ITERATE THRU ALL HF FEDs AND SLOTs and CHECK THE UNIFORMITY
		//	OF OCCUPANCIES for RECO
		//	for RECO, check the possible TCDS shifts
		if (_recoarvesting)
		{
			//	ocp
			for (doubleCompactMap::const_iterator it=xUniHFR.begin();
				it!=xUniHFR.end(); ++it)
			{
				uint32_t hash1 = it->first;
				HcalElectronicsId eid1(hash1);
				double x1 = it->second;

				for (doubleCompactMap::const_iterator jt=xUniHFR.begin();
					jt!=xUniHFR.end(); ++jt)
				{
					if (it==jt)
						continue;
					uint32_t hash2 = jt->first;
					double x2 = jt->second;
					if (x1/x2<0.2)
						xUniR.get(eid)++;
				}
			}
			//	tcds
			double a = _cTimingCut_HBHEPartition.getMean(
				HcalDetId(HcalBarrel, 1, 5, 1));
			double b = _cTimingCut_HBHEPartition.getMean(
				HcalDetId(HcalBarrel, 1, 30, 1));
			double c = _cTimingCut_HBHEPartitiion.getMean(
				HcalDetId(HcalBarrel, 1, 55, 1));
			double dab = fabs(a-b);
			double dac = fabs(a-c);
			double dbc = fabs(b-c);
			if (dab>=1.5 || dac>=1.5 || dbc->1.5)
				tcdsshift = true;
		}
	}

	//
	//	ITERATE THRU ALL TRIGTOWERDETIds and EVALUATE
	//
	if (_tpHarvesting)
	{
		for (std::vector<HcalTrigTowerDetId>::const_iterator it=tids.begin();
			it!=tids.end(); ++it)
		{
			//	skip 2x3 HF TPs
			if (tid.version()==0 && ietaAbs()>=29)
				continue;
			
			HcalElectronicsId eid = HcalElectronicsId(_ehashmapT.lookup(*it));
			_cOccupancyData_depthlike.getBinContent(*it)<1?
				xDeadTD.get(eid)++:xDeadTD.get(eid)+=0;
			_cOccupancyEmul_depthlike.getBinContent(*it)<1?
				xDeadTE.get(eid)++:xDeadTE.get(eid)+=0;
			double etmsmfr = double(_cEtMsm_depthlike.getBinContent(*it))/
				double(_cEtCorrRatio_depthlike.getBinEntries(*it));
			etmsmfr>=0.1?xEtMsm.get(eid)++:xEtMsm.get(eid)+=0;
			double fgmsmfr = double(_cFGMsm_depthlike.getBinContent(*it))/
				double(_cEtCorrRatio_depthlike.getBinEntries(*it));
			fgmsmfr>=0.1?xFGMsm.get(eid)++:xFGMsm.get(eid)+=0;
		}
	}

	//
	// READY TO GENERATE RUN SUMMARY!
	//
	int ifed = 0;
	for (std::vector<uint32_t>::const_iterator it=_vhashFEDs.begin();
		it!=_vhashFEDs.end(); ++it)
	{
		HcalElectronicsId eid(*it);

		// RAW 
		// - evn > 0 fill
		// - bcn > 0 fill
		// - badq> 0 fill
		if (_rawHarvesting)
		{
			//	check if this FED was included at cDAQ
			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(), *it);
			if (cit==_vcdaqEids.end())
			{
				//	if was not - set all the flags as inapplicable
				for (uint32_t f=0; f<_frawnames.size(); f++)
					_rawSummary.setBinContent(eid,f,quantity::fNA);
				_creportSummaryMap->setBinContent(ifed+1, _modules[0],
					quantity::fNA);
			}
			else
			{
				//	FED is present - generate summary for it
				bool temp=false;
				if (xEvn.get(eid)>0)
				{
					_crawSummary.setBinContent(eid,fEvnMsm,quantity::fLow);
					temp=true;
				}
				else
					_crawSummary.setBinContent(eid,fEvnMsm,qunatity::fGood);
				if (xBcn.get(eid)>0)
				{
					_crawSummary.setBinContent(eid, fBcnMsm, quantity::fLow);
					temp=true;
				}
				else
					_crawSummary.setBinContent(eid, fBcnMsm, quantity::fGood);
				if (xBadQ.get(eid)>0)
				{
					_crawSummary.setBinContent(eid, fBadQ, quantity::fLow);
					temp=true;
				}
				else
					_crawSummary.setBinContent(eid, fBadQ, quantity::fGood);
				temp==true?
					_reportSummaryMap.setBinContent(ifed+1, _modules[0],
						quantity::fLow):
					_reportSummaryMap.setBinContent(ifed+1, _modules[0],
						quantity::fGood);
			}

		}

		//	DIGI if set to harvest
		counter=0;
		if (_digiHarvesting)
		{
			//	check if this FED was included at cDAQ
			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(), *it);
			if (cit==_vcdaqEids.end())
			{
				//	if was not - set all the flags as inapplicable
				for (uint32_t f=0; f<_fdiginames.size(); f++)
					_digiSummary.setBinContent(eid,f,quantity::fNA);
				_creportSummaryMap->setBinContent(ifed+1, _modules[1],
					quantity::fNA);
			}
			else
			{
				//	FED is preset - gen summary
				bool temp=false;
				if (xDeadD.get(eid)>0)
				{
					temp=true;
					_cdigiSummary.setBinContent(eid, fDeadD, quantity::fLow);
				}
				else
					_cdigiSummary.setBinContent(eid, fDeadD, quantity::fGood);
				if (xUniD.get(eid)>0)
				{
					temp=true;
					_cdigiSummary.setBinContent(eid, fUniD, quantity::fLow);
				}
				else
					_cdigiSummary.setBinContent(eid, fUniD, quantity::fGood);
				if (xDigiSize.get(eid)>0)
				{
					temp=true;
					_cdigiSummary.setBinContent(eid, fDigiSize, quantity::fLow);
				}
				else
					_cdigiSummary.setBinContent(eid, fDigiSize, 
						quantity::fGood);
				temp==true?
					_reportSummaryMap->setBinContent(ifed+1, _modules[1],
						quantity::fLow):
					_reportSummaryMap->setBinContent(ifed+1, _modules[1],
						quantity::fGood);
			}
		}

		//	RECO
		counter=0;
		if (_recoHarvesting)
		{
			//	check if this FED was included at cDAQ
			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(), *it);
			if (cit==_vcdaqEids.end())
			{
				//	if was not - set all the flags as inapplicable
				for (uint32_t f=0; f<_freconames.size(); f++)
					_recoSummary.setBinContent(eid,f,quantity::fNA);
				_creportSummaryMap->setBinContent(ifed+1, _modules[2],
					quantity::fNA);
			}
			else
			{
				//	FED is present - gen summary
				bool temp=false;
				if (xDeadR.get(eid)>0)
				{
					temp=true;
					_crecoSummary.setBinContent(eid, fDeadR, quantity::fLow);
				}
				else
					_crecoSummary.setBinContent(eid, fDeadR, quantity::fGood);
				if (tcdsshift)
				{
					temp=true;
					_crecoSummary.setBinContent(eid, fTCDS, quantity::fLow);
				}
				else
					_crecoSummary.setBinContent(eid, fTCDS, quantity::fGood);
				if (xUniR.get(eid)>0)
				{
					temp=true;
					_crecoSummary.setBinContent(eid, fUniR, quantity::fLow);
				}
				else
					_crecoSummary.setBinContent(eid, fUniR, quantity::fGood);
				temp==true?
					_reportSummaryMap->setBinContent(ifed+1, _modules[2],
						quantity::fLow):
					_reportSummaryMap->setBinContent(ifed+1, _modules[2],
						quantity::fGood);
			}
		}

		//	TP
		counter=0;
		if (_tpHarvesting)
		{
			//	check if this FED was included at cDAQ
			std::vector<uint32_t>::const_iterator cit=std::find(
				_vcdaqEids.begin(), _vcdaqEids.end(), *it);
			if (cit==_vcdaqEids.end())
			{
				//	if was not - set all the flags as inapplicable
				for (uint32_t f=0; f<_freconames.size(); f++)
					_recoSummary.setBinContent(eid,f,quantity::fNA);
				_creportSummaryMap->setBinContent(ifed+1, _modules[3],
					quantity::fNA);
			}
			else
			{
				//	FED is present
				bool temp = false;
				if (xDeadTD.get(eid)>0)
				{
					temp=true;
					_ctpSummary.setBinContent(eid, fDeadTD, quantity::fLow);
				}
				else
					_ctpSummary.setBinContent(eid, fDeadTD, quantity::fGood);
				if (xDeadTE.get(eid)>0)
				{
					temp=true;
					_ctpSummary.setBinConent(eid, fDeadTE, quantity::fLow);
				}
				else
					_ctpSummary.setBinContent(eid, fDeadTE, quantity::fGood);
				if (xEtMsm.get(eid)>0)
				{
					temp=true;
					_ctpSummary.setBinContent(eid, fEtMsm, quantity::fLow);
				}
				else
					_ctpSummary.setBinContent(eid, fEtMsm, quantity::fGood);
				if (xFGMsm.get(eid)>0)
				{
					temp=true;
					_ctpSummary.setBinContent(eid, fFGMsm, quantity::fLow);
				}
				else
					_ctpSummary.setBinContent(eid, fFGMsm, quantity::fGood);
				temp==true?
					_reportSummaryMap->setBinContent(ifed+1, _modules[3],
						quantity::fLow):
					_reportSummaryMap->setBinContent(ifed+1, _modules[3],
						quantity::fGood);
			}
		}

		//	increment fed counter
		ifed++;
	}
}

DEFINE_FWK_MODULE(HcalHarvesting);
