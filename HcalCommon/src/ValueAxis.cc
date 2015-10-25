
#include "DQM/HcalCommon/interface/ValueAxis.h"

namespace hcaldqm
{
	using namespace constants;
	namespace axis
	{
		ValueAxis::ValueAxis():
			Axis(), _vtype(fEnergy)
		{}

		ValueAxis::ValueAxis(AxisType type, ValueType vtype, bool log):
			Axis(vtitle[vtype], type, fValue, vnbins[vtype],
				vmin[vtype], vmax[vtype], log), _vtype(vtype)
		{}

		ValueAxis::ValueAxis(AxisType type, ValueType vtype, int n,
			double min, double max, std::string title, bool log):
			Axis(title, type, fValue, n, min, max, log), _vtype(vtype)
		{}

		/* virtual */ void ValueAxis::_setup()
		{
		}
	}
}




