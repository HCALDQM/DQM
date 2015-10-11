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

#include <string>

namespace hcaldqm
{
	namespace axis
	{
		using namespace constants;
		enum AxisType
		{
			fXaxis = 0,
			fYaxis = 1.
			fZaxis = 2,
			nAxisType = 3
		};

		enum AxisQ
		{
			fQuantity = 0,
			fCoordinate = 1,
			fFlag = 2,
			nAxisQ = 3;
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

			//	Value Types
			fEntries = 9,
			fEvents = 10,
			fEnergy = 11,
			fTime = 12,

			//	Flag Types
			
			nAxisQType = 13
		};
		std::string axisTitle[nAxisQType] = {
			"SubDetector", "iphi", "ieta", "depth", "FED",
			"Crate", "Slot", "Fiber", "FiberChannel",
			"Entries", "Events", "Energy (GeV)", "Time"};
		double axisMin[nAxisQType] = {
			HB, IPHI_MIN-0.5, -IETA_MAX-0.5, DEPTH_MIN-0.5, FED_VME_MIN,
			CRATE_VME_MIN, SLOT_uTCA_MIN, FIBER_VME_MIN, FIBERCH_MIN,
			0, 0, AXIS_ENERGY_MIN, AXIS_TIME_MIN
			};
		double axisMax[nAxisQType] = {
			HF+1, IPHI_MAX+0.5, IETA_MAX+0.5, DEPTH_MAX+0.5, FED_uTCA_MAX+1,
			CRATE_uTCA_MAX+1, SLOT_VME_MAX+1, FIBER_uTCA_MAX, FIBERCH_MAX+1,,
			3000, 0, AXIS_ENERGY_MAX, AXIS_TIME_MAX 
			};
		int axisNbins[nAxisQType] = {
			SUBDET_NUM, IPHI_NUM, 2*IETA_NUM+1, DEPTH_NUM, 
			FED_VME_NUM+FED_uTCA_NUM, CRATE_VME_NUM+CRATE_uTCA_NUM,
			FIBER_VME_NUM+FIBER_uTCA_NUM, FIBERCH_NUM, 500, 0
			AXIS_ENERGY_NBINS, AXIS_TIME_NBINS};
		bool axisLogs[nAxisQType] = {
			false, false, false, false, false, false, false, false,
			false, false, false, false};
	
		class Axis
		{
			public:
				Axis();
				Axis(AxisType, AxisQType);
				virtual ~Axis() {}

				virtual int resolve(HcalDetId const&);
				virtual int resolve(HcalElectronicsId const&);
				virtual int resolve(int) { return 0;}
				virtual int resolve(double) { return 0;}
				virtual AxisQ getAxisQ(); 
	
				//	
				void setAxisLog(TObject*);
				void setAxisTitle(TAxis*);
				void setAxisBins(TAxis*);
	
				AxisType			_type;
				AxisQType			_qtype;
				std::string			_title;
				int					_nbins;
				double				_min;
				double				_max;
				double				*_bins;
				bool				_log;
		};
	}
}

#endif














