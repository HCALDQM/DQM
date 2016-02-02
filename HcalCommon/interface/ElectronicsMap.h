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
			nElectronicsMapType = 3
		};

		class ElectronicsMap()
		{
			public:
				//	define how to use upon construction
				ElectronicsMap(ElectronicsMapType etype) : 
					_etype(etype), _emap(NULL)
				{}
				~ElectronicsMap() {}

				void initialize(HcalElectronicsMap const*);
				HcalElectronicsId const lookup(DetId const&) const;
				HcalElectronicsId const lookupTrigger(DetId const&) const;
				

			private:
				//	configures how to use emap
				ElectronicsMapType	_etype;

				//	2 choices either use as HcalElectronicsMap or as ur hash
				typedef boost::unordered_map<uint32_t, HcalElectronicsId> 
					EMapType;
				EMapType			_ids;

				//	
				HcalElectronicsMap const* _emap;
		}
	}
}

#endif
