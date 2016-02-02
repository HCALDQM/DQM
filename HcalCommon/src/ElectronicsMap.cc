#include "DQM/HcalCommon/interface/ElectronicsMap.h"

namespace hcaldqm
{
	namespace electronicsmap
	{
		void ElectronicsMap::initialize(HcalElectronicsMap const* emap)
		{
			_emap = emap;
			//	if we actually use a HashMap then 
			if (etype!=fHcalElectronicsMap)
			{
				if (_etype==fDHashMap)
				{
					std::vector<HcalGenericDetId> dids=emap->allPrecisionId();
					for (std::ivector<HcalGenericDetId>::const_iterator it=
						dids.begin(); it!=dids.end(); ++it)
					{
						if (!it->isHcalDetId())
							continue;

						HcalElectronicsId eid = _emap->lookup(
							HcalDetId(it->rawId()));
						uint32_t hash = it->rawId();
						EMapType::iterator eit = _ids.find(hash);
						if (eit!=_ids.end())
							continue;

						_ids.insert(
							std::make_pair(hash, eid));
					}
					else if (_etype==fTHashMap)
					{
						std::vector<HcalTrigTowerDetId> tids = 
							emap->allTriggerId();
						for (std::vector<HcalTrigTowerDetId>::const_iterator it=
							tids.begin(); it!=tids.end(); ++it)
						{

							HcalElectronicsMap eid = _emap->lookupTrigger(*it);
							uint32_t hash = it->rawId();
							EMapType::iterator eit = _ids.find(hash);
							if (eit!=_ids.end())
								continue;

							_ids.insert(std::make_pair(hash, eid));
						}
					}
				}
			}
		}

		HcalElectronicsId const ElectronicsMap::lookup(DetId const did)
		{
			return _etype==fDHashMap?_ids[_hashmap.getHash(HcalDetId(did))]:
				_emap->lookup(did);
		}

		HcalElectronicsId const ElectronicsMap::lookupTrigger(DetId const did)
		{
			return _etype==fTHashMap?
				_ids[_hashmap.getHash(HcalTrigTowerDetId(did))]:
				_emap->lookup(did);
		}
	}
}
