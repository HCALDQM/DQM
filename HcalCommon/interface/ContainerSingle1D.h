#ifndef ContainerSingle1D_h
#define ContainerSingle1D_h

/*
 *	file:			ContainerSignle1D.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Container to hold a single ME - for convenience of initialization
 */

#include "DQM/HcalCommon/interface/Container.h"
#include "DQM/HcalCommon/interface/ValueQuantity.h"

#include <string>

namespace hcaldqm
{
	using namespace quantity;
	class ContainerSingle1D : public Container
	{
		public:
			ContainerSingle1D();
			ContainerSingle1D(std::string const& folder, 
				Quantity*,
				Quantity *qy = new ValueQuantity(quantity::fN));
			virtual ~ContainerSingle1D() {}
			
			virtual void initialize(std::string const& folder, 
				Quantity*,
				Quantity *qy = new ValueQuantity(quantity::fN),
				int debug=0);

			virtual void initialize(std::string const& folder, 
				std::string const&, 
				Quantity*,
				Quantity *qy = new ValueQuantity(quantity::fN),
				int debug=0);

			//	booking
			virtual void book(DQMStore::IBooker&,
				std::string subsystem="Hcal", std::string aux="");

			//	filling
			virtual void fill(int);
			virtual void fill(double);
			virtual void fill(int, int);
			virtual void fill(int, double);
			virtual void fill(double, int);
			virtual void fill(double, double);

			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, double, double);

			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual void fill(HcalTrigTowerDetId const&);
			virtual void fill(HcalTrigTowerDetId const&, double);
			virtual void fill(HcalTrigTowerDetId const&, double, double);

			virtual void reset() {_me->Reset();}
			virtual void print() {std::cout << _qname << std::endl;}

		protected:
			MonitorElement				*_me;
			Quantity					*_qx;
			Quantity					*_qy;

			virtual void customize();
	};
}

#endif
