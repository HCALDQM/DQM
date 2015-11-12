#ifndef FlagAxis_h
#define FlagAxis_h

/*
 *	file:			FlagAxis.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 */

#include "DQM/HcalCommon/interface/Axis.h"

namespace hcaldqm
{
	namespace axis
	{
		using namespace hcaldqm::constants;
		class FlagAxis : public Axis
		{
			public:
				friend class hcaldqm::Container;
				friend class hcaldqm::Container1D;
				friend class hcaldqm::Container2D;
				friend class hcaldqm::ContainerProf1D;
				friend class hcaldqm::ContainerProf2D;
				friend class hcaldqm::ContainerSingle1D;
				friend class hcaldqm::ContainerSingle2D;

			public:
				FlagAxis();
				FlagAxis(AxisType, FlagType, int, int, int, std::string);
				virtual ~FlagAxis() {}

				virtual loadLabels(std::vector<std::string> const&);

			protected:
				virtual void _setup();
		};
	}
}

#endif
