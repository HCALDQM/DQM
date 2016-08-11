
#include "DQM/HcalTasks/interface/QIE10Task.h"
#include <map>

using namespace hcaldqm;
QIE10Task::QIE10Task(edm::ParameterSet const& ps):
	DQTask(ps)
{
	
	//	tags
	_tagQIE10 = ps.getUntrackedParameter<edm::InputTag>("tagQIE10",
		edm::InputTag("hcalDigis"));
	_tokQIE10 = consumes<QIE10DigiCollection>(_tagQIE10);

	//	cuts
	_cut = ps.getUntrackedParameter<double>("cut", 50.0);
	_ped = ps.getUntrackedParameter<int>("ped", 4);
}
/* virtual */ void QIE10Task::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	if (_ptype==fLocal)
		if (r.runAuxiliary().run()==1)
			return;

	DQTask::bookHistograms(ib, r, es);

	//	GET WHAT YOU NEED
	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);
	_emap = dbs->getHcalMapping();
	std::vector<uint32_t> vhashC36;
	vhashC36.push_back(HcalElectronicsId(36, 3,
		FIBER_uTCA_MIN1, FIBERCH_MIN, false).rawId());
	_filter_C36.initialize(filter::fPreserver, hashfunctions::fCrateSlot,
		vhashC36);

	//	create a Dual Anode filter
	std::vector<uint32_t> vhashDA;
	for (int depth=1; depth<=2; depth++)
		for (int ieta=29; ieta<=41; ieta++)
			vhashDA.push_back(HcalDetId(HcalForward, ieta, 39, depth));
	_filter_DA.initialize(filter::fPreserver, hashfunctions::fDChannel,
		vhashDA);

	//	INITIALIZE what you need
	_cShapeCut_EChannel.initialize(_name,
		"ShapeCut", hashfunctions::fEChannel,
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fQIE10fC_300000));
	_cShapeCut.initialize(_name,
		"ShapeCut", 
		new quantity::ValueQuantity(quantity::fTiming_TS),
		new quantity::ValueQuantity(quantity::fQIE10fC_300000));
	_cLETDCvsADC.initialize(_name, "LETDCvsADC",
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fQIE10TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cTETDCvsADC.initialize(_name, "TETDCvsADC",
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fQIE10TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cLETDC.initialize(_name, "LETDC",
		new quantity::ValueQuantity(quantity::fQIE10TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));
	_cADC.initialize(_name, "ADC",
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fN, true));
	_cOccupancy_depth.initialize(_name, hashfunctions::fdepth,
		new quantity::DetectorQuantity(quantity::fiphi),
		new quantity::DetectorQuantity(quantity::fieta),
		new quantity::ValueQuantity(quantity::fN, true));

	//	QIE8 vs 10 and 10 vs 10
	_cADCCorrelation10vs8.initialize(_name, "ADCCorrelation10vs8",
		new quantity::ValueQuantity(quantity::fADC_128),
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fN, true));
	_cADCCorrelation10vs10.initialize(_name, "ADCCorrelation10vs10",
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fQIE10ADC_256),
		new quantity::ValueQuantity(quantity::fN, true));
	_cLETDCCorrelation10vs10.initialize(_name, "LETDCCorrelation",
		new quantity::ValueQuantity(quantity::fQIE10TDC_64),
		new quantity::ValueQuantity(quantity::fQIE10TDC_64),
		new quantity::ValueQuantity(quantity::fN, true));

	int nTS = _ptype==fLocal ? 10 : 4;
	for (unsigned int j=0; j<nTS; j++)
	{
		_cLETDCvsADC_EChannel[j].initialize(_name,
			"LETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
		_cTETDCvsADC_EChannel[j].initialize(_name,
			"TETDCvsADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
		_cADC_EChannel[j].initialize(_name,
			"ADC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN, true));
		_cLETDC_EChannel[j].initialize(_name,
			"LETDC", hashfunctions::fEChannel,
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));

		_cADCCorrelation10vs8[j].initialize(_name, "ADCCorrelation10vs8",
			hashfunctions::fDChannel,
			new quantity::ValueQuantity(quantity::fADC_128),
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN, true));

		_cADCCorrelation10vs10_ieta30[j].initialize(_name, 
			"ADCCorrelation10vs10_ieta30",
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN, true));
		_cADCCorrelation10vs10_ieta34[j].initialize(_name, 
			"ADCCorrelation10vs10_ieta34",
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fQIE10ADC_256),
			new quantity::ValueQuantity(quantity::fN, true));
		_cLETDCCorrelation10vs10_ieta30[j].initialize(_name, "LETDCCorrelation_ieta30",
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
		_cLETDCCorrelation10vs10_ieta34[j].initialize(_name, "LETDCCorrelation_ieta34",
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fQIE10TDC_64),
			new quantity::ValueQuantity(quantity::fN, true));
	}

	_cShapeCut_EChannel.book(ib, _emap, _filter_C36, _subsystem);
	_cShapeCut.book(ib, _subsystem);
	_cLETDCvsADC.book(ib, _subsystem);
	_cTETDCvsADC.book(ib, _subsystem);
	_cLETDC.book(ib, _subsystem);
	_cADC.book(ib, _subsystem);
	_cADCCorrelation10vs8.book(ib, _subsystem);
	_cADCorrelation10vs10.book(ib, _subsystem);
	_cLETDCCorrelation10vs10.book(ib, _subsystem);
	for (unsigned int i=0; i<nTS; i++)
	{
		char aux[10];
		sprintf(aux, "TS%d", i);
		_cLETDCvsADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cTETDCvsADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cLETDC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);
		_cADC_EChannel[i].book(ib, _emap, _filter_C36, _subsystem, aux);

		_cADCCorrelation10vs8_DChannel[i].book(ib, _emap, _filter_DA, _subsystem, aux);
		_cADCCorrelation10vs10_ieta30[i].book(ib, _subsystem, aux);
		_cADCCorrelation10vs10_ieta34[i].book(ib, _subsystem, aux);
		_cLETDCCorrelation10vs10_ieta30[i].book(ib, _subsystem, aux);
		_cLETDCCorrelation10vs10_ieta34[i].book(ib, _subsystem, aux);
	}

	_ehashmap.initialize(_emap, electronicsmap::fD2EHashMap, _filter_C36);
}

/* virtual */ void QIE10Task::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{
	
	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE10Task::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<QIE10DigiCollection> cqie10;
	if (!e.getByToken(_tokQIE10, cqie10))
		return;

	std::map<uint32_t, QIE10DataFrame> mqie10;
	for (uint32_t i=0; i<cqie10->size(); i++)
	{
		QIE10DataFrame frame = static_cast<QIE10DataFrame>((*cqie10)[i]);
		HcalDetId did = frame.detid();
		HcalElectronicsId eid = HcalElectronicsId(_ehashmap.lookup(did));

		//	compute the signal, ped subracted
		double q = utilities::sumQ_v10<QIE10DataFrame>(frame,
			constants::adc2fC[_ped], 0, frame.samples()-1);

		//	iterate thru all TS and fill
		for (int j=0; j<frame.samples(); j++)
		{
			//	shapes are after the cut
			if (q>_cut)
			{
				_cShapeCut_EChannel.fill(eid, j, 
					constants::adc2fC[frame[j].adc()]);	
				_cShapeCut.fill(j, constants::adc2fC[frame[j].adc()]);
			}

			//	w/o a cut
			_cLETDCvsADC_EChannel[j].fill(eid, frame[j].adc(), 
				frame[j].le_tdc());
			_cLETDCvsADC.fill(frame[j].adc(), frame[j].le_tdc());
			_cTETDCvsADC_EChannel[j].fill(eid, frame[j].adc(), 
				frame[j].te_tdc());
			_cTETDCvsADC.fill(frame[j].adc(), frame[j].te_tdc());
			_cLETDC_EChannel[j].fill(eid, frame[j].le_tdc());
			_cLETDC.fill(frame[j].le_tdc());
			_cADC_EChannel[j].fill(eid, frame[j].adc());
			_cADC.fill(frame[j].adc());
		}

		//	Correlation histograms
		mqie10[did.rawId()] = frame;
	}

	//	Collect HF Digis
	std::map<uint32_t, HFDataFrame> mqie8;
	for (HFDigiCollection::const_iterator it=chf->begin(); it!=chf->end();
		++it)
	{
		//	we only need iphi 39
		if (!(it->id().iphi()==39 && it->id().ieta()>0))
			continue;

		mqie8[it->id().rawId()] = *it;
	}

	//	Now, we iterate thru qie10s and fill the correlation plots
	for (std::map<uint32_t, QIE10DataFrame>::const_iterator it=mqie10.begin();
		it!=mqie10.end(); ++it)
	{
		uint32_t rawid = it->first;
		//	1 is QIE10 
		HcalDetId did1 = HcalDetId(rawid);
		QIE10DataFrame frame1_10 = it->second;
		if ((did1.ieta()==30 || did1.ieta()==34) && 
			(did1.depth()==2 || did1.depth()==4)) // select the dualqie10 ro
		{
			if (did1.depth()==4) continue; // skip depth4, they are treated for depth2

			//	2 is QIE10
			HcalDetId did2 = HcalDetId(HcalForward, did1.ieta(), did1.iphi(),
				did1.depth()+2);
			QIE10DataFrame frame2_10 = mqie10[did2.rawId()];

			//	fill
			for (int j=0; j<frame.samples(); j++)
			{
				_cADCCorrelation10vs10.fill(frame2_10[j].adc(),
					frame1_10[j].adc());
				_cLETDCCorrelation10vs10.fill(frame2_10[j].le_tdc(),
					frame1_10[j].le_tdc());
				
				if (did2.ieta()==30)
				{
					_cADCCorrelation10vs10_ieta30[j].fill(frame2_10[j].adc(),
						frame1_10[j].adc());
					_cLETDCCorrelation10vs10_ieta30[j].fill(frame2_10[j].le_tdc(),
						frame1_10[j].le_tdc());
				}
				else if (did2.ieta()==34)
				{
					_cADCCorrelation10vs10_ieta34[j].fill(frame2_10[j].adc(),
						frame1_10[j].adc());
					_cLETDCCorrelation10vs10_ieta34[j].fill(frame2_10[j].le_tdc(),
						frame1_10[j].le_tdc());
				}
				else
					std::cout << "1111111111111" << std::endl;
			}
		}
		else 
		{
			//	2 is QIE8
			HcalDetId did2 = HcalDetId(HcalForward, did1.ieta(), did1.iphi(), 
				did1.depth()-2);
			HFDataFrame frame2_8 = mqie8[did2.rawId()];

			//	fill
			for (int j=0; j<frame.samples(); j++)
			{
				_cADCCorrelation10vs8.fill(frame2_8[j].adc(),
					frame1_10[j].adc());
				_cADCCorrelation10vs8_DChannel[j].fill(did2,
					frame2_8[j].adc(), frame1_10[j].adc());
			}
		}	

		
	}
}

/* virtual */ void QIE10Task::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10Task);
