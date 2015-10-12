#ifndef Container1D_h
#define Container1D_h

/*
 *	file:		Container1D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		1D Container
 */

#include "DQM/HcalCommon/interface/Container.h"
#include "DQM/HcalCommon/interface/Axis.h"
#include "DQM/HcalCommon/interface/Mapper.h"

#include <vector>
#include <string>

namespace hcaldqm
{

	using namespace axis;
	using namespace mapper;
	class Container1D : public Container
	{
		public:
			Container1D() {}
			Container1D(std::string const& folder, std::string const& nametitle,
				mapper::MapperType mt, axis::AxisQType xvt=axis::fEnergy, 
				axis::AxisQType yvt=axis::fEntries):
				Container(folder, nametitle), _title(nametitle),
				_mapper(mt), _xaxis(fXaxis, xvt), _yaxis(fYaxis, yvt)
			{}
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
			virtual void book(DQMStore::IBooker&);

		protected:
			typedef	std::vector<MonitorElement*>	MEVector;
			MEVector								_mes;
			std::string								_title;
			mapper::Mapper									_mapper;
			axis::Axis									_xaxis;
			axis::Axis									_yaxis;

	};
}

#endif








