#ifndef ElectronicsQuantity_h
#define ElectronicsQauntity_h

/**
 *	file:		ElectronicsQuantity.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/Quantity.h"

namespace hcaldqm
{
	namespace quantity
	{
		enum ElectronicsQuantityType
		{
			fFED = 0,
			fFEDuTCA = 1,
			fFEDVME = 2,
			fCrate = 3,
			fCrateuTCA = 4,
			fCrateVME = 5,
			fSlotuTCA = 6,
			fSlotVME = 7,
			fSpigot = 8
			fFiberuTCA = 9,
			fFiberVME = 10,
			fFiberCh = 11,
			nElectronicsQuantityType = 12
		};

		int getValue_FED(HcalElectronicsId const&);
		int getValue_FEDuTCA(HcalElectronicsId const&);
		int getValue_FEDVME(HcalElectronicsId const&);
		int getValue_Crate(HcalElectronicsId const&);
		int getValue_CrateuTCA(HcalElectronicsId const&);
		int getValue_CrateVME(HcalElectronicsId const&);
		int getValue_SlotuTCA(HcalElectronicsId const&);
		int getValue_SlotVME(HcalElectronicsId const&);
		int getValue_Spigot(HcalElectronicsId const&);
		int getValue_FiberuTCA(HcalElectronicsId const&);
		int getValue_FiberVME(HcalElectronicsId const&);
		int getValue_FiberCh(HcalElectronicsId const&);
		uint32_t getBin_FED(HcalElectronicsId const&);
		uint32_t getBin_FEDuTCA(HcalElectronicsId const&);
		uint32_t getBin_FEDVME(HcalElectronicsId const&);
		uint32_t getBin_Crate(HcalElectronicsId const&);
		uint32_t getBin_CrateuTCA(HcalElectronicsId const&);
		uint32_t getBin_CrateVME(HcalElectronicsId const&);
		uint32_t getBin_SlotuTCA(HcalElectronicsId const&);
		uint32_t getBin_SlotVME(HcalElectronicsId const&);
		uint32_t getBin_Spigot(HcalElectronicsId const&);
		uint32_t getBin_FiberuTCA(HcalElectronicsId const&);
		uint32_t getBin_FiberVME(HcalElectronicsId const&);
		uint32_t getBin_FiberCh(HcalElectronicsId const&);

		typedef int(*getValueType_eid)(HcalElectronicsId const&);
		typedef uint32_t (*getBinType_eid)(HcalElectronicsId const&);
		getValueType_eid const getValue_functions_eid[nElectronicsQuantityType]
			= {
			getValue_FED, getValue_FEDuTCA, getValue_FEDVME,
			getValue_Crate, getValue_CrateuTCA, getValue_CrateVME,
			getValue_SlotuTCA, getValue_SlotVME,
			getValue_Spigot,
			getValue_FiberuTCA, getValue_FiberVME, getValue_FiberCh
		};
		getBinType_eid const getBin_functions_eid[nElectronicsQuantityType] = {
			getBin_FED, getBin_FEDuTCA, getBin_FEDVME,
			getBin_Crate, getBin_CrateuTCA, getBin_CrateVME,
			getBin_SlotuTCA, getBin_SlotVME, 
			getValue_Spigot,
			getBin_FiberuTCA, getBin_FiberVME, getBin_FiberCh
		};
		std::string const name_eid[nElectronicsQuantityType] = {
			"FED", "FEDuTCA", "FEDVME", 
			"Crate", "CrateuTCA", "CrateVME",
			"SlotuTCA", "SlotVME", 
			"Spigot"
			"FiberuTCA", "FiberVME", 
			"FiberCh"
		};
		double const min_eid[nElectronicsQuantityType] = {
			0, 0, 0, 
			0, 0, 0,
			0, 0,
			0,
			0, 0,
			0
		};
		double const max_eid[nElectronicsQuantityType] = {
			FED_TOTAL_NUM, FED_uTCA_NUM, FED_VME_NUM,
			CRATE_TOTAL_NUM, CRATE_uTCA_NUM, CRATE_VME_NUM,
			SLOT_uTCA_NUM, SLOT_VME_NUM,
			SPIGOT_NUM,
			FIBER_uTCA_NUM, FIBER_VME_NUM,
			FIBERCH_NUM
		};
		int const nbins_eid[nElectronicsQuantityType] = {
			FED_TOTAL_NUM, FED_uTCA_NUM, FED_VME_NUM,
			CRATE_TOTAL_NUM, CRATE_uTCA_NUM, CRATE_VME_NUM,
			SLOT_uTCA_NUM, SLOT_VME_NUM,
			SPIGOT_NUM,
			FIBER_uTCA_NUM, FIBER_VME_NUM,
			FIBERCH_NUM
		};

		class ElectronicsQuantity : public
		{
			public:
				ElectronicQuantity() {}
				ElectronicsQuantity(ElectronicsQuantityType type, 
					bool isLog=false) : 
					Quantity(name_eid[type], isLog), _type(type)
				{}
				virtual ~ElectronicsQuantity() {}

				virtual int getValue(HcalElectronicsId const& eid)
				{return getValue_functions_eid[_type](eid);}
				virtual uint32_t getBin(HcalElectronicsId const& eid)
				{return getBin_functions_eid[_type](eid);}
				virtual QuantityType type()
				{return fElectronicsQuantity;}

				virtual int nbins() {return nbins_eid[_type];}
				virtual double min() {return min_eid[_type];}
				virtual double max() {return max_eid[_type];}

				virtual bool isCoordinate() {return true;}

			protected:
				ElectronicsQuantityType _type;
		};
	}
}

#endif
