#ifndef RawTask_h
#define RawTask_h

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

using namespace hcaldqm;
using namespace hcaldqm::filter;
class RawTask : public DQTask
{
	public:
		RawTask(edm::ParameterSet const&);
		virtual ~RawTask() {}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		edm::InputTag		_tagFEDs;
		edm::EDGetTokenT<FEDRawDataCollection>	_tokFEDs;

		//	emap
		HcalElectronicsMap const* _emap;

		//	Filters
		HashFilter _filter_VME;
		HashFilter _filter_uTCA;
		HashFilter _filter_FEDsVME;
		HashFilter _filter_FEDsuTCA;

		Container2D _cEvnMsm_ElectronicsVME;
		Container2D _cBcnMsm_ElectronicsVME;
		Container2D _cOrnMsm_ElectronicsVME;
		Container2D _cEvnMsm_ElectronicsuTCA;
		Container2D _cBcnMsm_ElectronicsuTCA;
		Container2D _cOrnMsm_ElectronicsuTCA;

		Container2D _cOccupancy_ElectronicsVME;
		Container2D _cOccupancy_ElectronicsuTCA;
};

#endif
