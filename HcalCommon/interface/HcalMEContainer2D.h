#ifndef HCALMECONTAINER2D_H
#define HCALMECONTAINER2D_H

/*
 *	file:		HcalMEContainer2D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TH2D or like
 *
 */

#include "DQM/HcalCommon/interface/HcalMEContainer.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class HcalMEContainer2D : public HcalMEContainer
	{
		public:
			HcalMEContainer1D() {}
			virtual ~HcalMEContainer1D() {}

		protected:
			
	};
}


#endif








