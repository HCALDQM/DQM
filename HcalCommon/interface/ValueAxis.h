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

namespace hcaldqm
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

		protected:
			ValueType			_valuetype;
	};
}

#endif














