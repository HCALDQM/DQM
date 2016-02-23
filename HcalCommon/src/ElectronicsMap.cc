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
				else if (_etype==fDHashMap_2E1D)
				{
					std::vector<HcalElectronicsId> eids = 
						emap->allElectronicsIdPrecision();
					for (std::vector<HcalElectronicsId>::const_iterator it=
						eids.begin(); it!=eids.end(); ++it)
					{
						HcalGenericDetId did = HcalGenericDetId(
							_emap->lookup(*it));

						//	check that it is an HcalDetId
						if (!did.isHcalDetId())
							continue;

						_ids.insert(std::make_pair(did.rawId(), 
							*it));
					}
				}
				else if (_etype==fTHashMap_2E1T)
				{
					std::vector<HcalElectronicsId> eids = 
						emap->allElectronicsIdTrigger();
					for (std::vector<HcalElectronicsId>::const_iterator it=
						eids.begin(); it!=eids.end(); ++it)
					{
						HcalTrigTowerDetId tid = _emap->lookupTrigger(*it);
						_ids.insert(std::make_pair(tid.rawId(),
							*it));
					}
				}
			}
		}

		//	2 funcs below are only for 1->1 mappings
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

		//	2 funcs below are for 2->1 mappings
		std::vector<HcalElectronicsId> ElectronicsMap::lookup(DetId const& did)
		{
			uint32_t hash = did.rawId();
			std::vector<HcalElectronicsId> eids;
			std::pair<EMapType::const_iterator, EMapType::const_iterator> p = 
				_ids.equal_range(hash);
			if (p.first==_ids.end() && p.second==_ids.end())
				return eids;

			eids.push_back(p.first.second);
			eids.push_back(p.second.second);
			return eids;
		}
		std::vector<HcalElectronicsId> ElectronicsMap::lookupTrigger(
			DetId const& did)
		{
			uint32_t hash = did.rawId();
			std::vector<HcalElectronicsId> eids;
			std::pair<EMapType::const_iterator, EMapType::const_iterator> p = 
				_ids.equal_range(hash);
			if (p.first==_ids.end() && p.second==_ids.end())
				return eids;

			eids.push_back(p.first.second);
			eids.push_back(p.second.second);
			return eids;
		}

		void ElectronicsMap::print()
		{
			std::cout << "ElectronicsMap" << std::endl;
			BOOST_FOREACH(EMapType::value_type &v, _ids)
			{
				std::cout << v.first << "  "<< v.second << std::endl;
			}
		}
	}
}
