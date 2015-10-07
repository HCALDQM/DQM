#ifndef HCALMECONTAINER1D_H
#define HCALMECONTAINER1D_H

/*
 *	file:		HcalMEContainer1D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TH1D or like
 *
 */

#include "DQM/HcalCommon/interface/HcalMEContainer.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class HcalMEContainer1D : public HcalMEContainer
	{
		public:
			HcalMEContainer1D() {}
			virtual ~HcalMEContainer1D() {}

		protected:
			
	};
}


#endif








