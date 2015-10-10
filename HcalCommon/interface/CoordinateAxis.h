#ifndef CoordinateAxis_h
#define CoordinateAxis_h

/*
 *	file:			CoordinateAxis.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *
 *
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

namespace hcaldqm
{
	namespace axis
	{
		enum CoordinateType
		{
			fSubDet = 0
			fiphi=1,
			fieta=2,
			fdepth=3,
	
			fFED = 4,
			fCrate = 5,
			fSlot = 6,
			fFiber = 7,
			fFiberCh = 8,
	
			nCoordinateType = 9
		};
	
		class CoordinateAxis : public Axis
		{
			public:
				CoordinateAxis() {}
				CoordinateAxis(AxisType, CoordinateType);
				CoordinateAxis(AxisType, CoordinateType, std::string const&);
				CoordianteAxis(AxisType, CoordinateType, std::string const&,
					int, double, double, bool log=false);
				virtual ~CoordinateAxis() {}
	
				vritual void customize(TAxis*, TObject*);
				virtual int resolve(HcalDetId const&);
				virtual int resolve(HcalElectronicsId const&);
	
			protected:
				CoordinateType			_ctype;
		};
	}
}

#endif














