#ifndef Axis_h
#define Axis_h

/*
 *	file:			Axis.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *
 *
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

#include <string>

namespace hcaldqm
{
	namespace axis
	{
		using namespace hcaldqm::constants;

		/*
		 *	Helper Custom Types
		 *	1) AxisType - xyz
		 *	2) AxisQ - generic Type: Value/Coordinate/Flag
		 *	3) Specific things...
		 */
		enum AxisType
		{
			fXaxis = 0,
			fYaxis = 1,
			fZaxis = 2,
			nAxisType = 3
		};

		enum AxisQ
		{
			fValue = 0,
			fCoordinate = 1,
			fFlag = 2,
			nAxisQ = 3
		};
	
		enum AxisQType
		{
			//	Coordinate Quantities
			fSubDet = 0,
			fiphi = 1,
			fieta = 2,
			fdepth = 3,
			fFED = 4,
			fCrate = 5,
			fSlot = 6,
			fFiber = 7,
			fFiberCh = 8,
			fCoordinateLast = 9,

			//	Value Types
			fEntries = 10,
			fEvents = 11,
			fEnergy = 12,
			fTime = 13,
			fADC_PED = 14,
			fADC_FULL = 15,
			fNomFC_3000 = 16,
			fNomFC_Full = 17,
			fTimeTS = 18,
			fValueLast = 19,

			//	Flag Types
			
			nAxisQType = 20
		};
		std::string const axisTitle[nAxisQType] = {
			"SubDetector", "iphi", "ieta", "depth", "FED",
			"Crate", "Slot", "Fiber", "FiberChannel",
			"Entries", "Events", "Energy (GeV)", "Time",
			"ADC", "ADC", "nominal fC", "nominal fC", "Time Slice"};
		double const axisMin[nAxisQType] = {
			HB, IPHI_MIN-0.5, -IETA_MAX-0.5, DEPTH_MIN-0.5, 
			0,
			CRATE_VME_MIN, 
			SLOT_uTCA_MIN, 
			FIBER_VME_MIN, 
			FIBERCH_MIN,
			0, 0, constants::AXIS_ENERGY_MIN, constants::AXIS_TIME_MIN,
			AXIS_ADC_MIN, AXIS_ADC_MIN, AXIS_NOMFC_MIN, AXIS_NOMFC_MIN,
			AXIS_TIMETS_MIN
			};
		double const axisMax[nAxisQType] = {
			HF+1, IPHI_MAX+0.5, IETA_MAX+0.5, DEPTH_MAX+0.5, 
			FED_VME_NUM+FED_uTCA_NUM,
			CRATE_uTCA_MAX+1, 
			SLOT_VME_MAX+1, 
			FIBER_uTCA_MAX+1, 
			FIBERCH_MAX+1,
			3000, 0, constants::AXIS_ENERGY_MAX, constants::AXIS_TIME_MAX,
			AXIS_ADC_MAX_PED, AXIS_ADC_MAX, AXIS_NOMFC_MAX_3000, AXIS_NOMFC_MAX,
			AXIS_TIMETS_MAX
			};
		int const axisNbins[nAxisQType] = {
			SUBDET_NUM, IPHI_NUM, IETA_NUM, DEPTH_NUM, 
			FED_VME_NUM+FED_uTCA_NUM, 
			CRATE_uTCA_MAX+1-CRATE_VME_MIN,
			SLOT_VME_MAX+1-SLOT_uTCA_MIN,
			FIBER_uTCA_MAX+1-FIBER_VME_MIN, 
			FIBERCH_NUM, 
			500, 0, AXIS_ENERGY_NBINS, AXIS_TIME_NBINS, 
			AXIS_ADC_NBINS_PED, AXIS_ADC_NBINS, AXIS_NOMFC_NBINS_3000,
			AXIS_NOMFC_NBINS, AXIS_TIMETS_NBINS
			};
		bool const axisLogs[nAxisQType] = {
			false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false};
	
		/*
		 *	Base Class for Axis.
		 */
		class Axis
		{
			public:
				Axis();
				Axis(AxisType, AxisQType);
				virtual ~Axis() {}

				virtual int resolve(HcalDetId const&);
				virtual int resolve(HcalElectronicsId const&);
				virtual int resolve(int);
				virtual int resolve(double) { return 0;}
				virtual AxisQ getAxisQ(); 
	
				//	
				void setAxisLog(TObject*);
	
				AxisType			_type;
				AxisQType			_qtype;
				std::string			_title;
				int					_nbins;
				double				_min;
				double				_max;
				bool				_log;
				std::vector<std::string>	_labels;
		};
	}
}

#endif














