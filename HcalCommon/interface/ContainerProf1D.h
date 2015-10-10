#ifndef ContainerProf1D_h
#define ContainerProf1D_h

/*
 *	file:		ContainerProf1D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TProfiles. 
 *		Direct Inheritance from Container1D + some more funcs
 *
 */

#include "DQM/HcalCommon/interface/ContainerProf1D.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class ContainerProf1D : public Container1D
	{
		public:
			HcalMEContainerProf1D() {}
			virtual ~HcalMEContainerProf1D() {}

			//	booking
			virtual bool book(DQMStore::IBooker&);
			virtual void book(DQMStore::IBooker&);

		protected:
	};
}


#endif








