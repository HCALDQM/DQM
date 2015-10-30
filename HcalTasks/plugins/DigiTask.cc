
#include "DQM/HcalTasks/interface/DigiTask.h"

using namespace hcaldqm;
DigiTask::DigiTask(edm::ParameterSet const& ps):
	DQTask(ps),

	//	Containers
	_cADCperTS_SubDet(_name+"/ADC_SubDet", "ADC",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fADC)),
	_cfCperTS_SubDet(_name+"/fC_SubDet", "fC",
		mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC)),
	_cShape_SubDet_iphi(_name+"/Shape_SubDet_iphi", "Shape",
		mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cShapeSumQcut_SubDet_iphi(_name+"/ShapeSumQcut_SubDet_iphi", "ShapeSumQcut",
		mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cSumQ_SubDet_iphi(_name+"/SumQ_SubDet_iphi", "SumQ",
		mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fNomFC)),
	_cTimingSumQcut_SubDet_iphi(_name+"/TimingSumQcut_SubDet_iphi", 
		"TimingSumQcut", mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fTimeTS_200)),
	_cOccupancyvsiphi_SubDet(_name+"/Occupancyvsiphi_SubDet", "Occupancyvsiphi",
		mapper::fSubDet,
		new axis::CoordinateAxis(fXaxis, axis::fiphi)),
	_cSumQvsLS_SubDet_iphi(_name+"/SumQvsLS_SubDet_iphi", "SumQvsLS",
		mapper::fSubDet_iphi,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::ValueAxis(axis::fYaxis, axis::fNomFC)),
	_cTimingSumQcutvsieta_SubDet_iphi(_name+"/TimingSumQcutvsieta_SubDet_iphi",
		"TimingSumQcutvsieta", mapper::fSubDet_iphi,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::ValueAxis(axis::fYaxis, axis::fTimeTS)),
	_cTimingSumQcutvsiphi_SubDet_ieta(_name+"/TimingSumQcutvsiphi_SubDet_ieta",
		"TimingSumQcutvsiphi", mapper::fSubDet_ieta,
		new axis::CoordinateAxis(axis::fXaxis, axis::fiphi),
		new axis::ValueAxis(axis::fYaxis, axis::fTimeTS)),
	_cOccupancy_depth(_name+"/Occupancy_depth", "Occupancy",
		mapper::fdepth, 
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta), 
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cOccupancyiphivsLS_SubDet(_name+"/OccupancyiphivsLS_SubDet", 
		"OccupancyiphivsLS", mapper::fSubDet,
		new axis::ValueAxis(axis::fXaxis, axis::fLS),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi)),
	_cSumQ_depth(_name+"/SumQ", "SumQ",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fNomFC)),
	_cTimingSumQcut_depth(_name+"/TimingSumQcut", "TimingSumQcut",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fTimeTS))
{
	_tagHBHE = ps.getUntrackedParameter<edm::InputTag>("tagHBHE",
		edm::InputTag("hcalDigis"));
	_tagHO = ps.getUntrackedParameter<edm::InputTag>("tagHO",
		edm::InputTag("hcalDigis"));
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void DigiTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);
	_cADCperTS_SubDet.book(ib);
	_cfCperTS_SubDet.book(ib);
	_cShape_SubDet_iphi.book(ib);
	_cShapeSumQcut_SubDet_iphi.book(ib);
	_cSumQ_SubDet_iphi.book(ib);
	_cTimingSumQcut_SubDet_iphi.book(ib);
	_cOccupancyvsiphi_SubDet.book(ib);
	_cSumQvsLS_SubDet_iphi.book(ib);
	_cTimingSumQcutvsieta_SubDet_iphi.book(ib);
	_cTimingSumQcutvsiphi_SubDet_ieta.book(ib);
	_cOccupancyiphivsLS_SubDet.book(ib);
	_cOccupancy_depth.book(ib);
	_cSumQ_depth.book(ib);
	_cTimingSumQcut_depth.book(ib);
}

/* virtual */ void DigiTask::_resetMonitors(int pflag)
{
	DQTask::_resetMonitors(pflag);
	if (pflag==0)
	{
		for (unsigned int i=0; i<constants::SUBDET_NUM; i++)
			_numDigis[i]=0;
	}
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

		//	fill without a cut
		_cOccupancy_depth.fill(digi.id());
//		_cOccupancyiphivsLS_SubDet.fill(digi.id(), _currentLS);
		_cSumQ_SubDet_iphi.fill(digi.id(), sumQ);
		_cSumQ_depth.fill(digi.id(), sumQ);
		_cSumQvsLS_SubDet_iphi.fill(digi.id(), _currentLS, sumQ);

		//	fill with a cut
		if (sumQ>20)
		{
			_cTimingSumQcut_SubDet_iphi.fill(digi.id(), timing);
			_cTimingSumQcutvsieta_SubDet_iphi.fill(digi.id(),
				timing);
			_cTimingSumQcutvsiphi_SubDet_ieta.fill(digi.id(),
				timing);
			_cTimingSumQcut_depth.fill(digi.id(), timing);
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(digi.id(), digi.sample(i).adc());
			_cfCperTS_SubDet.fill(digi.id(), digi.sample(i).nominal_fC());
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);

			//	with a cut
			if (sumQ>20)
			{
				_cShapeSumQcut_SubDet_iphi.fill(digi.id(), i,
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

		//	fill without a cut
		_cOccupancy_depth.fill(digi.id());
//		_cOccupancyiphivsLS_SubDet.fill(digi.id(), _currentLS);
		_cSumQ_SubDet_iphi.fill(digi.id(), sumQ);
		_cSumQ_depth.fill(digi.id(), sumQ);
		_cSumQvsLS_SubDet_iphi.fill(digi.id(), _currentLS, sumQ);

		//	fill with a cut
		if (sumQ>20)
		{
			_cTimingSumQcut_SubDet_iphi.fill(digi.id(), timing);
			_cTimingSumQcutvsieta_SubDet_iphi.fill(digi.id(),
				timing);
			_cTimingSumQcutvsiphi_SubDet_ieta.fill(digi.id(),
				timing);
			_cTimingSumQcut_depth.fill(digi.id(), timing);
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(digi.id(), digi.sample(i).adc());
			_cfCperTS_SubDet.fill(digi.id(), digi.sample(i).nominal_fC());
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-8.5);

			//	with a cut
			if (sumQ>20)
			{
				_cShapeSumQcut_SubDet_iphi.fill(digi.id(), i,
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

		//	fill without a cut
		_cOccupancy_depth.fill(digi.id());
//		_cOccupancyiphivsLS_SubDet.fill(digi.id(), _currentLS);
		_cSumQ_SubDet_iphi.fill(digi.id(), sumQ);
		_cSumQ_depth.fill(digi.id(), sumQ);
		_cSumQvsLS_SubDet_iphi.fill(digi.id(), _currentLS, sumQ);

		//	fill with a cut
		if (sumQ>20)
		{
			_cTimingSumQcut_SubDet_iphi.fill(digi.id(), timing);
			_cTimingSumQcutvsieta_SubDet_iphi.fill(digi.id(),
				timing);
			_cTimingSumQcutvsiphi_SubDet_ieta.fill(digi.id(),
				timing);
			_cTimingSumQcut_depth.fill(digi.id(), timing);
		}
		
		//	per TS
		for (int i=0; i<digi.size(); i++)
		{
			//	without a cut
			_cADCperTS_SubDet.fill(digi.id(), digi.sample(i).adc());
			_cfCperTS_SubDet.fill(digi.id(), digi.sample(i).nominal_fC());
			_cShape_SubDet_iphi.fill(digi.id(), i, 
				digi.sample(i).nominal_fC()-2.5);

			//	with a cut
			if (sumQ>20)
			{
				_cShapeSumQcut_SubDet_iphi.fill(digi.id(), i,
					digi.sample(i).nominal_fC()-2.5);
			}
		}
	}
}

DEFINE_FWK_MODULE(DigiTask);


