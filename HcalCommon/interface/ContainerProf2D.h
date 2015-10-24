#ifndef ContainerProf2D_h
#define ContainerProf2D_h

/*
 *	file:		ContainerProf2D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TProfile or like
 *
 */

#include "DQM/HcalCommon/interface/Container2D.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	using namespace axis;
	class ContainerProf2D : public Container2D
	{
		public:
			ContainerProf2D() {}
			ContainerProf2D(std::string const& folder, 
				std::string const &nametitle,
				MapperType mt, Axis *xaxis, 
				Axis *yaxis = new CoordinateAxis(fYaxis, fiphi), 
				Axis *zaxis = new ValueAxis(fZaxis, fEntries)):
				Container2D(folder, nametitle, mt, xaxis, yaxis, zaxis)
			{}
			virtual ~ContainerProf2D() {}

			virtual void book(DQMStore::IBooker&, std::string subsystem="Hcal");

		protected:
	};
}


#endif








