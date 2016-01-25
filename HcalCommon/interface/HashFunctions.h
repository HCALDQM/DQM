#ifndef HashFunctions_h
#define HashFunctions_h

/**
 *	file:			HashFunctions.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Hash accoring to various Detector Elements
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

namespace hcaldqm
{
	using namespace constants;
	namespace hashfunctions
	{
		/**
		 *	by HcalDetId
		 */
		uint32_t hash_Subdet(HcalDetId const&);
		uint32_t hash_Subdetiphi(HcalDetId const&);
		uint32_t hash_Subdetieta(HcalDetId const&);
		uint32_t hash_Subdetdepth(HcalDetId const&);
		uint32_t hash_SubdetPM(HcalDetId const&);
		uint32_t hash_SubdetPMiphi(HcalDetId const&);
		uint32_t hash_iphi(HcalDetId const&);
		uint32_t hash_ieta(HcalDetId const&);
		uint32_t hash_depth(HcalDetId const&);
		uint32_t hash_HFPMiphi(HcalDetId const&);
		uint32_t hash_HBHEPartition(HcalDetId const&);

		std::string name_Subdet(HcalDetId const&);
		std::string name_Subdetiphi(HcalDetId const&);
		std::string name_Subdetieta(HcalDetId const&);
		std::string name_Subdetdepth(HcalDetId const&);
		std::string name_SubdetPM(HcalDetId const&);
		std::string name_SubdetPMiphi(HcalDetId const&);
		std::string name_iphi(HcalDetId const&);
		std::string name_ieta(HcalDetId const&);
		std::string name_depth(HcalDetId const&);
		std::string name_HFPMiphi(HcalDetId const&);
		std::string name_HBHEPartition(HcalDetId const&);

		enum HashType
		{
			fSubdet = 0,
			fSubdetiphi = 1,
			fSubdetieta = 2,
			fSubdetdepth = 3,
			fSubdetPM = 4,
			fSubdetPMiphi = 5,
			fiphi = 6,
			fieta = 7,
			fdepth = 8,
			fHFPMiphi = 9,
			fHBHEPartition = 10,
			nHashType_did = 11,
			nHashType_eid = 12,
			nHashType_tid = 13,
			nHashType = 14
		};
		typedef uint32_t (*hash_function_did)(HcalDetId const&);
		typedef std::string (*name_function_did)(HcalDetId const&);
		hash_function_did const hash_did[nHashType_did] = {
			hash_Subdet, hash_Subdetiphi, hash_Subdetieta, 
			hash_Subdetdepth, hash_SubdetPM, hash_SubdetPMiphi,
			hash_iphi, hash_ieta, hash_depth, hash_HFPMiphi, 
			hash_HBHEPartition
		};
		name_function_did const name_did[nHashType_did] = {
			name_Subdet, name_Subdetiphi, name_Subdetieta, 
			name_Subdetdepth, name_SubdetPM, name_SubdetPMiphi,
			name_iphi, name_ieta, name_depth, name_HFPMiphi, 
			name_HBHEPartition
		};
		int const nhashes = nHashType_did + (nHashType_eid-nHashType_did-1) + 
			(nHashType_tid-nHashType_eid-1);
		std::string const hash_names[nhashes] = {
			"Subdet", "Subdetiphi", "Subdetieta", "Subdetdepth",
			"SubdetPM", "SubdetPMiphi", "iphi", "ieta", "depth",
			"HFPMiphi", "HBHEPartition"
		};

		/**
		 *	by HcalElectronics Id
		 */
/*		uint32_t hash_FED(HcalElectronicsId const&);
		uint32_t hash_FEDSpigot(HcalElectronicsId const&);
		uint32_t hash_FEDSlot(HcalElectronicsId const&);
		uint32_t hash_Crate(HcalElectronicsId const&);
		uint32_t hash_CrateSpigot(HcalElectronicsId const&);
		uint32_t hash_CrateSlot(HcalElectronicsId const&);
		uint32_t hash_Fiber(HcalElectronicsId const&);
		uint32_t hash_FiberFiberCh(HcalElectronicsId const&);
		uint32_t hash_FiberCh(HcalElectronicsId const&);
*/
		/**
		 *	by HcalTrigTowerDetId
		 */
/*		uint32_t hash_TTSubdet(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetPM(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetPMiphi(HcalTrigTowerDetId const&);
		uint32_t hash_TTSubdetieta(HcalTrigTowerDetId const&);
		*/
	}
}

#endif
