
#include "DQM/HcalCommon/interface/ValueAxis.h"

namespace hcaldqm
{
	ValueAxis::ValueAxis():
		Axis(), _valuetype(fEnergy)
	{}

	ValueAxis::ValueAxis(AxisType at, ValueType vt):
		Axis(at), _valuetype(vt)
	{}

	ValueAxis::ValueAxis(AxisType at, ValueType vt, std::string const& s):
		Axis(at, s), _valuetype(vt)
	{}

	ValueAxis::ValueAxis(AxisType at, ValueType vt, std::string const& s,
		int nbins, double min, double max)
}







