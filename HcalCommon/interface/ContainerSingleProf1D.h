#ifndef ContainerSingleProf1D_h
#define ContainerSingleProf1D_h

/*
 *	file:			ContainerSignle1D.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Container to hold a single ME - for convenience of initialization
 */

#include "DQM/HcalCommon/interface/Container.h"
#include "DQM/HcalCommon/interface/ValueAxis.h"
#include "DQM/HcalCommon/interface/CoordinateAxis.h"

#include <string>

namespace hcaldqm
{
	using namespace axis;
	class ContainerSingleProf1D : public Container
	{
		public:
			ContainerSingleProf1D():
				Container()
			{}
			ContainerSingleProf1D(std::string const& folder, 
				std::string const& nametitle, 
				axis::Axis *xaxis,
				axis::Axis *yaxis = new ValueAxis(fYaxis, axis::fEntries));
			virtual ~ContainerSingleProf1D() {}

			//	booking
			virtual void book(DQMStore::IBooker&,
				std::string subsystem="Hcal", std::string aux="");

		protected:
			MonitorElement				*_me;
			Axis						*_xaxis;
			Axis						*_yaxis;
	};
}

#endif
