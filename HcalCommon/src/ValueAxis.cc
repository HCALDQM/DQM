
#include "DQM/HcalCommon/interface/ValueAxis.h"

namespace hcaldqm
{
	using namespace constants;
	namespace axis
	{
		/* virtual */ void ValueAxis::_setup()
		{
			char name[20];
			switch(_vtype)
			{
				case fSubDet:
					for (int i=HB; i<=HF; i++)
						_labels.push_back(SUBDET_NAME[i-1]);
					break;
				default:
					break;
			}
		}
	}
}




