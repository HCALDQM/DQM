#ifndef Mapper_h
#define Mapper_h

/*
 *	file:		Mapper.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"
#include "DQM/HcalCommon/interface/Logger.h"

#include <string>
#include <vector>
#include <sstream>

namespace hcaldqm
{
	namespace mapper
	{
<<<<<<< HEAD
=======
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
			int i3;
			int i4;
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
>>>>>>> 6c9a6dfd557563eb738a42603a65975db39832b3
		class Mapper
		{
			public:
				Mapper()
				{}
				virtual ~Mapper() {}

				virtual uint32_t getHash(HcalDetId const&) {return 0;}
				virtual uint32_t getHash(HcalElectronicsId const&) {return 0;}
				virtual uint32_t getHash(HcalTrigTowerDetId const&) {return 0;}

				virtual std::string getName(HcalDetId const&) {return "";}
				virtual std::string getName(HcalElectronicsId const&) 
				{return "";}
				virtual std::string getName(HcalTrigTowerDetId const&)
				{return "";}

			protected:
		};
	}
}

#endif





