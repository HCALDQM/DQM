
#include "DQM/HcalCommon/interface/DetectorQuantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		int getValue_iphi(HcalDetId const& did)
		{
			return did.iphi();
		}

		int getValue_ieta(HcalDetId const& did)
		{
			x = did.ieta();
			if (x<0)
				x = did.subdet()==HcalForward ? x+41 : x+42;
			else
				x = did.subdet()==HcalForward ? x+42 : x+41;
			return x;
		}

		int getValue_depth(HcalDetId const& did)
		{
			return did.depth();
		}

		int getValue_Subdet(HcalDetId const& did)
		{
			return did.subdet()-1;
		}

		int getValue_SubdetPM(HcalDetId const& did)
		{
			return did.ieta()<0 ? 2*(did.subdet()-1) : 
				2*(did.subdet()-1)+1;
		}

		int getBin_iphi(HcalDetId const& did)
		{
			return did.iphi();
		}

		int getBin_ieta(HcalDetId const& did)
		{
			return getValue_ieta(did)+1;
		}

		int getBin_depth(HcalDetId const& did)
		{
			return did.depth();
		}

		int getBin_Subdet(HcalDetId const& did)
		{
			return did.subdet();
		}

		int getBin_SubdetPM(HcalDetId const& did)
		{
			return getVaalue_SubdetPM(did)+1;
		}
	}
}
