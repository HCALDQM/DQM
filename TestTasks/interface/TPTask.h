#ifndef TPTask_h
#define TPTask_h

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
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

using namespace hcaldqm;
using namespace hcaldqm::filter;
class TPTask : public DQTask
{
	public:
		TPTask(edm::ParameterSet const&);
		virtual ~TPTask() {}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

		enum TPFlag
		{
			fOcpUniData = 0,
			fOcpUniEmul = 1,
			fEtMsmUni = 2,
			fFGMsmUni = 3,
		};

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		edm::InputTag		_tagData;
		edm::InputTag		_tagEmul;
		edm::EDGetTokenT<HcalTrigPrimDigiCollection> _tokData;
		edm::EDGetTokenT<HcalTrigPrimDigiCollection> _tokEmul;

		//	switches
		bool _skip1x1;

		//	emap
		HcalElectronicsMap const* _emap;
		electronicsmap::ElectronicsMap _ehashmap;

		//	Filters
		HashFilter _filter_VME;
		HashFilter _filter_uTCA;
		HashFilter _filter_depth0;

		//	Et/FG
		Container1D _cEtData_TTSubdet;
		Container1D _cEtEmul_TTSubdet;
		Container2D	_cEtCorr_TTSubdet;
		Container2D _cFGCorr_TTSubdet;

		//	Shape
		Container1D _cShapeData_TTSubdet;
		Container1D _cShapeEmul_TTSubdet;

		//	Occupancies
		Container2D _cOccupancyData_ElectronicsVME;
		Container2D _cOccupancyData_ElectronicsuTCA;
		Container2D _cOccupancyEmul_ElectronicsVME;
		Container2D _cOccupancyEmul_ElectronicsuTCA;

		//	Mismatches: Et and FG
		Container2D _cEtMsm_ElectronicsVME;
		Container2D _cEtMsm_ElectronicsuTCA;
		Container2D _cFGMsm_ElectronicsVME;
		Container2D _cFGMsm_ElectronicsuTCA;

		//	Missing Data w.r.t. Emulator
		Container2D _cMsnData_ElectronicsVME;
		Container2D _cMsnData_ElectronicsuTCA;

		//	Missing Emulator w.r.t. Data
		Container2D _cMsnEmul_ElectronicsVME;
		Container2D _cMsnEmul_ElectronicsuTCA;
};

#endif
