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

#include "DQM/HcalCommon/interface/Container1D.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	using namespace mapper,axis;
	class ContainerProf1D : public Container1D
	{
		public:
			ContainerProf1D() {}
			ContainerProf1D(std::string const& folder, 
				std::string const& nametitle, MapperType mt, 
				AxisQType xvt=fEnergy, AxisQType yvt):
				Containter1D(folder, nametitle, mt, xvt, yvt)
			{}
			virtual ~HcalMEContainerProf1D() {}

			//	booking
			virtual void book(DQMStore::IBooker&);

		protected:
	};
}


#endif








