#include "DQM/HcalCommon/interface/ElectronicsMap.h"
#include <iomanip>

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

		void ElectronicsMap::initialize(HcalElectronicsMap const* emap,
			ElectronicsMapType etype, filter::HashFilter const& filter)
		{
			_etype=etype;
			_emap = emap;
			
			//	note this initialization has iteration over electronics not 
			//	detector
			if (_etype!=fHcalElectronicsMap)
			{
				if (_etype==fDHashMap)
				{
					std::vector<HcalElectronicsId> eids = 
						emap->allElectronicsIdPrecision();
					for (std::vector<HcalElectronicsId>::const_iterator it=
						eids.begin(); it!=eids.end(); ++it)
					{
						HcalGenericDetId did = HcalGenericDetId(
							_emap->lookup(*it));
						if (filter.filter(*it))
							continue;
						if (!did.isHcalDetId())
							continue;

						_ids.insert(std::make_pair(did.rawId(), *it));
					}
				}
				else if (_etype==fTHashMap)
				{
					std::vector<HcalElectronicsId> eids=
						emap->allElectronicsIdTrigger();
					for (std::vector<HcalElectronicsId>::const_iterator it=
						eids.begin(); it!=eids.end(); ++it)
					{
						if (filter.filter(*it))
							continue;
						HcalTrigTowerDetId tid = emap->lookupTrigger(*it);
						std::cout << tid << std::endl;
						std::cout << *it << std::endl;
						_ids.insert(std::make_pair(tid.rawId(), *it));
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

		void ElectronicsMap::print()
		{
			std::cout << "ElectronicsMap" << std::endl;
			BOOST_FOREACH(EMapType::value_type &v, _ids)
			{
				std::cout << std::hex << v.first 
					<< std::dec << "  "<< v.second << std::endl;
			}
		}
	}
}
