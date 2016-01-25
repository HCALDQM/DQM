#ifndef DetectorQuantity_h
#define DetectorQuantity_h

/**
 *	file:		DetectorQuantity.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/Quantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		enum DetectorQuantityType
		{
			fiphi = 0,
			fieta = 1,
			fdepth = 2,
			fSubdet = 3,
			fSubdetPM = 4,
			nDetectorQuantityType = 5
		};
		int getValue_iphi(HcalDetId const&);
		int getValue_ieta(HcalDetId const&);
		int getValue_depth(HcalDetId const&);
		int getValue_Subdet(HcalDetId const&);
		int getValue_SubdetPM(HcalDetId const&);
		uint32_t getBin_iphi(HcalDetId const&);
		uint32_t getBin_ieta(HcalDetId const&);
		uint32_t getBin_depth(HcalDetId const&);
		uint32_t getBin_Subdet(HcalDetId const&);
		uint32_t getBin_SubdetPM(HcalDetId const&);

		typedef int (*getValueType_did)(HcalDetId const&);
		typedef uint32_t (*getBinType_did)(HcalDetId const&);
		getValueType_did const getValue_functions_did[nDetectorQuantityType] = {
			getValue_iphi, getValue_ieta, getValue_depth,
			getValue_Subdet, getValue_SubdetPM
		};
		getBinType_did const getBin_functions_did[nDetectorQuantityType] = {
			getBin_iphi, getBin_ieta, getBin_depth,
			getBin_Subdet, getBin_SubdetPM
		};
		std::string const name_did[nDetectorQuantityType] = {
			"iphi", "ieta", "depth", "Subdet", "SubdetPM"
		};
		double const min_did[nDetectorQuantityType] = {
			0.5, 0, 0.5, 0, 0
		};
		double const max_did[nDetectorQuantityType] = {
			72.5, 84, 4.5, 4, 8
		};
		int const nbins_did[nDetectorQuantityType] = {
			72, 84, 4, 4, 8
		};

		class DetectorQuantity : public Quantity
		{
			public:
				DetectorQuantity() {}
				DetectorQuantity(DetectorQuantityType type, bool isLog=false) :
					Quantity(name_did[type], isLog), _type(type)
				{}
				virtual ~DetectorQuantity();

				virtual int getValue(HcalDetId const& did) 
				{return getValue_functions_did[_type](did);}
				virtual int getBin(HcalDetId const& did)
				{return getBin_functions_did[_type](did);}
				virtual QuantityType type() 
				{return fDetectorQuantity;}

				virtual int nbins() {return nbins_did[_type];}
				virtual double min() {return min_did[_type];}
				virtual double max() {return max_did[_type];}

			protected:
				DetectorQuantityType	_type;
		};
	}
}

#endif
