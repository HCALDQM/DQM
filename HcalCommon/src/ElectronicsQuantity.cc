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

		int getValue_FEDuTCASlot(HcalElectronicsId const& eid)
		{
			int ifed = getValue_FEDuTCA(eid);
			int islot = getValue_SlotuTCA(eid);
			return ifed*SLOT_uTCA_NUM+islot;
		}

		int getValue_FEDVMESpigot(HcalElectronicsId const& eid)
		{
			int ifed = getValue_FEDVME(eid);
			int ispigot = getValue_Spigot(eid);
			return ifed*SPIGOT_NUM+ispigot;
		}

		int getValue_FiberuTCAFiberCh(HcalElectronicsId const &eid)
		{
			int ifiber = getValue_FiberuTCA(eid);
			int ifch = getValue_FiberCh(eid);
			return ifiber*FIBERCH_NUM+ifch;
		}

		int getValue_FiberVMEFiberCh(HcalElectronicsId const &eid)
		{
			int ifiber = getValue_FiberVME(eid);
			int ifch = getValue_FiberCh(eid);
			return ifiber*FIBERCH_NUM+ifch;
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

		uint32_t getBin_FEDuTCASlot(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FEDuTCASlot(eid)+1);
		}

		uint32_t getBin_FEDVMESpigot(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FEDVMESpigot(eid)+1);
		}

		uint32_t getBin_FiberuTCAFiberCh(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FiberuTCAFiberCh(eid)+1);
		}

		uint32_t getBin_FiberVMEFiberCh(HcalElectronicsId const& eid)
		{
			return (uint32_t)(getValue_FiberVMEFiberCh(eid)+1);
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

		HcalElectronicsId getEid_FEDuTCASlot(int v)
		{
			HcalElectronicsId fedeid = getEid_FEDuTCA(v/SLOT_uTCA_NUM);
			HcalElectronicsId sloteid = getEid_SlotuTCA(v%SLOT_uTCA_NUM);
			return HcalElectronicsId(
				fedeid.crateId(), sloteid.slot(), FIBER_uTCA_MIN1,
				FIBERCH_MIN, false);
		}

		HcalElectronicsId getEid_FEDVMESpigot(int v)
		{
			HcalElectronicsId fedeid = getEid_FEDVME(v/SPIGOT_NUM);
			HcalElectronicsId spid = getEid_Spigot(v%SPIGOT_NUM);
			return HcalElectronicsId(FIBERCH_MIN, FIBER_VME_MIN,
				spid.spigot(), fedeid.dccid());
		}

		HcalElectronicsId getEid_FiberuTCAFiberCh(int v)
		{
			HcalElectronicsId fibereid = getEid_FiberuTCA(v/FIBERCH_NUM);
			HcalElectronicsId fcheid = getEid_FiberCh(v%FIBERCH_NUM);
			return HcalElectronicsId(CRATE_uTCA_MIN, SLOT_uTCA_MIN,
				fibereid.fiberIndex(), fcheid.fiberChanId(), false);
		}

		HcalElectronicsId getEid_FiberVMEFiberCh(int v)
		{
			HcalElectronicsId fibereid = getEid_FiberVME(v/FIBERCH_NUM);
			HcalElectronicsId fcheid = getEid_FiberCh(v%FIBERCH_NUM);
			return HcalElectronicsId(fcheid.fiberChanId(),
				fibereid.fiberIndex(), SPIGOT_MIN, CRATE_VME_MIN);
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
			for (int i=0; i<FED_VME_NUM; i++)
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
			return std::vector<std::string>();
/*			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<SPIGOT_NUM; i++)
			{
				HcalElectronicsId eid = getEid_Spigot(i);
				sprintf(name, "%d",
					eid.spigot());
				labels.push_back(std::string(name));
			}
			return labels;
	*/
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

		std::vector<std::string> getLabels_FEDuTCASlot()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FED_uTCA_NUM; i++)
				for (int j=0; j<SLOT_uTCA_NUM; j++)
				{
					if (j>0)
					{
						labels.push_back(std::string(""));
						continue;
					}
					HcalElectronicsId eid = getEid_FEDuTCASlot(
						i*SLOT_uTCA_NUM+j);
					sprintf(name, "%d-%d", 
						utilities::crate2fed(eid.crateId()),
						eid.slot());
					labels.push_back(std::string(name));
				}
			return labels;
		}

		std::vector<std::string> getLabels_FEDVMESpigot()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FED_VME_NUM; i++)
				for (int j=0; j<SPIGOT_NUM; j++)
				{
					if (j>0)
					{
						labels.push_back(std::string(""));
						continue;
					}

					HcalElectronicsId eid = getEid_FEDVMESpigot(
						i*SPIGOT_NUM+j);
					sprintf(name, "%d-%d",
						eid.dccid()+FED_VME_MIN, eid.spigot());
					labels.push_back(std::string(name));
				}
			return labels;
		}

		std::vector<std::string> getLabels_FiberuTCAFiberCh()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FIBER_uTCA_NUM; i++)
				for (int j=0; j<FIBERCH_NUM; j++)
				{
					if (j>0)
					{
						labels.push_back(std::string(""));
						continue;
					}

					HcalElectronicsId eid = getEid_FiberuTCAFiberCh(
						i*FIBERCH_NUM+j);
					sprintf(name, "%d-%d", eid.fiberIndex(),
						eid.fiberChanId());
					labels.push_back(std::string(name));
				}
			return labels;
		}

		std::vector<std::string> getLabels_FiberVMEFiberCh()
		{
			std::vector<std::string> labels;
			char name[10];
			for (int i=0; i<FIBER_VME_NUM; i++)
				for (int j=0; j<FIBERCH_NUM; j++)
				{
					if (j>0)
					{
						labels.push_back(std::string(""));
						continue;
					}
					HcalElectronicsId eid = getEid_FiberVMEFiberCh(
						i*FIBERCH_NUM+j);
					sprintf(name, "%d-%d", eid.fiberIndex(),
						eid.fiberChanId());
					labels.push_back(std::string(name));
				}
			return labels;
		}
	}
}
