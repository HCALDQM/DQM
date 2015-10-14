#ifndef ContainerSingle2D_h
#define ContainerSingle2D_h

/*
 *	file:			ContainerSignle2D.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Container to hold a single ME - for convenience of initialization
 */

#include "DQM/HcalCommon/interface/Container.h"
#include "DQM/HcalCommon/interface/Axis.h"

#include <string>

namespace hcaldqm
{
	using namespace axis;
	class ContainerSingle2D : public Container
	{
		public:
			ContainerSingle2D():
				Container()
			{}
			ContainerSingle2D(std::string const& folder, 
				std::string cosnt& nametitle, axis::AxisQType xvt,
				axis::AxisQType yvt, axis::AxisQType zvt=axis::fEntries):
				Container(folder, nametitle),
				_xaxis(xvt), _yaxis(yvt), _zaxis(zvt)
			{}
			~ContainerSignle2D() {}

			//	booking
			virtual void book(DQMStore::IBooker&);

			//	filling
			virtual void fill(int, int);
			virtual void fill(int, double);
			virtual void fill(int, double, double);
			virtual void fill(double, int);
			virtual void fill(double, double);
			virtual void fill(double, double, double);

			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, double, double);

			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual void fill(HcalDetId const&, HcalElectronicsId const&);
			virtual void fill(HcalDetId const&, HcalElectronicsId const&, double);

		protected:
			MonitorElement				*_me;
			Axis						_xaxis;
			Axis						_yaxis;
			Axis						_zaxis;
	};
}

#endif
