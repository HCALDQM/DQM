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

#include <string>
#include <vector>

namespace hcaldqm
{
	namespace mapper
	{
		/*
		 *	Mapper Type enum:
		 *		
		 */
		enum MapperType
		{
			//	By HCAL subdetector
			fSubDet = 0,
	
			//	By Detector Coordinate
			fiphi = 1,
			fieta = 2,
			fdepth = 3,

			//	Detector Combinations
			fSubDet_iphi = 4;
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
		index_generator vindex[nMapperType] = { generate_fSubDet,
			generate_fiphi, generate_fieta, generate_fdepth, 
			generate_fSubDet_iphi, generate_fSubDet_ieta,
			genrate_fFEDf, generate_fCrate, generate_fFED_Slot, 
			generate_fCrate_Slot};

		/*
		 *	Mapper Class
		 */
		class Mapper
		{
			public:
				Mapper() {}
				Mapper(MapperType type) : _type(type)
				{}
				virtual ~Mapper() {}
	
				virtual unsigned int index() {return 0;}
				virtual unsigned int index(double) { return 0;}
				virtual unsigned int index(int x) 
				{
					if (_type==fFED)
						return vindex[_type](x);
					
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
						i.i2 = did.ieta()
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
						i.i2 = eid.
					}
					return vindex[_type](x);
				}
	
			protected:
				MapperType			_type;
		};
	}
}

#endif








