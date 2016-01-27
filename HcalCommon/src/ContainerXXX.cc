
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
				_cmap.insert(
					std::make_pair(_hashmap.getHash(did), CompactX()));
			}
		}
	}

	/* virtual */ void ContainerXXX::fill(HcalDetId const& did, double x)
	{
		if (x==GARBAGE_VALUE)
			return;

		CompactX &c = _cmap[_hashmap.getHash(did)];
		c._sum += x;
		c._sum2 += x*x;
		c._entries++;
	}

	/* virtual */ void ContainerXXX::dump(Container1D* c, bool q)
	{
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			CompactX &x = p.second;
			if (x._entries<=0)
				continue;

			double mean = x._sum/x._entries;
			double rms = sqrt(x._sum2/x._entries - mean*mean);
			q ? c->fill(HcalDetId(p.first), mean) : 
				c->fill(HcalDetId(p.first), rms);
		}
	}

	/* virtual */ void ContainerXXX::print()
	{
		std::cout << "Container by " << _hashmap.getHashTypeName() << std::endl;
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			std::cout << HcalDetId(p.first) << std::endl;
		}
	}

	/* virtual */ void ContainerXXX::load(Container1D* cont)
	{
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			CompactX &x = p.second;
			uint32_t hash = p.first;
			HcalDetId did(hash);

			double value = cont->getBinContent(did);
			x._sum += value;
			x._sum2 += value*value;
			x._entries++;
		}	
	}

	/* virtual */ void ContainerXXX::compare(ContainerXXX const& cx, 
		Container1D* cc, bool q)
	{
		if (_hashmap.getHashType()!=cx.getHashType())
			return;

		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			CompactX &x = p.second;
			uint32_t hash = p.first;
			CompactMap::iterator y = cx._cmap.find(hash);
			if (y==cx._cmap.end())
				continue;
			HcalDetId did(hash);

			double meanX = x._sum/x._entries;
			double rmsX = sqrt(x._sum2/x._entries - meanX*meanX);
			double meanY = x->_sum/y->_entries;
			double rmsY = sqrt(y->_sum2/y->_entries - meanY*meanY);
			q ? cc->fill(did, meanX-meanY) : cc->fill(rmsX-rmsY);
		}
	}
}


