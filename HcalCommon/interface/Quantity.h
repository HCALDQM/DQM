#ifndef Quantity_h
#define Quantity_h

/**
 *	file:		Quantity.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

namespace hcaldqm
{
	namespace quantity
	{
		enum QuantityType
		{
			fDetectorQuantity = 0,
			fElectronicsQuantity = 1,
			fTrigTowerQuantity = 2,
			fValueQuantity = 3,
			fFlagQuantity = 4,
			fNone = 5,
			nQuantityType = 6
		};

		class Quantity
		{
			public:
				Quantity() : _name("Quantity"), _isLog(false)
				{}
				Quantity(std::string const& name, bool isLog) : 
					_name(name), _isLog(isLog)
				{}
				virtual ~Quantity() {}

				QuantityType type() {return fNone;}
				std::string name() {return _name;}
				bool		isLog() {return _isLog;}

				virtual uint32_t getBin(HcalDetId const&) {return 1;}
				virtual uint32_t getBin(HcalElectronicsId const&) {return 1;}
				virtual uint32_t getBin(HcalTrigTowerDetId const&) {return 1;}
				virtual uint32_t getBin(int) {return 1;}
				virtual uint32_t getBin(double) {return 1;}

				virtual int getValue(HcalDetId const&) {return 0;}
				virtual int getValue(HcalElectronicsId const&) {return 0;}
				virtual int getValue(HcalTrigTowerDetId const&) {return 0;}
				virtual int getValue(int x) {return x;}
				virtual double getValue(double x) {return x;}

				virtual int nbins() {return 1;}
				virtual double min() {return 0;}
				virtual double max() {return 1;}
				virtual bool isCoordinate() {return false;}

			protected:
				std::string		_name;
				bool			_isLog;
		};
	}
}

#endif
