#include "DQM/HcalCommon/interface/ElectronicsMap.h"

namespace hcaldqm
{
	namespace electronicsmap
	{
		void ElectronicsMap::initialize(HcalElectronicsMap const* emap,
			ElectronicsMapType etype/*=fHcalElectronicsMap*/)
		{
			_etype=etype;
			_emap = emap;
			//	if we actually use a HashMap then 
			if (_etype!=fHcalElectronicsMap)
			{
				if (_etype==fDHashMap)
				{
					std::vector<HcalGenericDetId> dids=emap->allPrecisionId();
					for (std::vector<HcalGenericDetId>::const_iterator it=
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
				}
				else if (_etype==fTHashMap)
				{
					std::vector<HcalTrigTowerDetId> tids = 
						emap->allTriggerId();
					for (std::vector<HcalTrigTowerDetId>::const_iterator it=
						tids.begin(); it!=tids.end(); ++it)
					{
						HcalElectronicsId eid = _emap->lookupTrigger(*it);
						uint32_t hash = it->rawId();
						EMapType::iterator eit = _ids.find(hash);
						if (eit!=_ids.end())
							continue;
							_ids.insert(std::make_pair(hash, eid));	
					}
				}
			}
		}

		const HcalElectronicsId ElectronicsMap::lookup(DetId const &did)
		{
			uint32_t hash = did.rawId();
			return _etype==fDHashMap? _ids[hash]:_emap->lookup(did);
		}

		const HcalElectronicsId  ElectronicsMap::lookupTrigger(DetId const &did)
		{
			uint32_t hash = did.rawId();
			return _etype==fTHashMap? _ids[hash]:_emap->lookup(did);
		}

		void ElectronicsMap::print()
		{
			std::cout << "ElectronicsMap" << std::endl;
			BOOST_FOREACH(EMapType::value_type &v, _ids)
			{
				std::cout << v.second << std::endl;
			}
		}
	}
}
