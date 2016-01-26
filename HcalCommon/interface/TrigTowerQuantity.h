#ifndef TrigTowerQuantity_h
#define TrigTowerQuantity_h

/**
 *	file:		TrigTowerQuantity.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/Quantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		enum TrigTowerQuantityType
		{
			fTTiphi = 0,
			fTTieta = 1,
			fTTdepth = 2,
			fTTSubdet = 3,
			fTTSubdetPM = 4,
			nTrigTowerQuantityType = 5
		};

		int getValue_TTiphi(HcalTrigTowerDetId const&);
		int getValue_TTieta(HcalTrigTowerDetId const&);
		int getValue_TTdepth(HcalTrigTowerDetId const&);
		int getValue_TTSubdet(HcalTrigTowerDetId const&);
		int getValue_TTSubdetPM(HcalTrigTowerDetId const&);
		uint32_t getBin_TTiphi(HcalTrigTowerDetId const&);
		uint32_t getBin_TTieta(HcalTrigTowerDetId const&);
		uint32_t getBin_TTdepth(HcalTrigTowerDetId const&);
		uint32_t getBin_TTSubdet(HcalTrigTowerDetId const&);
		uint32_t getBin_TTSubdetPM(HcalTrigTowerDetId const&);

		typedef int (*getValueType_tid)(HcalTrigTowerDetId const&);
		typedef uint32_t (*getBinType_tid)(HcalTrigTowerDetId const&);
		getValueType_tid const getValue_functions_tid[nTrigTowerQuantityType]= 
		{
			getValue_TTiphi, getValue_TTieta, getValue_TTdepth,
			getValue_TTSubdet, getValue_TTSubdetPM
		};
		getBinType_tid const getBin_functions_tid[nTrigTowerQuantityType] = {
			getBin_TTiphi, getBin_TTieta, getBin_TTdepth,
			getBin_TTSubdet, getBin_TTSubdetPM
		};
		std::string const name_tid[nTrigTowerQuantityType] = {
			"TTiphi", "TTieta", "TTdepth", "TTSubdet", "TTSubdetPM"
		};
		double const min_tid[nTrigTowerQuantityType] = {
			0.5, 0, -0.5, 0, 0
		};
		double const max_tid[nTrigTowerQuantityType] = {
			72.5, 64, 0.5, 2, 4
		};
		int const nbins_tid[nTrigTowerQuantityType] = {
			72, 64, 1, 2, 4
		};

		class TrigTowerQuantity : public Quantity
		{
			public:
				TrigTowerQuantity() {}
				TrigTowerQuantity(TrigTowerQuantityType type, bool isLog=false):
					Quantity(name_tid[type], isLog), _type(type)
				{}
				virtual ~TrigTowerQuantity() {}

				virtual int getValue(HcalTrigTowerDetId const& tid)
				{return getValue_functions_tid[_type](tid);}
				virtual uint32_t getBin(HcalTrigTowerDetId const& tid)
				{return getBin_functions_tid[_type](tid);}

				virtual QuantityType type() {return fTrigTowerQuantity;}
				virtual int nbins() {return nbins_tid[_type];}
				virtual double min() {return min_tid[_type];}
				virtual double max() {return max_tid[_type];}
				virtual bool isCoordinate() {return true;}

			protected:
				TrigTowerQuantityType _type;
		};
	}
}

#endif
