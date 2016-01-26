
#include "DQM/HcalCommon/interface/ContainerXXX.h"

namespace hcaldqm
{
	using namespace constants;

	/* virtual */ void ContainerXXX::initialize(hashfunctions::HashType ht)
	{
		_hashmap.initialize(ht);
	}

	/* virtual */ void ContainerXXX::book(HcalElectronicsMap const* emap)
	{
		if (_hashmap.isDHash())
		{
			std::vector<HcalGenericDetId> dids = emap->allPrecisionId();
			for (std::vector<HcalGenericDetId>::const_iterator it=
				dids.begin(); it!=dids.end(); ++it)
			{
				if (!it->isHcalDetId())
					continue;

				HcalDetId did = HcalDetId(it->rawId());
				_mes.insert(
					std::make_pair(_hashmap.getHash(did), Compact()));
			}
		}
	}

	/* virtual */ void ContainerXXX::fill(HcalDetId const& did, double x)
	{
		if (x==GARBAGE_VALUE)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._sum += x;
		c._sum2 += x*x;
		c._entries++;
	}

	/* virtual */ void ContainerXXX::dump(Container1D* c, bool q)
	{
		BOOST_FOREACH(MEMap::value_type &p, _cmap)
		{
			Compact &x = _cmap[p.first];
			if (x._entries<=0)
				continue;

			double mean = x._sum/x._entries;
			double rms = sqrt(x._sum2/x._entries - mean*mean);
			q ? c->fill(HcalDetId(p.first), mean) : 
				c->fill(HcalDetId(p.first), rms);
		}
	}
}


