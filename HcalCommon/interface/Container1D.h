#ifndef Container1D_h
#define Container1D_h

/*
 *	file:		Container1D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		1D Container
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Axis.h"
#include "DQM/HcalCommon/interface/Mapper.h"
#include "Container.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class Container1D : public Container
	{
		public:
			Container1D() {}
			virtual ~Container1D() {}

			//	just to have here
			virtual void fill(double ) {}
			virtual void fill(int) {}
			//	using int as mapper or double
			virtual void fill(int, int);
			virtual void fill(int, double);
			virtual void fill(int, int, double);
			virtual void fill(int, double, double);

			//	using DetId as mapper
			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, int);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, int, double);
			virtual void fill(HcalDetId const&, double, double);

			//	using ElectronicsId as mapper
			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, int);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, int, double);
			virtual void fill(HcalElectronicsId const&, double, double);

			//	booking
			virtual bool book(DQMStore::IBooker&);
			virtual void book(DQMStore::IBooker&);

		protected:
			typedef	std::vector<MonitorElement*>	MEVector;
			MEVector								_mes;
			Mapper									_mapper;
			Axis									*_xaxis;
			Axis									*_yaxis;
	};
}

#endif








