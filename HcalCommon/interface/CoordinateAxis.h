#ifndef CoordinateAxis_h
#define CoordinateAxis_h

/*
 *	file:		CoordinateAxis.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *
 */

#include "DQM/HcalCommon/interface/Axis.h"

namespace hcaldqm
{
	namespace axis
	{
		using namespace hcaldqm::constants;
		enum CoordinateType
		{
			fSubDet = 0,
			fiphi = 1,
			fieta = 2,
			fdepth = 3,

			fFED = 4,
			fCrate = 5,
			fSlot = 6,
			fFiber = 7,
			fFiberCh = 8,

			nCoordinateType = 9
		};

		std::string const ctitle[nCoordinateType] = {
			"Sub Detector", "iphi", "ieta", "depth", "FED", "Crate", "Slot",
			"Fiber", "Fiber Channel"
		};
		double const cmin[nCoordinateType] = {
			HB, IPHI_MIN-0.5, 0, DEPTH_MIN-0.5,
			0, CRATE_VME_MIN, SLOT_uTCA_MIN, FIBER_VME_MIN, FIBERCH_MIN
		};
		double const cmax[nCoordinateType] = {
			HF+1, IPHI_MAX+0.5, 84, DEPTH_MAX+0.5,
			FED_VME_NUM+FED_uTCA_NUM, CRATE_uTCA_MAX+1, SLOT_VME_MAX+1,
			FIBER_uTCA_MAX+1, FIBERCH_MAX+1
		};
		int const cnbins[nCoordinateType] = {
			SUBDET_NUM, IPHI_NUM, 84, DEPTH_NUM, FED_VME_NUM+FED_uTCA_NUM,
			CRATE_uTCA_MAX+1-CRATE_VME_MIN, SLOT_VME_MAX+1-SLOT_uTCA_MIN,
			FIBER_uTCA_MAX+1-FIBER_VME_MIN, FIBERCH_NUM
		};

		class CoordinateAxis : public Axis
		{
			public:
				friend class hcaldqm::Container;
				friend class hcaldqm::Container1D;
				friend class hcaldqm::Container2D;
				friend class hcaldqm::ContainerProf1D;
				friend class hcaldqm::ContainerProf2D;
				friend class hcaldqm::ContainerSingle2D;

			public:
				CoordinateAxis();
				CoordinateAxis(AxisType, CoordinateType, bool log=false);
				CoordinateAxis(AxisType type, CoordinateType ctype, 
					int n, double min, double max, std::string title, 
					bool log=false);
				virtual ~CoordinateAxis() {}

				virtual int get(HcalDetId const&);
				virtual int get(HcalElectronicsId const&);
				virtual int get(int);

			protected:
				virtual void _setup();

			protected:
				CoordinateType _ctype;
		};
	}
}

#endif




