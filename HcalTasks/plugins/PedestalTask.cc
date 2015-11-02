
#include "DQM/HcalTasks/interface/PedestalTask.h"

using namespace hcaldqm;
PedestalTask::PedestalTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Containers
	_cPedestalMeans_SubDet(_name+"/PedestalMeans/SubDet", "PedestalMeans",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fADC_15),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cPedestalRMSs_SubDet(_name+"/PedestalRMSs/SubDet", "PedestalRMSs",
		mapper::fSubDet, 
		new axis::ValueAxis(axis::fXaxis, axis::fADC_5),
		new axis::ValueAxis(axis::fYaxis, axis::fEntries, true)),
	_cPedestalMeans_depth(_name+"/PedestalMeans/depth", "PedestalMeans",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cPedestalRMSs_depth(_name+"/PedestalRMSs/depth", "PedestalRMSs",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi))
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
	_tagTrigger = ps.getUntrackedParameter<edm::InputTag>("tagTrigger",
		edm::InputTag("tbunpacker"));
}

/* virtual */ void PedestalTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cPedestalMeans_SubDet.book(ib, _subsystem);
	_cPedestalRMSs_SubDet.book(ib, _subsystem);
	_cPedestalMeans_depth.book(ib, _subsystem);
	_cPedestalRMSs_depth.book(ib, _subsystem);
}

/* virtual */ void PedestalTask::_resetMonitors(int pflag)
{
	DQTask::_resetMonitors(pflag);
}

/* virtual */ void PedestalTask::_dump()
{
	_cPedestalMeans_SubDet.reset();
	_cPedestalRMSs_SubDet.reset();
	_cPedestalMeans_depth.reset();
	_cPedestalRMSs_depth.reset();
	_cPedestals.dump(&_cPedestalMeans_SubDet, true);
	_cPedestals.dump(&_cPedestalRMSs_SubDet, false);
	_cPedestals.dump(&_cPedestalMeans_depth, true);
	_cPedestals.dump(&_cPedestalRMSs_depth, false);
}

/* virtual */ void PedestalTask::_process(edm::Event const& e,
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
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		_cPedestals.fill(digi.id(), utilities::aveADC<HBHEDataFrame>(digi,
			0, 0, digiSizeToUse));
	}
	for (HODigiCollection::const_iterator it=cho->begin();
		it!=cho->end(); ++it)
	{
		const HODataFrame digi = (const HODataFrame)(*it);
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		_cPedestals.fill(digi.id(), utilities::aveADC<HODataFrame>(digi, 0, 0, 
			digiSizeToUse));
	}
	for (HFDigiCollection::const_iterator it=chf->begin();
		it!=chf->end(); ++it)
	{
		const HFDataFrame digi = (const HFDataFrame)(*it);
		int digiSizeToUse = floor(digi.size()/constants::CAPS_NUM)*
			constants::CAPS_NUM-1;
		_cPedestals.fill(digi.id(), utilities::aveADC<HFDataFrame>(digi, 0, 0, 
			digiSizeToUse));
	}

	if (_ptype==fOnline && _evsTotal>0 && 
		_evsTotal%constants::CALIBEVENTS_MIN==0)
		this->_dump();
}

/* virtual */ bool PedestalTask::_isApplicable(edm::Event const& e)
{
	if (_ptype==fOnline)
	{
		//	online-global
		return this->_getCalibType(e)==hc_Pedestal;
	}
	else 
	{
		//	local
		edm::Handle<HcalTBTriggerData>	ctrigger;
		if (!e.getByLabel(_tagTrigger, ctrigger))
			this->_throw("Collection HcalTBTriggerData isn't available",
				" " + _tagTrigger.label() + " " + _tagTrigger.instance());
		return ctrigger->wasSpillIgnorantPedestalTrigger();
	}

	return false;
}

DEFINE_FWK_MODULE(PedestalTask);


