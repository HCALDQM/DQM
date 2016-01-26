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
			Container2D();
			Container2D(std::string const& folder,
				hashfunctions::HashType, Quantity*, Quantity*,
				Quantity *qz = new ValueQuantity(quantity::fN));
			virtual ~Container2D() {}

			//	Initialize Container
			//	@folder
			//	@nametitle, 
			virtual void initialize(std::string const& folder, 
					hashfunctions::HashType, Quantity*, Quantity*,
					Quantity *qz = new ValueQuantity(quantity::fN),
					int debug=0);
			
			//	@qname - quantity name replacer
			virtual void initialize(std::string const& folder, 
				std::string const& qname,
				hashfunctions::HashType, Quantity*, Quantity*,
				Quantity *qz = new ValueQuantity(quantity::fN),
				int debug=0);

			//	redeclare what to override
			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, int);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, int, double);
			virtual void fill(HcalDetId const&, int, int);
			virtual void fill(HcalDetId const&, double, double);

			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, int);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, int, double);
			virtual void fill(HcalElectronicsId const&, int, int);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual void fill(HcalTrigTowerDetId const&);
			virtual void fill(HcalTrigTowerDetId const&, int);
			virtual void fill(HcalTrigTowerDetId const&, double);
			virtual void fill(HcalTrigTowerDetId const&, int, int);
			virtual void fill(HcalTrigTowerDetId const&, int, double);
			virtual void fill(HcalTrigTowerDetId const&, double, double);

			//	booking. see Container1D.h
			virtual void book(DQMStore::IBooker&,
				HcalElectronicsMap const*,
				std::string subsystem="Hcal", std::string aux="");

		protected:
			Quantity	*_qz;

			virtual void customize(MonitorElement*);
	};
}


#endif








