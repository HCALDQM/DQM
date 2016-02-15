#ifndef DigiTask_h
#define DigiTask_h

/**
 *	file:
 *	Author:
 *	Description:
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/HashFilter.h"
#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerProf1D.h"
#include "DQM/HcalCommon/interface/ContainerProf2D.h"
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf2D.h"
#include "DQM/HcalCommon/interface/ContainerXXX.h"

using namespace hcaldqm;
using namespace hcaldqm::filter;
class DigiTask : public DQTask
{
	public:
		DigiTask(edm::ParameterSet const&);
		virtual ~DigiTask() {}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

		enum DigiFlag
		{
			fLowOcp = 0, // per subdet
			fUniSlot = 1, // occupancy uniformity vs Slots per FED
			fMsn1LS = 2, // missing for 1 LS
			fCapIdRot = 3, // caps non rotating
			nDigiFlag = 4
		};

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		edm::InputTag		_tagHBHE;
		edm::InputTag		_tagHO;
		edm::InputTag		_tagHF;
		edm::EDGetTokenT<HBHEDigiCollection> _tokHBHE;
		edm::EDGetTokenT<HODigiCollection>	 _tokHO;
		edm::EDGetTokenT<HFDigiCollection>	_tokHF;

		double _cutSumQ_HBHE, _cutSumQ_HO, _cutSumQ_HF;

		//	emap
		HcalElectronicsMap const* _emap;

		//	Filters
		HashFilter _filter_VME;
		HashFilter _filter_uTCA;
		HashFilter _filter_FEDsVME;
		HashFilter _filter_FEDsuTCA;
		HashFilter _filter_FEDHF;

		//	Containers
		Container1D _cADC_SubdetPM;
		Container1D _cfC_SubdetPM;
		Container1D _cSumQ_SubdetPM;
		ContainerProf2D	_cSumQ_depth;
		ContainerProf1D _cSumQvsLS_FEDSlot;

		//	Shape
		Container1D _cShapeCut_FEDSlot;

		//	Timing
		Container1D		_cTimingCut_FEDSlot;
		ContainerProf2D _cTimingCut_FEDVME;
		ContainerProf2D	_cTimingCut_FEDuTCA;
		ContainerProf2D _cTimingCut_ElectronicsVME;
		ContainerProf2D _cTimingCut_ElectronicsuTCA;
		ContainerProf1D _cTimingCutvsLS_FEDSlot;

		ContainerProf1D _cQ2Q12CutvsLS_FEDHFSlot;

		Container2D _cOccupancy_FEDVME;
		Container2D _cOccupancy_FEDuTCA;
		Container2D _cOccupancy_ElectronicsVME;
		Container2D _cOccupancy_ElectronicsuTCA;

		Container2D _cOccupancyCut_FEDVME;
		Container2D _cOccupancyCut_FEDuTCA;
		Container2D _cOccupancyCut_ElectronicsVME;
		Container2D _cOccupancyCut_ElectronicsuTCA;

		ContainerProf1D _cOccupancyvsLS_Subdet;
		ContainerProf1D _cOccupancyCutvsLS_Subdet;

		Container2D _cCapIdRots_FEDVME;
		Container2D _cCapIdRots_FEDuTCA;

		Container2D _cMissing1LS_FEDVME;
		Container2D _cMissing1LS_FEDuTCA;

		ContainerSingle2D _cSummary;
};

#endif