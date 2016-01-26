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
			return v;
		}

		int getValue_FEDuTCA(HcalElectronicsId const& eid)
		{
			return 0;
		}

		int getValue_FEDVME(HcalElectronicsId const& eid)
		{
			return eid.dccid();
		}

		int getValue_Crate(HcalElectronicsId const& eid)
		{
			int c = eid.crateId();
			if (eid.isUTCAid())
				c = 0;
			return c;
		}

		int getValue_CrateuTCA(HcalElectronicsId const& eid)
		{
			return 0;
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
	}
}
