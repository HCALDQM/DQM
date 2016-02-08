#include "DQM/HcalCommon/interface/HashFilter.h"

namespace hcaldqm
{
	namespace filter
	{
		HashFilter::HashFilter(FilterType ftype, HashType htype) : 
			HashMapper(htype), _ftype(htype)
		{}

		HashFilter::HashFilter(FilterType ftype, HashType htype,
			std::vector<uint32_t> const& v) :
			HashMapper(htype), _ftype(ftype)
		{
			for (std::vector<uint32_t>::const_iterator it=v.begin();
				it!=v.end(); ++it)
			{
				_ids.insert(*it);		
			}
		}

		HashFilter::HashFilter(HashFilter const& hf) :
			HashMapper(hf._htype), _ftype(hf._ftype)
		{
			_ids = hf._ids;
		}

		/* virtual */ void HashFilter::initialize(FilterType ftype,
			HashType htype, std::vector<uint32_t> const& v)
		{
			HashMapper::initialize(htype);
			_ftype = hf._ftype;
			_ids = hf._ids;
		}

		/* virtual */ bool HashFilter::filter(HcalDetId const& did)
		{
			return _ftype==fFilter?
				filter(getHash(did)):preserve(getHash(did));
		}

		/* virtual */ bool HashFilter::filter(HcalElectronicsId const& eid)
		{
			return _ftype==fFilter?
				filter(getHash(eid)):preserve(getHash(eid));
		}

		/* virtual */ bool HashFilter::filter(HcalTrigTowerDetId const& tid)
		{
			return _ftype==fFilter?
				filter(getHash(tid)):preserve(getHash(tid));
		}

		/* virtual */ bool HashFilter::filter(uint32_t id)
		{
			BOOST_FOREACH(FilterMap::value_type &hash, _ids)
			{
				if (hash==id)
					return true;
			}

			return false;
		}

		/* virtual */ bool HashFilter::preserve(uint32_t id)
		{
			BOOST_FOREACH(FilterMap::value_type &hash, _ids)
			{
				if (hash==id)
					return false;
			}

			return true;
		}
	}
}
