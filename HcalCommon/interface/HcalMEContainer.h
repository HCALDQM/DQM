#ifndef HCALMECONTAINER_H
#define HCALMECONTAINER_H

/*
 *	file:		HcalMEContainer.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class HcalMEContainer
	{
		public:
			HcalMEContainer() {}
			virtual ~HcalMEContainer() {}

		protected:
			std::vector<MonitorElement*>			_mes;
	};
}

#endif








