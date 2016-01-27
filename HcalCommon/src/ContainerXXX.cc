
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

				uint32_t hash = it->rawId();
				HcalDetId did = HcalDetId(hash);
				CompactMap::iterator mit = _cmap.find(hash);
				if (mit!=_cmap.end())
					continue;

				_cmap.insert(
					std::make_pair(_hashmap.getHash(did), CompactX()));
			}
		}
	}

	/* virtual */ void ContainerXXX::fill(HcalDetId const& did, double x)
	{
		CompactX &c = _cmap[_hashmap.getHash(did)];
		c._sum += x;
		c._sum2 += x*x;
		c._entries++;
	}

	/* virtual */ uint32_t ContainerXXX::size()
	{
		return (uint32_t)(_cmap.size());
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
			std::cout << HcalDetId(p.first) << p.second << std::endl;
		}
	}

	/* virtual */ void ContainerXXX::reset()
	{
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			p.second.reset();
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
		if (_hashmap.getHashType()!=cx._hashmap.getHashType())
			return;

		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			CompactX &x = p.second;
			uint32_t hash = p.first;
			CompactMap::const_iterator it = cx._cmap.find(hash);

			if (x._entries<=0)
				continue;
			if (it==cx._cmap.end())
				continue;

			CompactX y = it->second;
			HcalDetId did(hash);

			double meanX = x._sum/x._entries;
			double rmsX = sqrt(x._sum2/x._entries - meanX*meanX);
			double meanY = y._sum/y._entries;
			double rmsY = sqrt(y._sum2/y._entries - meanY*meanY);
			q ? cc->fill(did, meanX-meanY) : cc->fill(did, rmsX-rmsY);
		}
	}
}


