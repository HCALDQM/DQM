#ifndef HCALMECONTAINER_H
#define HCALMECONTAINER_H

/*
 *	file:		HcalMEContainer.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Base Class for all Container Types		
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

			virtual void fill(double x);
			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, double, double);
			virtual void fill(HcalDetId const&, double, double, double);

		protected:
			std::vector<MonitorElement*>			_mes;
	};
}

#endif








