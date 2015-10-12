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

			//	Value Types
			fEntries = 9,
			fEvents = 10,
			fEnergy = 11,
			fTime = 12,

			//	Flag Types
			
			nAxisQType = 13
		};
		std::string const axisTitle[nAxisQType] = {
			"SubDetector", "iphi", "ieta", "depth", "FED",
			"Crate", "Slot", "Fiber", "FiberChannel",
			"Entries", "Events", "Energy (GeV)", "Time"};
		double const axisMin[nAxisQType] = {
			constants::HB, constants::IPHI_MIN-0.5, -constants::IETA_MAX-0.5, 
			constants::DEPTH_MIN-0.5, constants::FED_VME_MIN,
			constants::CRATE_VME_MIN, constants::SLOT_uTCA_MIN, 
			constants::FIBER_VME_MIN, constants::FIBERCH_MIN,
			0, 0, constants::AXIS_ENERGY_MIN, constants::AXIS_TIME_MIN
			};
		double const axisMax[nAxisQType] = {
			constants::HF+1, IPHI_MAX+0.5, constants::IETA_MAX+0.5, 
			constants::DEPTH_MAX+0.5, constants::FED_uTCA_MAX+1,
			constants::CRATE_uTCA_MAX+1, constants::SLOT_VME_MAX+1, 
			constants::FIBER_uTCA_MAX, constants::FIBERCH_MAX+1,
			3000, 0, constants::AXIS_ENERGY_MAX, constants::AXIS_TIME_MAX 
			};
		int const axisNbins[nAxisQType] = {
			constants::SUBDET_NUM, constants::IPHI_NUM, 
			2*constants::IETA_NUM+1, constants::DEPTH_NUM, 
			constants::FED_VME_NUM+constants::FED_uTCA_NUM, 
			constants::CRATE_VME_NUM+constants::CRATE_uTCA_NUM,
			constants::FIBER_VME_NUM+constants::FIBER_uTCA_NUM, 
			constants::FIBERCH_NUM, 500, 0,
			constants::AXIS_ENERGY_NBINS, constants::AXIS_TIME_NBINS};
		bool const axisLogs[nAxisQType] = {
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














