
#include "DQM/SpecificTasks/interface/TPComparisonTask.h"

using namespace hcaldqm;
TPComparisonTask::TPComparisonTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	//	tags and tokens
	_tag1 = ps.getUntrackedParameter<edm::InputTag>("tag1",
		edm::InputTag("hcalDigis"));
	_tag2 = ps.getUntrackedParameter<edm::InputTag>("tag2",
		edm::InputTag("utcaDigis"));
	_tok1 = consumes<HcalTrigPrimDigiCollection>(_tag1);
	_tok2 = consumes<HcalTrigPrimDigiCollection>(_tag2);

	//	tmp flags
	_skip1x1 = ps.getUntrackedParameter<bool>("skip1x1", true);

	//	initialize COntainers
	for (unsigned int i=0; i<4; i++)
	{
		_cEt_TPSubDet[i].initialize(_name+"/Et/TPSubDet", "Et",
			mapper::fTPSubDet,
			new axis::ValueAxis(axis::fXaxis, axis::fEt_256),
			new axis::ValueAxis(axis::fYaxis, axis::fEt_256));
		_cFG_TPSubDet[i].initialize(_name+"/FG/TPSubDet", "FG",
			mapper::fTPSubDet,
			new axis::ValueAxis(axis::fXaxis, axis::fFG),
			new axis::ValueAxis(axis::fYaxis, axis::fFG));
	}
	_cMsn_depth.initialize(_name+"/Missing/depth", "Missing",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries));
	_cEtMsm_depth.initialize(_name+"/EtMismatches/depth", "EtMismatches",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries));
	_cFGMsm_depth.initialize(_name+"/FGMismatches/depth", "FGMismatches",
		mapper::fdepth,
		new axis::CoordinateAxis(axis::fXaxis, axis::fTPieta),
		new axis::CoordinateAxis(axis::fYaxis, axis::fiphi),
		new axis::ValueAxis(axis::fZaxis, axis::fEntries));
}

/* virtual */ void TPComparisonTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);

	char aux[20];
	for (unsigned int i=0; i<4; i++)
	{
		sprintf(aux, "_TS%d", i);
		_cEt_TPSubDet[i].book(ib, _subsystem, aux);
		_cFG_TPSubDet[i].book(ib, _subsystem, aux);
	}
	_cMsn_depth.book(ib);
	_cEtMsm_depth.book(ib);
	_cFGMsm_depth.book(ib);
}

/* virtual */ void TPComparisonTask::_resetMonitors(UpdateFreq uf)
{
	DQTask::_resetMonitors(uf);
}

/* virtual */ void TPComparisonTask::_process(edm::Event const& e,
	edm::EventSetup const& es)
{
	edm::Handle<HcalTrigPrimDigiCollection>	coll1;
	edm::Handle<HcalTrigPrimDigiCollection>	coll2;

	if (!e.getByToken(_tok1, coll1))
		_logger.dqmthrow(
			"Collection HcalTrigPrimDigiCollection isn't available" + 
			_tag1.label() + " " + _tag1.instance());
	if (!e.getByToken(_tok2, coll2))
		_logger.dqmthrow(
			"Collection HcalTrigPrimDigiCollection isn't available" + 
			_tag2.label() + " " + _tag2.instance());

	for (HcalTrigPrimDigiCollection::const_iterator it1=coll1->begin();
		it1!=coll1->end(); ++it1)
	{
		//	tmp
		if (_skip1x1)
			if (it1->id().depth()==10)
				continue;
		//	\tmp

		HcalTrigTowerDetId tid = it1->id();
		HcalTrigPrimDigiCollection::const_iterator it2=coll2->find(
			HcalTrigTowerDetId(tid.ieta(), tid.iphi(), 0));

		if (it2==coll2->end())
			_cMsn_depth.fill(tid);
		else
			for (int i=0; i<it1->size(); i++)
			{
				_cEt_TPSubDet[i].fill(tid, 
					it1->sample(i).compressedEt(), 
					it2->sample(i).compressedEt());
				_cFG_TPSubDet[i].fill(tid,
					it1->sample(i).fineGrain(),
					it2->sample(i).fineGrain());
				if (it1->sample(i).compressedEt()!=
					it2->sample(i).compressedEt())
					_cEtMsm_depth.fill(tid);
				if (it1->sample(i).fineGrain()!=
					it2->sample(i).fineGrain())
					_cFGMsm_depth.fill(tid);
			}
	}
}

/* virtual */ void TPComparisonTask::endLuminosityBlock(
	edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	//	in the end always
	DQTask::endLuminosityBlock(lb, es);
}

DEFINE_FWK_MODULE(TPComparisonTask);

