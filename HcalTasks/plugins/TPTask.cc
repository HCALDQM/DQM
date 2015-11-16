
#include "DQM/HcalTasks/interface/TPTask.h"

using namespace hcaldqm;
TPTask::TPTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Et
	_cEtData_SubDet(_name+"/Et/Data_SubDet", "Et",
		mapper::fTPSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_256),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cEtEmul_SubDet(_name+"/Et/Emul_SubDet", "Et",
		mapper::fTPSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_256),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cEtCorr_SubDet(_name+"/Et/Correlation_SubDet", "Et_DataVSEmul",
		mapper::fTPSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_128),
		new axis::ValueAxis(axis::fYaxis, axis::fEt_128)),
	_cEtMsm(_name+"/Et/MisMatched", "Et_MisMatched",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),
	_cEtData_SubDetPM_iphi(_name+"/Et/Data_SubDetPM_iphi", "EtData",
		mapper::fTPSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_256),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cEtData_SubDet_ieta(_name+"/Et/Data_SubDet_ieta", "EtData",
		mapper::fTPSubDet_ieta,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_256),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cEtCorr_SubDetPM_iphi(_name+"/Et/Correlation_SubDetPM_iphi", 
		"Et_DataVSEmul",
		mapper::fTPSubDetPM_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fEt_128),
		new axis::ValueAxis(axis::fYaxis, axis::fEt_128)),

	//	FG
	_cFGCorr_SubDet(_name+"/FG/Correlation_SubDet", "FG_DataVSEmul",
		mapper::fTPSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fFG),
		new axis::ValueAxis(axis::fYaxis, axis::fFG)),
	_cFGMsm(_name+"/FG/MisMatched", "FG_MisMatched",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),

	//	Occupancy
	_cOccupancyData(_name+"/Occupancy", "Occupancy_Data",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),
	_cOccupancyEmul(_name+"/Occupancy", "Occupancy_Emul",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),
	_cMsData(_name+"/Missing", "Missing_Data",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries)),
	_cMsEmul(_name+"/Missing", "Missing_Emul",
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries))
{
	//	tags
	_tagData = ps.getUntrackedParameter<edm::InputTag>("tagData",
		edm::InputTag("hcalDigis"));
	_tagEmul = ps.getUntrackedParameter<edm::InputTag>("tagEmul",
		edm::InputTag("emulDigis"));

	//	Special
	_skip1x1 = ps.getUntrackedParameter<bool>("skip1x1", true);
}

/* virtual */ void TPTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cEtData_SubDet.book(ib);
	_cEtEmul_SubDet.book(ib);
	_cEtCorr_SubDet.book(ib);
	_cEtMsm.book(ib);
	_cEtData_SubDetPM_iphi.book(ib);
	_cEtData_SubDet_ieta.book(ib);
	_cEtCorr_SubDetPM_iphi.book(ib);
	_cFGCorr_SubDet.book(ib);
	_cFGMsm.book(ib);
	_cOccupancyData.book(ib);
	_cOccupancyEmul.book(ib);
	_cMsData.book(ib);
	_cMsEmul.book(ib);
}

/* virtual */ void TPTask::_process(edm::Event const& e,
	edm::EventSetup const &es)
{
	edm::Handle<HcalTrigPrimDigiCollection> ctpd;
	edm::Handle<HcalTrigPrimDigiCollection> ctpe;

	if (!e.getByLabel(_tagData, ctpd))
		this->_throw("Collection HcalTrigPrimDigiCollection isn't available",
			" " + _tagData.label() + " " + _tagData.instance());
	if (!e.getByLabel(_tagEmul, ctpe))
		this->_throw("Collection HcalTrigPrimDigiCollection isn't available",
			" " + _tagEmul.label() + " " + _tagEmul.instance());

	//	tmp
	bool useD1 = false;
	//	tmp


	//	iterate thru the Data Digis
	for (HcalTrigPrimDigiCollection::const_iterator hddigi=ctpd->begin();
		hddigi!=ctpd->end(); ++hddigi)
	{
		//	tmpt
		if (_skip1x1)
			if (hddigi->id().depth()==10) // 10 is the depth for 1x1 TPs
				continue;
		//\tmp

		//	get quantities
		int soiEt_d = hddigi->SOI_compressedEt();
		int soiFG_d = hddigi->SOI_fineGrain() ? 1 : 0;

		//	tmp
		if (hddigi->id().depth()==1)
			useD1 = true;
		//\tmp

		//	fill individual
		_cEtData_SubDet.fill(hddigi->id(), soiEt_d);
		_cEtData_SubDetPM_iphi.fill(hddigi->id(), soiEt_d);
		_cEtData_SubDet_ieta.fill(hddigi->id(), soiEt_d);
		_cOccupancyData.fill(hddigi->id());

		//	now, find the emulator digi and compare
		HcalTrigPrimDigiCollection::const_iterator hedigi = 
			ctpe->find(HcalTrigTowerDetId(hddigi->id().ieta(),
				hddigi->id().iphi(), 0));
	
		if (hedigi!=ctpe->end())
		{
			int soiEt_e = hedigi->SOI_compressedEt();
			int soiFG_e = hedigi->SOI_fineGrain() ? 1 : 0;

			//	fill correlations
			_cEtEmul_SubDet.fill(hedigi->id(), soiEt_e);
			_cEtCorr_SubDet.fill(hddigi->id(), soiEt_e, soiEt_d);
			_cEtCorr_SubDetPM_iphi.fill(hddigi->id(), soiEt_e, soiEt_d);
			_cFGCorr_SubDet.fill(hddigi->id(), soiFG_e, soiFG_d);
			_cOccupancyEmul.fill(hedigi->id());

			//	fill those that mismatched
			if (soiEt_d!=soiEt_e)
				_cEtMsm.fill(hddigi->id());
			if (soiFG_d!=soiEt_e)
				_cFGMsm.fill(hddigi->id());
		}
		else 
		{
			_cMsEmul.fill(hddigi->id());
		}
	}

	//	iterate thru the Emulator Digis
	for (HcalTrigPrimDigiCollection::const_iterator hedigi=ctpe->begin();
		hedigi!=ctpe->end(); ++hedigi)
	{
		//	tmp
		if (_skip1x1)
			if (hedigi->id().depth()==10)	// 10 for 1x1
				continue;
		//	\tmp

		//	now, find the emulator digi and compare
		//	tmp
		HcalTrigPrimDigiCollection::const_iterator hddigi = 
			ctpd->find(HcalTrigTowerDetId(hedigi->id().ieta(),
				hedigi->id().iphi(), useD1 ? 1: 0));
		//	\tmp

		//	we have to only check those that are missing
		if (hddigi==ctpd->end())
		{
			_cMsData.fill(hedigi->id());
		}
	}
}

/* virtual */ void TPTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

DEFINE_FWK_MODULE(TPTask);


