
#include "DQM/HcalCommon/interface/Container1D.h"

namespace hcaldqm
{
	using namespace mapper;
	using namespace constants;

	Container1D::Container1D()
	{}

	Container1D::Container1D(std::string const& folder,
		hashfunctions::HashType hashtype, Quantity *qx, Quantity *qy) :
		Container(folder, qy->name()+"vs"+qx->name()), _hashmap(hashtype),
		_qx(qx), _qy(qy)
	{}

	/* virtuial */ void Container1D::initialize(std::string const& folder, 
		hashfunctions::HashType hashtype, Quantity *qx, Quantity *qy/* = ... */,
		int debug /* =0 */)
	{
		Container::initialize(folder, qy->name()+"vs"+qx->name(), debug);
		_hashmap.initialize(hashtype);
		_qx = qx;
		_qy = qy;
	}

	/* virtual */ void Container1D::reset()
	{
		BOOST_FOREACH(MEMap::value_type &pair, _mes)
		{
			_mes[pair.first]->Reset();
		}
	}

	/* virtual */ void Container1D::fill(HcalDetId const& did)
	{
		_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did));
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x)
	{
		QuantityType qtype = _qx->type();
		if (qtype==fValue || qtype==fFlag)
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x));
		else 
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x));
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x)
	{
		QuantityType qtype = _qx->type();
		if (qtype==fValue || qtype==fFlag)
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x));
		else 
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
					_qy->getValue(x));
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x, double y)
	{
		QuantityType qtype = _qx->type();
		if (qtype==fValue || qtype==fFlag)
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
			_qy->getValue(y));
		else 
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x, int y)
	{
		QuantityType qtype = _qx->type();
		if (qtype==fValue || qtype==fFlag)
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
			_qy->getValue(y));
		else 
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x , 
			double y)
	{
		QuantityType qtype = _qx->type();
		if (qtype==fValue || qtype==fFlag)
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
			_qy->getValue(y));
		else 
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
	}

	/* virtual */ void Container1D::book(DQMStore::IBooker& ib, 
		HcalElectronicsMap const *emap,
		std::string subsystem, std::string aux)
	{

		//	full path to where all the plots are living
		//	subsystem/taskname/QxvsQy/HashType
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qy->name()+
			"vs"+_qx->name()+"/"+_hashmap.getHashTypeName());
		if (_hashmap.isDHash())
		{
			//	for Detector Hashes
			std::vector<HcalGenericDetId> dids = emap->allPrecisionId();
			for (std::vector<HcalGenericDetId>::const_iterator it=
				dids.begin(); it!=dids.end(); ++it)
			{
				//	skip trigger towers and calibration
				if (!it->isHcalDetId())
					continue;

				HcalDetId did = HcalDetId(it->rawId());
				uint32_t hash = _hashmap.getHash(did);
				_mes.insert(
					std::make_pair(hash, ib.book1D(_hashmap.getName(did),
					_hashmap.getName(did), _qx->nbins(), _qx->min(), 
					_qx->max())));

			}
		}
		/*
		else if (_hashmap.isEHash())
		{
			//	for Electronics Hashes
			std::vector<HcalElectronicsId> eids = 
				emap.allElectronicsIdPrecision();
			for (std::vector<HcalElectronicsId>::const_iterator it=
				eids.begin(); it!=eids.end(); ++it)
			{
				HcalElectronicsId eid = HcalElectronicsId(it->rawId());
				uint32_t hash = _hashmap.getHash(eid);
				std::pair<MEMap::iterator, bool> r = _mes.insert(hash,
					ib.book1D(_hashmap.getName(eid),
					_hashmap.getName(eid), qx->nbins(), qx->min(), qx->max()));
			}
		}
		else if (_hashmap.isTHash())
		{
			//	for TrigTower Hashes
			std::vector<HcalTrigTowerDetId> tids = 
				emap.allTriggerId();
			for (std::vector<HcalTrigTowerDetId>::const_iterator it=
				tids.begin(); it!=tids.end(); ++it)
			{
				HcalTrigTowerDetId tid = HcalTrigTowerDetId(it->rawId());
				uint32_t hash = _hashmap.getHash(tid);
				std::pair<MEMap::iterator, bool> r = mes.insert(hash,
					ib.book1D(_hashmap.getName(tid), _hashmap.getName(tid),
					qx->nbins(), qx->min(), qx->max()));
			}
		}
		*/
	}
}







