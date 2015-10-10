#ifndef ValueAxis_h
#define ValueAxis_h

/*
 *	file:			ValueAxis.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *
 *
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Axis.h"

namespace hcaldqm
{
	namespace axis
	{
		enum ValueType
		{
			fEnergy = 0.
			fTime = 1,
	
	
			nValueType = 2
		};
	
		class ValueAxis : public Axis
		{
			public:
				ValueAxis();
				ValueAxis(AxisType, ValueType);
				ValueAxis(AxisTYpe, ValueType, std::string const&);
				ValueAxis(AxisType, ValueType, std::string const&,
					int, double, double, bool log=false);
				virtual ~ValueAxis() {}
	
				virtual void customize(TAxis*, TObject*);
				virtual int resolve(HcalDetId const&);
				virtual int resolve(HcalElectronicsId const&);
	
			protected:
				ValueType			_valuetype;
		};
	}
}

#endif














