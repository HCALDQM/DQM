#include "DQM/HcalCommon/interface/TrigTowerQuantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		int getValue_TTiphi(HcalTrigTowerDetId const& tid)
		{
			return tid.iphi();
		}

		int getValue_TTieta(HcalTrigTowerDetId const& tid)
		{
			return tid.ieta()<0 ? tid.ieta()+32 : tid.ieta()+31;
		}

		int getValue_TTdepth(HcalTrigTowerDetId const& tid)
		{
			return tid.depth();
		}

		int getValue_TTSubdet(HcalTrigTowerDetId const& tid)
		{
			return tid.ietaAbs()<29 ? 0 : 1;
		}

		int getValue_TTSubdetPM(HcalTrigTowerDetId const& tid)
		{
			int x = tid.ietaAbs()<29 ? 0 : 2;
			return tid.ieta()>0 ? x+1 : x;
		}

		uint32_t getBin_TTiphi(HcalTrigTowerDetId const& tid)
		{
			return (uint32_t)(getValue_TTiphi(tid));
		}

		uint32_t getBin_TTieta(HcalTrigTowerDetId const& tid)
		{
			return (uint32_t)(getValue_TTieta(tid)+1);
		}

		uint32_t getBin_TTdepth(HcalTrigTowerDetId const& tid)
		{
			return (uint32_t)(getValue_TTdepth(tid)+1);
		}

		uint32_t getBin_TTSubdet(HcalTrigTowerDetId const& tid)
		{
			return (uint32_t)(getValue_TTSubdet(tid)+1);
		}

		uint32_t getBin_TTSubdetPM(HcalTrigTowerDetId const& tid)
		{
			return (uint32_t)(getValue_TTSubdetPM(tid)+1);
		}
	}
}
