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

#include <string>

namespace hcaldqm
{
	class ContainerSingle2D : public Container
	{
		public:
			ContainerSingle2D();
			ContainerSingle2D(std::string const& folder, 
				Quantity*, Quantity*,
				Quantity *qz = new ValueQuantity(quantity::fN));
			virtual ~ContainerSingle2D() {}

			virtual void initialize(std::string const& folder, 
				Quantity*, Quantity*,
				Quantity *qz = new ValueQuantity(quantity::fN),
				int debug=0);

			virtual void initialize(std::string const& folder, 
				std::string const&,
				Quantity*, Quantity*,
				Quantity *qz = new ValueQuantity(quantity::fN),
				int debug=0);

			//	booking
			virtual void book(DQMStore::IBooker&,
				std::string subsystem="Hcal", std::string aux="");

			//	filling
			virtual void fill(int, int);
			virtual void fill(int, double);
			virtual void fill(int, double, double);
			virtual void fill(int, int, int);
			virtual void fill(int, int, double);
			virtual void fill(double, int);
			virtual void fill(double, double);
			virtual void fill(double, double, double);

			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, int);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, int, int);
			virtual void fill(HcalDetId const&, int, double);
			virtual void fill(HcalDetId const&, double, double);

			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, int);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, int, int);
			virtual void fill(HcalElectronicsId const&, int, double);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual void fill(HcalDetId const&, HcalElectronicsId const&);
			virtual void fill(HcalDetId const&, HcalElectronicsId const&, 
				double);

			virtual void fill(HcalTrigTowerDetId const&);
			virtual void fill(HcalTrigTowerDetId const&, int);
			virtual void fill(HcalTrigTowerDetId const&, double);
			virtual void fill(HcalTrigTowerDetId const&, int, int);
			virtual void fill(HcalTrigTowerDetId const&, int, double);
			virtual void fill(HcalTrigTowerDetId const&, double, double);

			virtual void reset() {_me->Reset();}
			virtual void print() {std::cout << _qname << std::endl;}

		protected:
			MonitorElement					*_me;
			Quantity						*_qx;
			Quantity						*_qy;
			Quantity						*_qz;
	};
}

#endif
