#ifndef Container2D_h
#define Container2D_h

/*
 *	file:		Container2D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TH2D or like
 *
 */

#include "DQM/HcalCommon/interface/Container1D.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class Container2D : public Container1D
	{
		public:
			Container2D() {}
			Container2D(std::string const& folder, std::string nametitle,
				mapper::MapperType mt, axis::Axis *xaxis, 
				axis::Axis *yaxis = new axis::CoordinateAxis(axis::fYaxis,
					axis::fiphi),
				axis::Axis *zaxis = new axis::ValueAxis(axis::fZaxis, 
					axis::fEntries));
			virtual ~Container2D() {}

			//	redeclare what to override
			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, int);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, int, double);
			virtual void fill(HcalDetId const&, double, double);

			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, int);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, int, double);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual void fill(HcalTrigTowerDetId const&);
			virtual void fill(HcalTrigTowerDetId const&, int);
			virtual void fill(HcalTrigTowerDetId const&, double);
			virtual void fill(HcalTrigTowerDetId const&, int, int);
			virtual void fill(HcalTrigTowerDetId const&, int, double);
			virtual void fill(HcalTrigTowerDetId const&, double, double);

			virtual void setBinContent(int, int, int, double);
			virtual void setBinContent(int, int, double, double);
			virtual void setBinContent(int, double, int, double);
			virtual void setBinContent(int, double, double, double);

			//	booking. see Container1D.h
			virtual void book(DQMStore::IBooker&,
				std::string subsystem="Hcal", std::string aux="");

			virtual void loadLabels(std::vector<std::string> const&);

		protected:
			Axis					*_zaxis;
	};
}


#endif








