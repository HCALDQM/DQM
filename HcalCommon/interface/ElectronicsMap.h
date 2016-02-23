#ifndef ElectronicsMap_h
#define ElectronicsMap_h

/**
 *	file:		ElectronicsMap.h
 *	Author:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/HashMapper.h"
#include "DQM/HcalCommon/interface/HashFilter.h"

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
				//	filter is to filter HcalElectronicsId out 
				void initialize(HcalElectronicsMap const*, ElectronicsMapType,
					filter::HashFilter const&);
				const HcalElectronicsId lookup(DetId const&);
				const HcalElectronicsId lookupTrigger(DetId const&);

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
