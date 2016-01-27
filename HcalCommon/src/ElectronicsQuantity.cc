#include "DQM/HcalCommon/interface/ElectronicsQuantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		int getValue_FED(HcalElectronicsId const& eid)
		{
			int v = 0;
			if (eid.isVMEid())
				v = eid.dccid();
			else
				v = utilities::crate2fed(eid.crateId())-FED_uTCA_MIN + 
					constants::FED_VME_NUM;
			return v;
		}

		int getValue_FEDuTCA(HcalElectronicsId const& eid)
		{
			return utilities::crate2fed(eid.crateId())-FED_uTCA_MIN;
		}

		int getValue_FEDVME(HcalElectronicsId const& eid)
		{
			return eid.dccid();
		}

		int getValue_Crate(HcalElectronicsId const& eid)
		{
			int c = eid.crateId();
			if (eid.isUTCAid())
				c = CRATE_VME_NUM + eid.crateId()-CRATE_uTCA_MIN;
			return c;
		}

		int getValue_CrateuTCA(HcalElectronicsId const& eid)
		{
			return eid.crateId()-CRATE_uTCA_MIN;
		}

		int getValue_CrateVME(HcalElectronicsId const& eid)
		{
			return eid.crateId();
		}

		int getValue_SlotuTCA(HcalElectronicsId const& eid)
		{
			return eid.slot()-SLOT_uTCA_MIN;
		}

		int getValue_SlotVME(HcalElectronicsId const& eid)
		{
			int slot = eid.slot();
			if (slot<=SLOT_VME_MAX1)
				slot-=SLOT_VME_MIN1;
			else
				slot = SLOT_VME_NUM1 + slot-SLOT_VME_MIN2;
			return slot;
		}

		int getValue_Spigot(HcalElectronicsId const& eid)
		{
			return eid.spigot();
		}

		int getValue_FiberuTCA(HcalElectronicsId const& eid)
		{
			int fiber = eid.fiberIndex();
			if (fiber<=FIBER_uTCA_MAX1)
				fiber-=FIBER_uTCA_MIN1;
			else
				fiber = (FIBER_uTCA_MAX1-FIBER_uTCA_MIN1+1) + 
					fiber-FIBER_uTCA_MIN2;
			return fiber;
		}

		int getValue_FiberVME(HcalElectronicsId const& eid)
		{
			return eid.fiberIndex()-1;
		}

		int getValue_FiberCh(HcalElectronicsId const& eid)
		{
			return eid.fiberChanId();
		}

		uint32_t getBin_FED(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FED(eid)+1);
		}

		uint32_t getBin_FEDuTCA(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FEDuTCA(eid)+1);
		}

		uint32_t getBin_FEDVME(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FEDVME(eid)+1);
		}

		uint32_t getBin_Crate(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_Crate(eid)+1);
		}

		uint32_t getBin_CrateuTCA(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_CrateuTCA(eid)+1);
		}

		uint32_t getBin_CrateVME(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_CrateVME(eid)+1);
		}

		uint32_t getBin_SlotuTCA(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_SlotuTCA(eid)+1);
		}

		uint32_t getBin_SlotVME(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_SlotVME(eid)+1);
		}

		uint32_t getBin_Spigot(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_Spigot(eid)+1);
		}

		uint32_t getBin_FiberuTCA(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FiberuTCA(eid)+1);
		}

		uint32_t getBin_FiberVME(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FiberVME(eid)+1);
		}

		uint32_t getBin_FiberCh(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FiberCh(eid)+1);
		}

		HcalElectronicsId getEid_FED(int v)
		{
			return v<FED_VME_NUM ? HcalElectronicsId(FIBERCH_MIN,
				FIBER_VME_MIN, SPIGOT_MIN, v) :
				HcalElectronicsId(utilities::fed2crate(v-FED_VME_NUM+1100),
				SLOT_uTCA_MIN, FIBER_uTCA_MIN1,
				FIBERCH_MIN, false);
		}

		HcalElectronicsId getEid_FEDVME(int v)
		{
			return HcalElectronicsId(FIBERCH_MIN,
				FIBER_VME_MIN, SPIGOT_MIN, v);
		}

		HcalElectronicsId getEid_FEDuTCA(int v)
		{
			return HcalElectronicsId(utilities::fed2crate(v+1100),
				SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, false);
		}

		HcalElectronicsId getEid_Crate(int v)
		{
			return v<CRATE_VME_NUM ?
				HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN, SPIGOT_MIN, v) :
				HcalElectronicsId(v-CRATE_VME_NUM+CRATE_uTCA_MIN,
					SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, false);
		}

		HcalElectronicsId getEid_CrateVME(int v)
		{
			return HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN,
				SPIGOT_MIN, v);
		}

		HcalElectronicsId getEid_CrateuTCA(int v)
		{
			return HcalElectronicsId(v+CRATE_uTCA_MIN,
				SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, false);
		}

		HcalElectronicsId getEid_SlotuTCA(int v)
		{
			return HcalElectronicsId(CRATE_uTCA_MIN,
				v+SLOT_uTCA_MIN, FIBER_uTCA_MIN1, FIBERCH_MIN, false);
		}

		//	nothing for now...
		HcalElectronicsId getEid_SlotVME(int v)
		{
			return 
				HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN, 
					SPIGOT_MIN, CRATE_VME_MIN);
		}

		HcalElectronicsId getEid_Spigot(int v)
		{
			return HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN,
				v, CRATE_VME_MIN);
		}

		HcalElectronicsId getEid_FiberuTCA(int v)
		{
			return HcalElectronicsId(CRATE_uTCA_MIN,
				SLOT_uTCA_MIN, v<(FIBER_uTCA_MAX1-FIBER_uTCA_MIN1+1)?
				v+FIBER_uTCA_MIN1 : v-(FIBER_uTCA_MAX1-FIBER_uTCA_MIN1+1)+
				FIBER_uTCA_MIN2,
				FIBERCH_MIN, false);
		}
		
		HcalElectronicsId getEid_FiberVME(int v)
		{
			return HcalElectronicsId(FIBERCH_MIN, v+1,
				SPIGOT_MIN, CRATE_VME_MIN);
		}

		HcalElectronicsId getEid_FiberCh(int v)
		{
			return HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
				FIBER_uTCA_MIN1, v, false);
		}

		std::vector<std::string> getLabels_FED()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FED_TOTAL_NUM; i++)
			{
				HcalElectronicsId eid = getEid_FED(i);
				sprintf(name, "%d", 
					eid.isVMEid()?eid.dccid()+700:utilities::crate2fed(
					eid.crateId()));
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_FEDuTCA()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FED_uTCA_NUM; i++)
			{
				HcalElectronicsId eid = getEid_FEDuTCA(i);
				sprintf(name, "%d",
					utilities::crate2fed(eid.crateId()));
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_FEDVME()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FED_VME_MIN; i++)
			{
				sprintf(name, "%d",
					getEid_FEDVME(i).dccid()+700);
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_Crate()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<CRATE_TOTAL_NUM; i++)
			{
				HcalElectronicsId eid = getEid_Crate(i);
				sprintf(name, "%d%c",
					eid.crateId(), eid.isVMEid()?'v':'u');
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_CrateVME()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<CRATE_VME_NUM; i++)
			{
				HcalElectronicsId eid = getEid_CrateVME(i);
				sprintf(name, "%dv",
					eid.crateId());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_CrateuTCA()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<CRATE_uTCA_NUM; i++)
			{
				HcalElectronicsId eid = getEid_CrateuTCA(i);
				sprintf(name, "%du",
					eid.crateId());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_SlotuTCA()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<SLOT_uTCA_NUM; i++)
			{
				HcalElectronicsId eid = getEid_SlotuTCA(i);
				sprintf(name, "%d",
					eid.slot());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_SlotVME()
		{
			return std::vector<std::string>();
		}

		std::vector<std::string> getLabels_Spigot()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<SPIGOT_NUM; i++)
			{
				HcalElectronicsId eid = getEid_SlotVME(i);
				sprintf(name, "%d",
					eid.spigot());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_FiberuTCA()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FIBER_uTCA_NUM; i++)
			{
				HcalElectronicsId eid = getEid_FiberuTCA(i);
				sprintf(name, "%d",
					eid.fiberIndex());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_FiberVME()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FIBER_VME_NUM; i++)
			{
				HcalElectronicsId eid = getEid_FiberVME(i);
				sprintf(name, "%d",
					eid.fiberIndex());
				labels.push_back(std::string(name));
			}
			return labels;
		}

		std::vector<std::string> getLabels_FiberCh()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FIBERCH_NUM; i++)
			{
				HcalElectronicsId eid = getEid_FiberCh(i);
				sprintf(name, "%d",
					eid.fiberChanId());
				labels.push_back(std::string(name));
			}
			return labels;
		}
	}
}
