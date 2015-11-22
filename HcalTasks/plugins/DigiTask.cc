
#include "DQM/HcalTasks/interface/DigiTask.h"

using namespace hcaldqm;
DigiTask::DigiTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Signal, ADC, fC, SumQ
	_cfCperTS_SubDet(_name+"/Signal/fC_SubDet", "fCperTS",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cADCperTS_SubDet(_name+"/Signal/ADC_SubDet", "ADCperTS",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fADC),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cSumQ_SubDetPM_iphi(_name+"/Signal/SubDetPM_iphi", "SumQ",
		mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cSumQ_depth(_name+"/Signal/depth", "SumQ",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fNomFC)),
	_cSumQvsLS_SubDetPM_iphi(_name+"/Signal/vsLS_SubDetPM_iphi", "SumQvsLS",
		mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),

	//	Shape
	_cShape_SubDetPM_iphi(_name+"/Shape/SubDetPM_iphi", "Shape",
		mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cShapeCut_SubDetPM_iphi(_name+"/Shape/SubDetPM_iphi", "Shape",
		mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cShapeCut_p3e41d2(_name+"/Shape/iphi3ieta41d2", "Shape",
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cShapeCut_p3em41d2(_name+"/Shape/iphi3ieta-41d2", "Shape",
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),

	//	Timing
	_cTimingCut_SubDetPM_iphi(_name+"/Timing/SubDetPM_iphi", 
		"Timing", mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS_200)),
	_cTimingCutvsieta_SubDet_iphi(_name+"/Timing/vsieta_SubDet_iphi",
		"Timingvsieta", mapper::fSubDet_iphi,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::ValueAxis(axis::fYaxis, axis::fTimeTS_200)),
	_cTimingCutvsiphi_SubDet_ieta(_name+"/Timing/vsiphi_SubDet_ieta",
		"Timingvsiphi", mapper::fSubDet_ieta,
		new axis::CoordinateAxis(axis::fXaxis, axis::fiphi),
		new axis::ValueAxis(axis::fYaxis, axis::fTimeTS_200)),
	_cTimingCutvsLS_SubDetPM_iphi(_name+"/Timing/vsLS_SubDetPM_iphi",
		"Timing", mapper::fSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fTimeTS_200)),
	_cTimingCut_depth(_name+"/Timing/depth", "Timing",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fTimeTS_200)),

	//	Special
	_cQ2Q12CutvsLS_HFPM_iphi(_name+"/Q2Q12/vsLS_HFPM_iphi",
		"Q2Q12", mapper::fHFPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fRatio)),
	_cQ2Q12CutvsLS_p3e41d2(_name+"/Q2Q12/vsLS_iphi3ieta41d2",
		"Q2Q12",
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fRatio)),
	_cQ2Q12CutvsLS_p3em41d2(_name+"/Q2Q12/vsLS_iphi3ieta-41d2",
		"Q2Q12",
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fRatio)),
	_cDigiSizevsLS_SubDet(_name+"/DigiSize/vsLS_SubDet", "DigiSize",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fDigiSize)),

	//	Occupancy
	_cOccupancyvsiphi_SubDet(_name+"/Occupancy/vsiphi_SubDet", "Occupancyvsiphi",
		mapper::fSubDet,
		new axis::CoordinateAxis(fXaxis, axis::fiphi)),
	_cOccupancyCutvsiphi_SubDet(_name+"/Occupancy/vsiphi_SubDet", 
		"Occupancyvsiphi",
		mapper::fSubDet,
		new axis::CoordinateAxis(fXaxis, axis::fiphi)),
	_cOccupancyvsLS_SubDet(_name+"/Occupancy/vsLS_SubDet",
		"OccupancyvsLS",
		mapper::fSubDet,
		new axis::ValueAxis(fXaxis, axis::fLS),
		new axis::ValueAxis(fYaxis, axis::fEntries)),
	_cOccupancyCutvsLS_SubDet(_name+"/Occupancy/vsLS_SubDet",
		"OccupancyvsLS",
		mapper::fSubDet,
		new axis::ValueAxis(fXaxis, axis::fLS),
		new axis::ValueAxis(fYaxis, axis::fEntries)),
	_cOccupancy_depth(_name+"/Occupancy/depth", "Occupancy",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cOccupancyCut_depth(_name+"/Occupancy/depth", "Occupancy",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cOccupancyCutiphivsLS_SubDet(_name+"/Occupancy/iphivsLS_SubDet", 
		"OccupancyiphivsLS",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),
	_cMsn1LS_depth(_name+"/Missing/1LS_depth", "Missing",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cMsn10LS_depth(_name+"/Missing/10LS_depth", "Missing",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cMsn1LSvsLS_depth(_name+"/Missing/1LSvsLS_SubDet", "Missing",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries)),

	//	Summaries
	_cSummary(_name+"/Summary", "Summary",
		new axis::CoordinateAxis(axis::fXaxis, axis::fSubDet),
		new axis::FlagAxis(axis::fYaxis, "Flag", int(nDigiFlag))),
	_cSummaryvsLS_SubDet(_name+"/Summary/vsLS_SubDet", "SummaryvsLS",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::FlagAxis(axis::fYaxis, "Flag", int(nDigiFlag)))
{
	//	tags
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));

	// cuts
	_cutSumQ_HBHE = ps.getUntrackedParameter<double>("cutSumQ_HBHE", 20);
	_cutSumQ_HO = ps.getUntrackedParameter<double>("cutSumQ_HO", 20);
	_cutSumQ_HF = ps.getUntrackedParameter<double>("cutSumQ_HF", 20);
	
	//	flags
	_fNames.push_back("Low Occupancy");
	_fNames.push_back("Digi Size Drift");
	_fNames.push_back("iphi Uniformity");
	_fNames.push_back("Fiber Bcn Offset");
	_fNames.push_back("Missing for 1LS");
	_fNames.push_back("Missing for 10LS");
	_fNames.push_back("Missing Always");
	_cSummary.loadLabels(_fNames);
	_cSummaryvsLS_SubDet.loadLabels(_fNames);
}

/* virtual */ void DigiTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	char cutstr[200];
	sprintf(cutstr, "_SumQHBHE%dHO%dHF%d", int(_cutSumQ_HBHE),
		int(_cutSumQ_HO), int(_cutSumQ_HF));
	char cutstr2[200];
	sprintf(cutstr2, "_SumQHF%d", int(_cutSumQ_HF));

	DQTask::bookHistograms(ib, r, es);
	_cADCperTS_SubDet.book(ib);
	_cfCperTS_SubDet.book(ib);
	_cSumQ_SubDetPM_iphi.book(ib);
	_cSumQ_depth.book(ib);
	_cSumQvsLS_SubDetPM_iphi.book(ib);

	_cShape_SubDetPM_iphi.book(ib);
	_cShapeCut_SubDetPM_iphi.book(ib, _subsystem, std::string(cutstr));
	_cShapeCut_p3e41d2.book(ib, _subsystem, std::string(cutstr));
	_cShapeCut_p3em41d2.book(ib, _subsystem, std::string(cutstr));

	_cTimingCut_SubDetPM_iphi.book(ib, _subsystem, std::string(cutstr));
	_cTimingCutvsieta_SubDet_iphi.book(ib, _subsystem, 
		std::string(cutstr));
	_cTimingCutvsiphi_SubDet_ieta.book(ib, _subsystem, std::string(cutstr));
	_cTimingCutvsLS_SubDetPM_iphi.book(ib, _subsystem, std::string(cutstr));
	_cQ2Q12CutvsLS_HFPM_iphi.book(ib, _subsystem, std::string(cutstr2));
	_cQ2Q12CutvsLS_p3e41d2.book(ib, _subsystem, std::string(cutstr2));
	_cQ2Q12CutvsLS_p3em41d2.book(ib, _subsystem, std::string(cutstr2));
	_cTimingCut_depth.book(ib, _subsystem, std::string(cutstr));

	_cOccupancyvsiphi_SubDet.book(ib);
	_cOccupancyCutvsiphi_SubDet.book(ib, _subsystem, std::string(cutstr));
	_cOccupancyvsLS_SubDet.book(ib);
	_cOccupancyCutvsLS_SubDet.book(ib, _subsystem, std::string(cutstr));
	_cOccupancy_depth.book(ib);
	_cOccupancyCut_depth.book(ib, _subsystem, std::string(cutstr));
	_cOccupancyCutiphivsLS_SubDet.book(ib, _subsystem, std::string(cutstr));
	_cMsn1LS_depth.book(ib);
	_cMsn10LS_depth.book(ib);

	_cDigiSizevsLS_SubDet.book(ib);

	_cSummary.book(ib);
	_cSummaryvsLS_SubDet.book(ib);

}

/* virtual */ void DigiTask::_resetMonitors(UpdateFreq uf)
{
	switch (uf)
	{
		case fEvent:
			for (unsigned int i=0; i<constants::SUBDET_NUM; i++)
			{
				_numDigis[i]=0;
				_numDigisCut[i] = 0;
			}
			break;
		case hcaldqm::fLS:
			for (unsigned int idet=0; idet<constants::SUBDET_NUM; idet++)
			{
				for (int iiphi=0; iiphi<constants::IPHI_NUM; iiphi++)
					for (int iieta=0; iieta<constants::IETA_NUM; iieta++)
						for (int id=0; id<constants::DEPTH_NUM; id++)
							_occ_1LS[idet][iiphi][iieta][id] = false;
				_nMsn[idet] = 0;
			}
			break;
		case hcaldqm::f10LS:
			for (unsigned int idet=0; idet<constants::SUBDET_NUM; idet++)
				for (int iiphi=0; iiphi<constants::IPHI_NUM; iiphi++)
					for (int iieta=0; iieta<constants::IETA_NUM; iieta++)
						for (int id=0; id<constants::DEPTH_NUM; id++)
							_occ_10LS[idet][iiphi][iieta][id] = false;
			_cMsn1LS_depth.reset();
			break;
		case hcaldqm::f50LS:
			_cMsn10LS_depth.reset();
			break;
		default:
			break;
	}
	DQTask::_resetMonitors(uf);
}

/* virtual */ void DigiTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HBHEDigiCollection>		chbhe;
	edm::Handle<HODigiCollection>		cho;
	edm::Handle<HFDigiCollection>		chf;

	if (!e.getByLabel(_tagHBHE, chbhe))
		this->_throw("Collection HBHEDigiCollection isn't available",
			" " + _tagHBHE.label() + " " + _tagHBHE.instance());
	if (!e.getByLabel(_tagHO, cho))
		this->_throw("Collection HODigiCollection isn't available",
			" " + _tagHO.label() + " " + _tagHO.instance());
	if (!e.getByLabel(_tagHF, chf))
		this->_throw("Collection HFDigiCollection isn't available",
			" " + _tagHF.label() + " " + _tagHF.instance());

	for (HBHEDigiCollection::const_iterator it=chbhe->begin();
		it!=chbhe->end(); ++it)
	{
		const HBHEDataFrame digi = (const HBHEDataFrame)(*it);
		double sumQ = utilities::sumQ<HBHEDataFrame>(digi, 2.5, 0, 
			digi.size()-1);
		double timing = utilities::aveTS<HBHEDataFrame>(digi, 2.5, 0,
			digi.size()-1);
		const HcalDetId did = digi.id();
		int iieta = did.ieta()<0 ? abs(did.ieta())-constants::IETA_MIN :
			did.ieta()-constants::IETA_MIN+constants::IETA_NUM/2;

		//	fill without a cut
		_cOccupancy_depth.fill(did);
		_cOccupancyvsiphi_SubDet.fill(did);
		_cSumQ_SubDetPM_iphi.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_SubDetPM_iphi.fill(did, _currentLS, sumQ);
		_numDigis[did.subdet()-1]++;
		_cDigiSizevsLS_SubDet.fill(did, _currentLS, digi.size());
		_occ_1LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_10LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_Always[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;

		//	fill with a cut
		if (sumQ>_cutSumQ_HBHE)
		{
			_cTimingCut_SubDetPM_iphi.fill(did, timing);
			_cTimingCutvsieta_SubDet_iphi.fill(did, timing);
			_cTimingCutvsiphi_SubDet_ieta.fill(did,	timing);
			_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, timing);
			_cTimingCut_depth.fill(did, timing);
			_cOccupancyCutvsiphi_SubDet.fill(did);
			_cOccupancyCut_depth.fill(did);
//			_cOccupancyCutiphivsLS_SubDet.fill(did, _currentLS);

			_numDigisCut[digi.id().subdet()-1]++;
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(did, digi.sample(i).adc());
			_cfCperTS_SubDet.fill(did, digi.sample(i).nominal_fC());
			_cShape_SubDetPM_iphi.fill(did, i, digi.sample(i).nominal_fC()-2.5);

			//	with a cut
			if (sumQ>_cutSumQ_HBHE)
			{
				_cShapeCut_SubDetPM_iphi.fill(did, i,
					digi.sample(i).nominal_fC()-2.5);
			}
		}
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		double sumQ = utilities::sumQ<HODataFrame>(digi, 8.5, 0, 
			digi.size()-1);
		double timing = utilities::aveTS<HODataFrame>(digi, 8.5, 0,
			digi.size()-1);
		const HcalDetId did = digi.id();
		int iieta = did.ieta()<0 ? abs(did.ieta())-constants::IETA_MIN :
			did.ieta()-constants::IETA_MIN+constants::IETA_NUM/2;

		//	fill without a cut
		_cOccupancy_depth.fill(did);
		_cOccupancyvsiphi_SubDet.fill(did);
		_cSumQ_SubDetPM_iphi.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_SubDetPM_iphi.fill(did, _currentLS, sumQ);
		_numDigis[did.subdet()-1]++;
		_cDigiSizevsLS_SubDet.fill(did, _currentLS, digi.size());
		_occ_1LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_10LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_Always[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;

		//	fill with a cut
		if (sumQ>_cutSumQ_HO)
		{
			_cTimingCut_SubDetPM_iphi.fill(did, timing);
			_cTimingCutvsieta_SubDet_iphi.fill(did, timing);
			_cTimingCutvsiphi_SubDet_ieta.fill(did,	timing);
			_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, timing);
			_cTimingCut_depth.fill(did, timing);
			_cOccupancyCutvsiphi_SubDet.fill(did);
			_cOccupancyCut_depth.fill(did);
//			_cOccupancyCutiphivsLS_SubDet.fill(did, _currentLS);
		
			_numDigisCut[digi.id().subdet()-1]++;
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(did, digi.sample(i).adc());
			_cfCperTS_SubDet.fill(did, digi.sample(i).nominal_fC());
			_cShape_SubDetPM_iphi.fill(did, i, digi.sample(i).nominal_fC()-8.5);

			//	with a cut
			if (sumQ>_cutSumQ_HO)
			{
				_cShapeCut_SubDetPM_iphi.fill(did, i,
					digi.sample(i).nominal_fC()-8.5);
			}
		}
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		double sumQ = utilities::sumQ<HFDataFrame>(digi, 2.5, 0, 
			digi.size()-1);
		double timing = utilities::aveTS<HFDataFrame>(digi, 2.5, 0,
			digi.size()-1);
		const HcalDetId did = digi.id();
		int iieta = did.ieta()<0 ? abs(did.ieta())-constants::IETA_MIN :
			did.ieta()-constants::IETA_MIN+constants::IETA_NUM/2;

		//	fill without a cut
		_cOccupancy_depth.fill(did);
		_cOccupancyvsiphi_SubDet.fill(did);
		_cSumQ_SubDetPM_iphi.fill(did, sumQ);
		_cSumQ_depth.fill(did, sumQ);
		_cSumQvsLS_SubDetPM_iphi.fill(did, _currentLS, sumQ);
		_numDigis[digi.id().subdet()-1]++;
		_cDigiSizevsLS_SubDet.fill(did, _currentLS, digi.size());
		_occ_1LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_10LS[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;
		_occ_Always[did.subdet()-1][did.iphi()-1][iieta][did.depth()-1] = true;

		//	fill with a cut
		if (sumQ>_cutSumQ_HF)
		{
			_cTimingCut_SubDetPM_iphi.fill(did, timing);
			_cTimingCutvsieta_SubDet_iphi.fill(did, timing);
			_cTimingCutvsiphi_SubDet_ieta.fill(did,	timing);
			_cTimingCut_depth.fill(did, timing);
			_cTimingCutvsLS_SubDetPM_iphi.fill(did, _currentLS, timing);
			_cOccupancyCutvsiphi_SubDet.fill(did);
			_cOccupancyCut_depth.fill(did);
		
			double q1 = digi.sample(1).nominal_fC()-2.5;
			double q2 = digi.sample(2).nominal_fC()-2.5;
			double q2q12 = q2/(q1+q2);
			_cQ2Q12CutvsLS_HFPM_iphi.fill(did, _currentLS, q2q12);
			if (did.iphi()==3 && did.ieta()==41 && did.depth()==2)
				_cQ2Q12CutvsLS_p3e41d2.fill(did, _currentLS, q2q12);
			if (did.iphi()==3 && did.ieta()==-41 && did.depth()==2)
				_cQ2Q12CutvsLS_p3em41d2.fill(did, _currentLS, q2q12);
			
//			_cOccupancyCutiphivsLS_SubDet.fill(did, _currentLS);
		
			_numDigisCut[digi.id().subdet()-1]++;
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(did, digi.sample(i).adc());
			_cfCperTS_SubDet.fill(did, digi.sample(i).nominal_fC());
			_cShape_SubDetPM_iphi.fill(did, i, digi.sample(i).nominal_fC()-2.5);

			//	with a cut
			if (sumQ>_cutSumQ_HF)
			{
				_cShapeCut_SubDetPM_iphi.fill(did, i,
					digi.sample(i).nominal_fC()-2.5);
				if (did.iphi()==3 && did.ieta()==41 && did.depth()==2)
					_cShapeCut_p3e41d2.fill(did, i,
						digi.sample(i).nominal_fC()-2.5);
				if (did.iphi()==3 && did.ieta()==-41 && did.depth()==2)
					_cShapeCut_p3em41d2.fill(did, i,
						digi.sample(i).nominal_fC()-2.5);
			}
		}
	}

	//	Fill the occupancy vs LS
	_cOccupancyvsLS_SubDet.fill(HcalDetId(HcalBarrel, 5, 5, 1), _currentLS,
		_numDigis[0]);
	_cOccupancyCutvsLS_SubDet.fill(HcalDetId(HcalBarrel, 5, 5, 1), _currentLS,
		_numDigisCut[0]);
	_cOccupancyvsLS_SubDet.fill(HcalDetId(HcalEndcap, 18, 5, 1), _currentLS,
		_numDigis[1]);
	_cOccupancyCutvsLS_SubDet.fill(HcalDetId(HcalEndcap, 18, 5, 1), _currentLS,
		_numDigisCut[1]);
	_cOccupancyvsLS_SubDet.fill(HcalDetId(HcalOuter, 5, 5, 4), _currentLS,
		_numDigis[2]);
	_cOccupancyCutvsLS_SubDet.fill(HcalDetId(HcalOuter, 5, 5, 4), _currentLS,
		_numDigisCut[2]);
	_cOccupancyvsLS_SubDet.fill(HcalDetId(HcalForward, 34, 5, 1), _currentLS,
		_numDigis[3]);
	_cOccupancyCutvsLS_SubDet.fill(HcalDetId(HcalForward, 34, 5, 1), _currentLS,
		_numDigisCut[3]);
}

/* virtual */ void DigiTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	//	statuses
	//	By default the flag is not applicable
	double status[nDigiFlag][constants::SUBDET_NUM]; 
	for (int i=fLowOcp; i<nDigiFlag; i++)
		for (unsigned int j=0; j<constants::SUBDET_NUM; j++)
			status[i][j]=constants::NOT_APPLICABLE;

	/*
	 * Do the checks here.
	 * -> HF Digi Occupancy
	 * -> Digi Size Fluctuations
	 */

	//	HF Digi Occupancy Check
	MonitorElement *meocpHF = _cOccupancyvsLS_SubDet.at(3);
	double numChs = meocpHF->getBinContent(_currentLS);
	if (constants::CHS_NUM[3] - numChs>=48)
		status[fLowOcp][3] = constants::VERY_LOW;
	else if (constants::CHS_NUM[3] - numChs>=24)
		status[fLowOcp][3] = constants::LOW;
	else if (constants::CHS_NUM[3] - numChs>=10)
		status[fLowOcp][3] = constants::LOW;
	else if (constants::CHS_NUM[3] - numChs>=1)
		status[fLowOcp][3] = constants::PROBLEMATIC;
	else if (constants::CHS_NUM[3] - numChs<0)
		status[fLowOcp][3] = constants::PROBLEMATIC;
	else if (constants::CHS_NUM[3]==numChs)
		status[fLowOcp][3] = constants::GOOD;
	for (int i=0; i<constants::SUBDET_NUM; i++)
	{
		_cSummary.setBinContent(i, int(fLowOcp), status[fLowOcp][i]);
		_cSummaryvsLS_SubDet.setBinContent(i, 
			_currentLS, int(fLowOcp), status[fLowOcp][i]);
	}

	//	Digi Size Check
	for (unsigned int i=0; i<constants::SUBDET_NUM; i++)
	{
		MonitorElement *meds = _cDigiSizevsLS_SubDet.at(i);
		double size = meds->getBinContent(_currentLS);
		double error = meds->getBinError(_currentLS);
		if (size==constants::TS_NUM[i] && error==0)
			status[fDigiSize][i] = constants::GOOD;
		else
			status[fDigiSize][i] = constants::PROBLEMATIC;

	}
	for (int i=0; i<constants::SUBDET_NUM; i++)
	{
		_cSummary.setBinContent(i, int(fDigiSize), status[fDigiSize][i]);
		_cSummaryvsLS_SubDet.setBinContent(i,
			_currentLS, int(fLowOcp), status[fLowOcp][i]);
	}

	/*
	 *	Generic all Hcal Loop.
	 *	-> Possible Missing Channels
	 */
	for (unsigned int idet=0; idet<constants::SUBDET_NUM; idet++)
	{
		HcalSubdetector subd = HcalEmpty;
		if (idet+1==HB)
			subd = HcalBarrel;
		else if (idet+1==HE)
			subd = HcalEndcap;
		else if (idet+1==HO)
			subd = HcalOuter;
		else
			subd = HcalForward;
		for (int iiphi=0; iiphi<constants::IPHI_NUM; iiphi++)
			for (int iieta=0; iieta<constants::IETA_NUM; iieta++)
				for (int id=0; id<constants::DEPTH_NUM; id++)
				{
					int ieta = iieta<constants::IETA_NUM/2 ? 
						-(iieta+constants::IETA_MIN) : 
						iieta-constants::IETA_NUM/2+constants::IETA_MIN;
					HcalDetId did(subd, ieta, iiphi+1, id+1);
					//	if not a valid Detector cell continue;
					if (!utilities::validDetId(did))
						continue;

					//	if absent for 1 full LS;
					if (!_occ_1LS[idet][iiphi][iieta][id])
					{
						_cMsn1LS_depth.fill(did);
						_nMsn[idet]++;
					}
					//	if absent for 10LSs 
					if (_procLSs>0 && _procLSs%10==0 && 
						!_occ_10LS[idet][iiphi][iieta][id])
						_cMsn10LS_depth.fill(did);
				}
	}
	_cMsn1LSvsLS_SubDet.fill(HcalDetId(HcalBarrel, 5, 5, 1), _nMsn[0]);
	_cMsn1LSvsLS_SubDet.fill(HcalDetId(HcalEndcap, 18, 5, 1), _nMsn[1]);
	_cMsn1LSvsLS_SubDet.fill(HcalDetId(HcalOuter, 5, 5, 4), _nMsn[2]);
	_cMsn1LSvsLS_SubDet.fill(HcalDetId(HcalForward, 32, 5, 1), _nMsn[3]);

	//	in the end always do the DQTask::endLumi
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(DigiTask);


