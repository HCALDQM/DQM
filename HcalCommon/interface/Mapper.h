#ifndef Mapper_h
#define Mapper_h

/*
 *	file:		Mapper.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Internal Mapper of vector<ME*> indices. All the possibilities should
 *		be predefined
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

#include <string>
#include <vector>
#include <sstream>

namespace hcaldqm
{
	namespace mapper
	{
		/*
		 *	Mapper Type enum:
		 *		
		 */
		using namespace hcaldqm::constants;
		enum MapperType
		{
			//	By HCAL subdetector
			fSubDet = 0,
	
			//	By Detector Coordinate
			fiphi = 1,
			fieta = 2,
			fdepth = 3,

			//	Detector Combinations
			fSubDet_iphi = 4,
			fSubDet_ieta = 5,
	
			//	By Electronics Coordinate
			fFED = 6,
			fCrate = 7,

			//	Double Electronics Combinations
			fFED_Slot = 8,
			fCrate_Slot = 9,

			nMapperType = 10
		};

		/*
		 *	Index Generation Functions - generate index based on input
		 */
		struct Input
		{
			int i1;
			int i2;
		};
		typedef unsigned int(*index_generator)(Input const&);
		unsigned int generate_fSubDet(Input const&);
		unsigned int generate_fiphi(Input const&);
		unsigned int generate_fieta(Input const&); 
		unsigned int generate_fdepth(Input const&);
		unsigned int generate_fSubDet_iphi(Input const&);
		unsigned int generate_fSubDet_ieta(Input const&);
		unsigned int generate_fFED(Input const&);
		unsigned int generate_fCrate(Input const&);
		unsigned int generate_fFED_Slot(Input const&);
		unsigned int generate_fCrate_Slot(Input const&);
		index_generator const vindex[nMapperType] = { generate_fSubDet,
			generate_fiphi, generate_fieta, generate_fdepth, 
			generate_fSubDet_iphi, generate_fSubDet_ieta,
			generate_fFED, generate_fCrate, generate_fFED_Slot, 
			generate_fCrate_Slot};

		/*
		 *	Mapper Class
		 */
		class Mapper
		{
			public:
				Mapper(): _type(fSubDet) {}
				Mapper(MapperType type) : _type(type)
				{
					this->setSize();
				}
				virtual ~Mapper() {}
	
				virtual void	configure(MapperType type)
				{
					_type = type;
				}
				virtual unsigned int index() {return 0;}
				virtual unsigned int index(double) { return 0;}
				virtual unsigned int index(int x) 
				{
					Input i; i.i1=x;
					if (_type==fFED)
						return vindex[_type](i);
					
					return 0; 
				}

				virtual unsigned int index(HcalDetId const& did)
				{
					Input i;
					if (_type==fSubDet)
						i.i1 = did.subdet();
					else if (_type==fiphi)
						i.i1 = did.iphi();
					else if (_type==fieta)
						i.i1 = did.ieta();
					else if (_type==fdepth)
						i.i1 = did.depth();
					else if (_type==fSubDet_iphi)
					{
						i.i1 = did.subdet();
						i.i2 = did.iphi();
					}
					else if (_type==fSubDet_ieta)
					{
						i.i1 = did.subdet();
						i.i2 = did.ieta();
					}

					return vindex[_type](i);
				}
				virtual unsigned int index(HcalElectronicsId const& eid)
				{
					Input i;
					if (_type==fCrate)
						i.i1 = eid.crateId();
					else if (_type==fCrate_Slot)
					{
						i.i1 = eid.crateId();
						i.i2 = eid.slot();
					}
					return vindex[_type](i);
				}

				virtual std::string buildName(unsigned id)
				{
					std::string builtname;
					switch(_type)
					{
						case fSubDet:
							builtname = constants::SUBDET_NAME[id];	
							break;
						case fiphi :
						{
							char name[10];
							sprintf(name, "iphi%d", 
								constants::IPHI_MIN+id*constants::IPHI_DELTA);
							builtname = name;
							break;
						}
						case fieta :
						{
							char name[10];
							int ieta = id<=(constants::IETA_NUM/2) ? 
								-(constants::IETA_MIN+id*constants::IETA_DELTA) : 
								(id-constants::IETA_NUM/2)*constants::IETA_DELTA 
								+ constants::IETA_MIN;
							sprintf(name, "ieta%d", ieta);
							builtname = name;
							break;
						}
						case fdepth:
						{
							char name[10];
							sprintf(name, "Depth%d", id+1);
							builtname = name;
							break;
						}
						case fSubDet_iphi:
						{
							char name[20];
							if (_size>=IPHI_NUM*3) // HF
								sprintf(name, "HFiphi%d",
									(id-3*constants::IPHI_NUM)*
									constants::IPHI_DELTA_HF+constants::IPHI_MIN);
							else if (_size>=2*constants::IPHI_NUM) // HO
								sprintf(name, "HOiphi%d",
									(id-2*constants::IPHI_NUM)*
									constants::IPHI_DELTA+constants::IPHI_MIN);
							else if (_size>=constants::IPHI_NUM) // HE
								sprintf(name, "HEiphi%d",
									(id-constants::IPHI_NUM)*
									constants::IPHI_DELTA+constants::IPHI_MIN);
							else 
								sprintf(name, "HBiphi%d",
									id*constants::IPHI_DELTA+constants::IPHI_MIN);
							
							builtname = name;
							break;
						}
						case fSubDet_ieta:
						{
							char name[20];
							unsigned int totalHB = IETA_MAX_HB-IETA_MIN_HB+1;
							unsigned int totalHE = IETA_MAX_HE-IETA_MIN_HE+1;
							unsigned int totalHO = IETA_MAX_HO-IETA_MIN_HO+1;
							unsigned int totalHF = IETA_MAX_HF-IETA_MIN_HF+1;
							if (_size>=(2*(totalHB+totalHE+totalHO)+totalHF))
								sprintf(name, "HFPieta%d", 
									(id-2*totalHB-2*totalHE-2*totalHO-totalHF) + 
									IETA_MIN_HF);
							else if (_size>=(2*totalHB + 2*totalHE + 2*totalHO))
								sprintf(name, "HFMieta%d",
									-((id-2*totalHB-2*totalHE-2*totalHO) + 
									IETA_MIN_HF));
							else if (_size>=(2*totalHB+2*totalHE+totalHO))
								sprintf(name, "HOPieta%d", 
									(id-2*totalHB-2*totalHE-totalHO + 
									 IETA_MIN_HO));
							else if (_size>=(2*totalHB+2*totalHE))
								sprintf(name, "HOMieta%d",
									-(id-2*totalHB-2*totalHE + IETA_MIN_HO));
							else if (_size>=(2*totalHB+totalHE))
								sprintf(name, "HEPieta%d", 
									(id-2*totalHB-totalHE + IETA_MIN_HE));
							else if (_size>=(2*totalHB))
								sprintf(name, "HEMieta%d",
									-(id-2*totalHB+IETA_MIN_HE));
							else if (_size>=totalHB)
								sprintf(name, "HBPieta%d",
									id-totalHB+IETA_MIN_HB);
							else 
								sprintf(name, "HBMieta%d",
									id+IETA_MIN_HB);

							builtname = name;
							break;
						}
						case fCrate:
						{
							char name[20];
							if (id>=CRATE_VME_NUM)
								sprintf(name, "CRATE%d",
									(id-CRATE_VME_NUM)*CRATE_uTCA_DELTA + 
									CRATE_uTCA_MIN);
							else
								sprintf(name, "CRATE%d",
									id*CRATE_VME_DELTA+CRATE_uTCA_MIN);

							builtname = name;
							break;
						}
						case fFED:
						{
							char name[20];
							if (id>=FED_VME_NUM)
								sprintf(name, "FED%d", 
									(id-FED_VME_NUM)*FED_uTCA_DELTA+FED_uTCA_MIN);
							else
								sprintf(name, "FED%d",
									id*CRATE_VME_DELTA+CRATE_VME_MIN);	
							builtname = name;
							break;
						}
						case fCrate_Slot:
						{
							char name[20];
							if (id>=CRATE_VME_NUM*SLOT_VME_NUM)
							{
								id -= CRATE_VME_NUM*SLOT_VME_NUM;
								int icrate = id/CRATE_uTCA_NUM;
								int islot = id%SLOT_uTCA_NUM;
								sprintf(name, "CRATE%dSLOT%d",
									icrate+CRATE_uTCA_MIN, islot+SLOT_uTCA_MIN);
							}
							else 
							{
								int icrate = id/CRATE_VME_NUM;
								int islot = id%SLOT_VME_NUM;
								if (islot>=SLOT_VME_NUM1)
									sprintf(name, "CRATE%dSLOT%d",
										icrate+CRATE_VME_MIN,
										islot+SLOT_VME_MIN2);
								else 
									sprintf(name, "CRATE%dSLOT%d",
										icrate+CRATE_VME_MIN,
										islot+SLOT_VME_MIN);
							}
							builtname = name;
							break;
						}
						default:
						{
							return std::string("UNKNOWN");
							break;
						}
					}
					return builtname;
				}

				inline unsigned int getSize() {return _size;}
	
			protected:
				MapperType			_type;
				unsigned int		_size;
				void setSize()
				{
					switch (_type)
					{
						case fSubDet : 
							_size = SUBDET_NUM;
							break;
						case fiphi:
							_size = IPHI_NUM;
							break;
						case fieta:
							_size = IETA_NUM;
							break;
						case fdepth:
							_size = DEPTH_NUM;
							break;
						case fSubDet_iphi:
							_size = SUBDET_NUM*IPHI_NUM;
							break;
						case fSubDet_ieta:
							_size = SUBDET_NUM*IETA_NUM;
							break;
						case fFED:
							_size = FED_VME_NUM+FED_uTCA_NUM;
							break;
						case fCrate:
							_size = CRATE_VME_NUM+CRATE_uTCA_NUM; 
							break;
						case fCrate_Slot:
							_size = CRATE_VME_NUM*SLOT_VME_NUM + 
								CRATE_uTCA_NUM*SLOT_uTCA_NUM;
							break;
						default:
							_size = 0;
					}
				}
		};
	}
}

#endif








