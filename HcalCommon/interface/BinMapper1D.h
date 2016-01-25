#ifndef BinMapper1D_h
#define BinMapper1D_h

/**
 *	file:		BinMapper1D.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/Mapper.h"
#include "DQM/HcalCommon/interface/DetectorQuantity.h"

namespace hcaldqm
{
	namespace mapper
	{
		class BinMapper1D : public Mapper
		{
			public:
				BinMapper1D();
				BinMapper1D(Quantity*, 
					Quantity *qy = DetectorQuantity(quantity::fiphi));
				BinMapper1D(BinMapper1D const&);
				virtual ~BinMapper1D();

				BinMapper1D &operator=(BinMapper1D const&);

				//	get Bins
				virtual uint32_t getBinX(HcalDetId const&);
				virtual uint32_t getBinX(HcalElectronicsId const&);
				virtual uint32_t getBinX(HcalTrigTowerDetId const&);
				virtual uint32_t getBinX(int);
				virtual uint32_t getBinX(double);
				virtual uint32_t getBinY(HcalDetId const&);
				virtual uint32_t getBinY(HcalTrigTowerDetId const&);
				virtual uint32_t getBinY(HcalElectronicsId const&);
				virtual uint32_t getBinY(int);
				virtual uint32_t getBinY(double);

				//	get Values
				virtual int getValueX(HcalDetId const&);
				virtual int getValueX(HcalElectronicsId const&);
				virtual int getValueX(HcalTrigTowerDetId const&);
				virtual int getValueX(int);
				virtual double getValueX(double);
				virtual int getValueY(HcalDetId const&);
				virtual int getValueY(HcalElectronicsId const&);
				virtual int getValueY(HcalTrigTowerDetd const&);
				virtual int getValueY(int);
				virtual double getValueY(double);

			protected:
				Quantity		*_qx;
				Quantity		*_qy;
		};
	}
}

#endif
