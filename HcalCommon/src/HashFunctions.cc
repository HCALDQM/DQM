#include "DQM/HcalCommon/interface/HashFunctions.h"
#include "DQM/HcalCommon/interface/Utilities.h"

namespace hcaldqm
{
	using namespace constants;
	namespace hashfunctions
	{
		/**
		 *	HcalDetId
		 */
		uint32_t hash_Subdet(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(), 1, 1, 1));
		}

		uint32_t hash_Subdetiphi(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(), 1, did.iphi(), 1));
		}

		uint32_t hash_Subdetieta(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(), did.ieta(),
				1, 1));
		}

		uint32_t hash_Subdetdepth(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(), 1,
				1, did.depth()));
		}

		uint32_t hash_SubdetPM(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(),
				did.ieta()>0 ? 1 : -1, 1, 1));
		}

		uint32_t hash_SubdetPMiphi(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(did.subdet(),
				did.ieta()>0 ? 1 : -1, did.iphi(), 1));
		}

		uint32_t hash_iphi(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(HcalBarrel,
				1, did.iphi(), 1));
		}

		uint32_t hash_ieta(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(HcalBarrel,
				did.ieta(), 1, 1));
		}

		uint32_t hash_depth(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(HcalBarrel,
				1, 1, did.depth()));
		}

		uint32_t hash_HFPMiphi(HcalDetId const& did)
		{
			return utilities::hash(HcalDetId(HcalForward,
				did.ieta()>0 ? 1 : -1, did.iphi(), 1));
		}

		uint32_t hash_HBHEPartition(HcalDetId const &did)
		{
			int iphi = did.iphi();
			uint32_t hash = 0;
			if (iphi>=3 && iphi<=26)
				hash = utilities::hash(HcalDetId(HcalBarrel,
					1, 3, 1));
			else if (iphi>=27 && iphi<=50)
				hash = utilities::hash(HcalDetId(HcalBarrel,
					1, 27, 1));
			else
				hash = utilities::hash(HcalDetId(HcalBarrel,
					1, 1, 1));

			return hash;
		}

		std::string name_Subdet(HcalDetId const& did)
		{
			return constants::SUBDET_NAME[did.subdet()-1];
		}

		std::string name_SubdetPM(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "%s%s",constants::SUBDET_NAME[did.subdet()-1].c_str(),
				did.ieta()>0 ? "P" : "M");
			return std::string(name);
		}

		std::string name_Subdetiphi(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "%siphi%d", 
				constants::SUBDET_NAME[did.subdet()-1].c_str(),
				did.iphi());
			return std::string(name);
		}

		std::string name_Subdetieta(HcalDetId const& did)
		{
			char name[20];
			sprintf(name, "%sieta%d", 
				constants::SUBDET_NAME[did.subdet()-1].c_str(),
				did.ieta());
			return std::string(name);
		}

		std::string name_Subdetdepth(HcalDetId const& did)
		{
			char name[20];
			sprintf(name, "%sdepth%d", 
				constants::SUBDET_NAME[did.subdet()-1].c_str(),
				did.depth());
			return std::string(name);
		}

		std::string name_SubdetPMiphi(HcalDetId const& did)
		{
			char name[20];
			sprintf(name, "%s%siphi%d", 
				constants::SUBDET_NAME[did.subdet()-1].c_str(), 
				did.ieta()>0 ? "P" : "M", did.iphi());
			return std::string(name);
		}

		std::string name_iphi(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "iphi%d", did.iphi());
			return std::string(name);
		}

		std::string name_ieta(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "ieta%d", did.ieta());
			return std::string(name);
		}

		std::string name_depth(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "depth%d", did.depth());
			return std::string(name);

		}

		std::string name_HFPMiphi(HcalDetId const& did)
		{
			char name[10];
			sprintf(name, "HF%siphi%d", did.ieta()>0 ? "P" : "M", did.iphi());
			return std::string(name);
		}

		std::string name_HBHEPartition(HcalDetId const& did)
		{
			char c;
			if (did.iphi()>=3 && did.iphi()<=26)
				c = 'a';
			else if (did.iphi()>=27 && did.iphi()<=50)
				c = 'b';
			else
				c = 'c';
			char name[10];
			sprintf(name, "HBHE%c", c);
			return std::string(name);
		}
	}
}
