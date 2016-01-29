
#include "DQM/HcalCommon/interface/ContainerXXX.h"

namespace hcaldqm
{
	using namespace constants;

	double ratio(double x, double y)
	{return y==0?GARBAGE_VALUE:x/y;}
	double diff(double x, double y)
	{return x-y;}
	bool pedestal_quality(double d) {return abs(d)<=0.5 ? true : false;}
	bool led_quality(double r) {return r>0.8&&r<1.2 ? true:false;}
	bool laser_quality(double r) {return r>0.8&&r<1.2 ? true:false;}

	/* virtual */ void ContainerXXX::initialize(hashfunctions::HashType ht,
		CompactUsageType ut, int debug)
	{
		_hashmap.initialize(ht);
		_usetype = ut;
		_logger.set("XXX", debug);
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
				_logger.debug(_hashmap.getName(did));
				CompactMap::iterator mit = _cmap.find(hash);
				if (mit!=_cmap.end())
					continue;

				_cmap.insert(
					std::make_pair(hash, Compact()));
			}
		}
		else if (_hashmap.isEHash())
		{
			std::vector<HcalElectronicsId> eids = 
				emap->allElectronicsIdPrecision();
			for (std::vector<HcalElectronicsId>::const_iterator it=
				eids.begin(); it!=eids.end(); ++it)
			{
				uint32_t hash = it->rawId();
				HcalElectronicsId eid = HcalElectronicsId(hash);
				_logger.debug(_hashmap.getName(eid));
				CompactMap::iterator mit = _cmap.find(hash);
				if (mit!=_cmap.end())
					continue;

				_cmap.insert(
					std::make_pair(hash, Compact()));
			}
		}
		else if (_hashmap.isTHash())
		{
			std::vector<HcalTrigTowerDetId> tids = emap->allTriggerId();
			for (std::vector<HcalTrigTowerDetId>::const_iterator it=
				tids.begin(); it!=tids.end(); ++it)
			{
				uint32_t hash = it->rawId();
				HcalTrigTowerDetId tid = HcalTrigTowerDetId(hash);
				_logger.debug(_hashmap.getName(tid));
				CompactMap::iterator mit = _cmap.find(hash);
				if (mit!=_cmap.end())
					continue;

				_cmap.insert(
					std::make_pair(hash, Compact()));
			}
		}
	}

	/* virtual */ void ContainerXXX::fill(HcalDetId const& did, double x)
	{
		if (_usetype!=fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x += x;
		c._x2 += x*x;
		c._n++;
	}

	/* virtual */ void ContainerXXX::fill(HcalElectronicsId const& did, 
		double x)
	{
		if (_usetype!=fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x += x;
		c._x2 += x*x;
		c._n++;
	}

	/* virtual */ void ContainerXXX::fill(HcalTrigTowerDetId const& did, 
		double x)
	{
		if (_usetype!=fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x += x;
		c._x2 += x*x;
		c._n++;
	}

	/* virtual */ void ContainerXXX::set(HcalDetId const& did, double x)
	{
		if (_usetype==fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x = x;
		c._x2 = x*x;
		c._n=1;
	}

	/* virtual */ void ContainerXXX::set(HcalElectronicsId const& did, 
		double x)
	{
		if (_usetype==fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x = x;
		c._x2 = x*x;
		c._n=1;
	}

	/* virtual */ void ContainerXXX::set(HcalTrigTowerDetId const& did, 
		double x)
	{
		if (_usetype==fHistogram)
			return;

		Compact &c = _cmap[_hashmap.getHash(did)];
		c._x = x;
		c._x2 = x*x;
		c._n=1;
	}

	/* virtual */ uint32_t ContainerXXX::getEntries(HcalDetId const& did)
	{
		return _cmap[_hashmap.getHash(did)]._n;
	}

	/* virtual */ uint32_t ContainerXXX::getEntries(HcalElectronicsId const& 
		did)
	{
		return _cmap[_hashmap.getHash(did)]._n;
	}

	/* virtual */ uint32_t ContainerXXX::getEntries(HcalTrigTowerDetId const& 
		did)
	{
		return _cmap[_hashmap.getHash(did)]._n;
	}

	/* virtual */ uint32_t ContainerXXX::size()
	{
		return (uint32_t)(_cmap.size());
	}

	/* virtual */ void ContainerXXX::dump(Container1D* c, bool q)
	{
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			Compact &x = p.second;
			uint32_t hash = p.first;
			if (x._n<=0)
				continue;

			double x1;
			double x2;
			if (_usetype==fHistogram)
			{
				x1 = x.mean();
				x2 = x.rms();
			}
			else
			{
				x1 = x._x;
				x2 = x._x2;
			}

			q ? c->fill(hash, x1) : 
				c->fill(hash, x2);
		}
	}

	/* virtual */ void ContainerXXX::dump(std::vector<Container1D*> const &vc, 
		bool q)
	{
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			Compact &x = p.second;
			if (x._n<=0)
				continue;

			double x1;
			double x2;
			uint32_t hash = p.first;
			if (_usetype==fHistogram)
			{
				x1 = x.mean();
				x2 = x.rms();
			}
			else
			{
				x1 = x._x;
				x2 = x._x2;
			}

			for (std::vector<Container1D*>::const_iterator it=vc.begin();
				it!=vc.end(); ++it)
			{
				q ? (*it)->fill(hash, x1) : 
					(*it)->fill(hash, x2);
			}
		}
	}

	/* virtual */ void ContainerXXX::print()
	{
		std::cout << "Container by " << _hashmap.getHashTypeName() << std::endl;
		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			if (_hashmap.isDHash())
				std::cout << HcalDetId(p.first) << p.second << std::endl;
			else if (_hashmap.isEHash())
				std::cout << HcalElectronicsId(p.first) << p.second 
					<< std::endl;
			else if (_hashmap.isTHash())
				std::cout << HcalTrigTowerDetId(p.first) << p.second 
					<< std::endl;
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
			Compact &x = p.second;
			uint32_t hash = p.first;

			double value=0;
			if (_hashmap.isDHash())
				value = cont->getBinContent(HcalDetId(hash));
			else if (_hashmap.isEHash())
				value = cont->getBinContent(HcalElectronicsId(hash));
			else if (_hashmap.isTHash())
				value = cont->getBinContent(HcalTrigTowerDetId(hash));

			x._x += value;
			x._x2 += value*value;
			x._n++;
		}	
	}

	/* virtual */ void ContainerXXX::load(Container1D* c1, Container1D* c2)
	{
		//	only for value use cases
		if (_usetype==fHistogram)
			return;

		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			Compact &x = p.second;
			uint32_t hash = p.first;

			double m=0; double rms=0;
			if (_hashmap.isDHash())
			{
				m = c1->getBinContent(HcalDetId(hash));
				rms = c2->getBinContent(HcalDetId(hash));
			}
			else if (_hashmap.isEHash())
			{
				m = c1->getBinContent(HcalElectronicsId(hash));
				rms = c2->getBinContent(HcalElectronicsId(hash));
			}
			else if (_hashmap.isTHash())
			{
				m = c1->getBinContent(HcalTrigTowerDetId(hash));
				rms = c2->getBinContent(HcalTrigTowerDetId(hash));
			}

			x._x = m;
			x._x2 = rms;
			x._n=1;
		}	
	}

	/* virtual */ void ContainerXXX::compare(ContainerXXX const& ctarget, 
		Container1D* cdump, Container1D* cnonpres, Container1D *cbad,
		comparison_function cfunc, quality_function qfunc, bool q)
	{
		//	cannot compare sets with different hashtypes
		if (_hashmap.getHashType()!=ctarget._hashmap.getHashType())
			return;

		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			Compact &x = p.second;
			uint32_t hash = p.first;
			CompactMap::const_iterator it = ctarget._cmap.find(hash);

			//	skip if reference doesn't have that channel...
			if (x._n<=0)
				continue;

			//	if this channel is absent or didn't have entries - fill
			if (it==ctarget._cmap.end() || it->second._n==0)
			{
				cnonpres->fill(hash);
				continue;
			}

			//	both channels exist and do have entries
			Compact y = it->second;
			std::pair<double, double> p1 = x.getValues(_usetype);
			std::pair<double, double> p2 = y.getValues(ctarget._usetype);

			//	get the comparison value
			double ccc = q?(*cfunc)(p2.first, p1.first):
				(*cfunc)(p2.second,p1.second);
			std::cout << "11111" << ccc << "  " << p2.first << "  " << p1.first 
				<< std::endl;

			//	check quality and fill the container for comparison
			if (!((*qfunc)(ccc)) || ccc==GARBAGE_VALUE)
				cbad->fill(hash);
			cdump->fill(hash, ccc);
		}
	}

	/* virtual */ void ContainerXXX::compare(ContainerXXX const& ctarget, 
		std::vector<Container1D*> const &vcdump, 
		Container1D* cnonpres, Container1D *cbad,
		comparison_function cfunc, quality_function qfunc, bool q)
	{
		//	cannot compare sets with different hashtypes
		if (_hashmap.getHashType()!=ctarget._hashmap.getHashType())
			return;

		BOOST_FOREACH(CompactMap::value_type &p, _cmap)
		{
			Compact &x = p.second;
			uint32_t hash = p.first;
			CompactMap::const_iterator it = ctarget._cmap.find(hash);

			//	skip if reference doesn't have that channel...
			if (x._n<=0)
				continue;

			//	if this channel is absent or didn't have entries - fill
			if (it==ctarget._cmap.end() || it->second._n==0)
			{
				cnonpres->fill(hash);
				continue;
			}

			//	both channels exist and do have entries
			Compact y = it->second;
			std::pair<double, double> p1 = x.getValues(_usetype);
			std::pair<double, double> p2 = y.getValues(ctarget._usetype);

			//	get the comparison value
			double ccc = q?(*cfunc)(p2.first, p1.first):
				(*cfunc)(p2.second,p1.second);

			//	check quality and fill the container for comparison
			if (!((*qfunc)(ccc)) || ccc==GARBAGE_VALUE)
				cbad->fill(hash);
			for (std::vector<Container1D*>::const_iterator it=vcdump.begin();
				it!=vcdump.end(); ++it)
				(*it)->fill(hash, ccc);
		}
	}
}


