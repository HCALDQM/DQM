#ifndef ElectronicsMap_h
#define ElectronicsMap_h

/**
 *	file:		ElectronicsMap.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/HashMapper.h"

#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
#include "string"

namespace hcaldqm
{
	namespace electronicsmap
	{
		enum ElectronicsMapType
		{
			fHcalElectronicsMap = 0,
			fDHashMap = 1,
			fTHashMap = 2,
			fDHashMap_2E1D = 3,
			fTHashMap_2E1T = 4,
			nElectronicsMapType = 5
		};

		class ElectronicsMap
		{
			public:
				ElectronicsMap() :
					_emap(NULL)
				{}
				//	define how to use upon construction
				ElectronicsMap(ElectronicsMapType etype) : 
					_etype(etype), _emap(NULL)
				{}
				~ElectronicsMap() {}

				void initialize(HcalElectronicsMap const*, ElectronicsMapType
					etype=fHcalElectronicsMap);
				const HcalElectronicsId lookup(DetId const&);
				const HcalElectronicsId lookupTrigger(DetId const&);
				std::vector<HcalElectronicsId> glookup(DetId const&);
				std::vector<HcalElectronicsId> glookupTrigger(DetId const&);

				void print();
				

			private:
				//	configures how to use emap
				ElectronicsMapType	_etype;

				//	2 choices either use as HcalElectronicsMap or as ur hash
				typedef boost::unordered_map<uint32_t, HcalElectronicsId> 
					EMapType;
				EMapType			_ids;

				//	
				HcalElectronicsMap const* _emap;
		};
	}
}

#endif
