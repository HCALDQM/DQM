#include "DQM/HcalTasks/interface/RawTask.h"

using namespace hcaldqm;
RawTask::RawTask(edm::ParameterSet const& ps):
	DQTask(ps),
	//	specify all the Containers
	_cVMEEvnMsm(_name+"/VME/EvnMsm", "EvnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDVME),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSpigot)),
	_cVMEBcnMsm(_name+"/VME/BcnMsm", "BcnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDVME),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSpigot)),
	_cVMEOrnMsm(_name+"/VME/OrnMsm", "OrnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDVME),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSpigot)),
	_cuTCAEvnMsm(_name+"/uTCA/EvnMsm", "EvnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDuTCA),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSlotuTCA)),
	_cuTCABcnMsm(_name+"/uTCA/BcnMsm", "BcnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDuTCA),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSlotuTCA)),
	_cuTCAOrnMsm(_name+"/uTCA/OrnMsm", "OrnMismatch",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDuTCA),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSlotuTCA)),
	_cVMEOccupancy(_name+"/VME/Occupancy", "Occupancy",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDVME),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSpigot)),
	_cuTCAOccupancy(_name+"/uTCA/Occupancy", "Occupancy",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDuTCA),
		new axis::CoordinateAxis(axis::fYaxis, axis::fSlotuTCA)),

	//	Summary Containers
	_cSummary(_name+"/Summary", "Summary",
		new axis::CoordinateAxis(axis::fXaxis, axis::fFEDComb),
		new axis::FlagAxis(axis::fYaxis, "Flag", int(nRawFlag))),
	_cSummaryvsLS_FED(_name+"/Summary/vsLS_FED", "SummaryvsLS",
		mapper::fFED,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::FlagAxis(axis::fYaxis, "Flag", int(nRawFlag)))
{
	//	tags
	_tagFEDs = ps.getUntrackedParameter<edm::InputTag>("tagFEDs",
		edm::InputTag("rawDataCollector"));

	//	load labels 
	_fNames.push_back("EVN Mismatch");
	_fNames.push_back("ORN Mismatch");
	_fNames.push_back("BCN Mismatch");
	_cSummary.loadLabels(_fNames);
	_cSummaryvsLS_FED.loadLabels(_fNames);
}

/* virtual */ void RawTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cVMEEvnMsm.book(ib);
	_cVMEBcnMsm.book(ib);
	_cVMEOrnMsm.book(ib);
	_cuTCAEvnMsm.book(ib);
	_cuTCABcnMsm.book(ib);
	_cuTCAOrnMsm.book(ib);
	_cVMEOccupancy.book(ib);
	_cuTCAOccupancy.book(ib);

	_cSummary.book(ib);
	_cSummaryvsLS_FED.book(ib);
}

/* virtual */ void RawTask::endLuminosityBlock(edm::LuminosityBlock const&,
	edm::EventSetup const&)
{}

/* virtual */ void RawTask::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<FEDRawDataCollection> craw;
	if (!e.getByLabel(_tagFEDs, craw))
		this->_throw("Collection FEDRawDataCollection isn't available",
			" " + _tagFEDs.label() + " " + _tagFEDs.instance());
	
	for (int fed=FEDNumbering::MINHCALFEDID; 
		fed<+FEDNumbering::MAXHCALuTCAFEDID; fed++)
	{
		//	skip all non-HCAL FEDs
		if ((fed>FEDNumbering::MAXHCALFEDID &&
			fed<FEDNumbering::MINHCALuTCAFEDID) ||
			fed>FEDNumbering::MAXHCALuTCAFEDID)
			continue;

		FEDRawData const& raw = craw->FEDData(fed);
		if (raw.size() < RAW_EMPTY)	// skip if empty
			continue;

		if (fed<=FEDNumbering::MAXHCALFEDID) // VME
		{
			HcalDCCHeader const* hdcc = (HcalDCCHeader const*)(raw.data());
			if (!hdcc)
				continue;;
			int bx = hdcc->getBunchId();
			unsigned int orn = hdcc->getOrbitNumber();
			unsigned evn = hdcc->getDCCEventNumber();
			int dccId = hdcc->getSourceId()-700; //	700 is the hard offset

			//	 Iterate over all the spigots
			HcalHTRData htr;
			for (int is=0; is<HcalDCCHeader::SPIGOT_COUNT; is++)
			{
				int r = hdcc->getSpigotData(is, htr, raw.size());

				//	invalid data
				if (r!=0)
					continue;

				unsigned int htr_evn = htr.getL1ANumber();
				unsigned int htr_orn = htr.getOrbitNumber();
				unsigned int htr_bx = htr.getBunchNumber();
				HcalElectronicsId eid(0, 1, is, dccId);
				_cVMEEvnMsm.fill(fed, eid, abs(htr_evn-evn));
				_cVMEBcnMsm.fill(fed, eid, abs(htr_bx-bx));
				_cVMEOrnMsm.fill(fed, eid, abs(htr_orn-orn));
				_cVMEOccupancy.fill(fed, eid);
			}
			
		}
		else //	uTCA
		{
			hcal::AMC13Header const* hamc13 = (hcal::AMC13Header const*)
				(raw.data());
			if (!hamc13)
				continue;

			int bx = hamc13->bunchId();
			unsigned int orn = hamc13->orbitNumber();
			int l1a = hamc13->l1aNumber();
			int namc = hamc13->NAMC();

			//	itearte over all AMC13
			for (int iamc=0; iamc<namc; iamc++)
			{
				int slot = hamc13->AMCSlot(iamc);
				int crate = hamc13->AMCId(iamc)&0xFF;
				HcalElectronicsId eid(crate, slot, 5, 0, false);
				HcalUHTRData uhtr(hamc13->AMCPayload(iamc), 
					hamc13->AMCSize(iamc));
				for (HcalUHTRData::const_iterator iuhtr=uhtr.begin(); 
					iuhtr!=uhtr.end(); ++iuhtr)
				{
					if (!iuhtr.isHeader())
						continue;

					_cuTCAOccupancy.fill(fed, eid);
					//	use data flavour - found in the unpacker
					if (iuhtr.flavor()==UTCA_DATAFLAVOR)
					{
						uint32_t l1a_uhtr = uhtr.l1ANumber();
						uint32_t bx_uhtr = uhtr.bunchNumber();
						uint32_t orn_uhtr = uhtr.orbitNumber();
						_cuTCAEvnMsm.fill(fed, eid, abs(l1a_uhtr-l1a));
						_cuTCABcnMsm.fill(fed, eid, abs(bx_uhtr-bx));
						_cuTCAOrnMsm.fill(fed, eid, abs(orn_uhtr-orn));
					}
				}
			}
		}
	}
}

/* virtual */ void RawTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

DEFINE_FWK_MODULE(RawTask);
