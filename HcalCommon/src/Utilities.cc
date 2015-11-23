#include "DQM/HcalCommon/interface/Utilities.h"

namespace hcaldqm
{
	using namespace constants;
	namespace utilities
	{

		/*
		 *	Useful Detector Functions. For Fast Detector Validity Check
		 */
		bool validDetId(HcalSubdetector sub, int ieta, int iphi, int depth)
		{
			int ie(abs(ieta));
			return ((iphi>=1) && 
					(iphi<=72) &&
					(depth>=1) &&
					(ie>=1) &&
					(((sub==HcalBarrel) &&
					  (((ie<=14) &&
						(depth==1)) ||
					   (((ie==15) || (ie==16)) &&
						(depth<=2)))) ||
					 ((sub==HcalEndcap) &&
					  (((ie==16) &&
						(depth==3)) ||
					   ((ie==17) &&
						(depth==1)) ||
					   ((ie>=18) &&
						(ie<=20) &&
						(depth<=2)) ||
					   ((ie>=21) &&
						(ie<=26) &&
						(depth<=2) &&
						(iphi%2==1)) ||
					   ((ie==29) &&
						(depth<=2) &&
						(iphi%2==1)))) ||
					 ((sub==HcalOuter) &&
					  (ie<=15) &&
					  (depth==4)) ||
					 ((sub==HcalForward) &&
					  (depth<=2) &&
					  (((ie>=29) &&
						(ie<=39) &&
						(iphi%2==1)) ||
					   ((ie>=40) &&
						(ie<=41) &&
						(iphi%4==3))))));
		}

		bool validDetId(HcalDetId const& did)
		{
			return validDetId(did.subdet(), 
				did.ieta(), did.iphi(), did.depth());
		}
	}
}

