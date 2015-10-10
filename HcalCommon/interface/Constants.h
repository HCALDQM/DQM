#ifndef Constants_h
#define Constants_h

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

namespace hcaldqm
{
	namespace constants
	{
		/*
		 *	Electronics Constants
		 */

		//	FEDs
		int const FED_VME_MIN = 700;
		int const FED_VME_MAX = 731;
		int const FED_VME_DELTA = 1;
		int const FED_VME_NUM = FED_VME_MAX-FED_VME_MIN+1;

		int const FED_uTCA_MIN = 1100;
		int const FED_uTCA_MAX = 1122;
		int const FED_uTCA_NUM = 12;
		int const FED_uTCA_DELTA = 2;
		
		//	Crates
		int const CRATE_VME_MIN = 0;
		int const CRATE_VME_MAX = 18;
		int const CRATE_VME_DELTA = 1;
		int const CRATE_VME_NUM = CRATE_VME_MAX-CRATE_VME_MIN+1;

		int const CRATE_uTCA_MIN = 20;
		int const CRATE_uTCA_MAX = 37;
		int const CRATE_uTCA_DELTA = 1;
		int const CRATE_uTCA_NUM = CRATE_uTCA_MAX-CRATE_uTCA_MIN+1;

		//	Slots
		int const SLOT_uTCA_MIN = 1;
		int const SLOT_uTCA_MAX = 12;
		int const SLOT_uTCA_DELTA = 1;
		int const SLOT_uTCA_NUM = SLOT_uTCA_MAX-SLOT_uTCA_MIN+1;

		int const SLOT_VME_MIN = 2;
		int const SLOT_VME_MIN1 = 7;
		int const SLOT_VME_MIN2 = 13;
		int const SLOT_VME_MAX = 18;
		int const SLOT_VME_NUM = SLOT_VME_MIN1-SLOT_VME_MIN + 1
			+ SLOT_VME_MAX-SLOT_VME_MIN1 + 1;

		/*
		 *	Detector Constants
		 */
		
		//	Hcal Subdetector
		int const HB = 1;
		int const HE = 2;
		int const HO = 3;
		int const HF = 4;

		//	iphis
		int const IPHI_MIN = 1;
		int const IPHI_MAX = 72;
		int const IPHI_NUM = 72;
		int const IPHI_DELTA = 1;
		int const IPHI_DELTA_HF = 2;
		
		//	ietas
		int const IETA_MIN=1;
		int const IETA_DELTA = 1;
		int const IETA_MAX=41;
		int const IETA_NUM=2*(IETA_MAX-IETA_MIN+1);
		int const IETA_MIN_HB = 1;
		int const IETA_MAX_HB = 16;
		int const IETA_MIN_HE = 16;
		int const IETA_MAX_HE = 29;
		int const IETA_MIN_HO = 1;
		int const IETA_MAX_HO = 15;
		int const IETA_MIN_HF = 29;
		int cosnt IETA_MAX_HF = 41;

		//	Depth
		int const DEPTH_MIN = 1;
		int const DEPTH_DELTA = 1;
		int const DEPTH_MAX = 4;
		int const DEPTH_NUM = 4;
	}
}

#endif
