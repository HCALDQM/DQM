
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/Utilities.h"

namespace hcaldqm
{
	using namespace constants;
	using namespace quantity;
	using namespace mapper;

	Container2D::Container2D()
	{
		_qx = NULL;
		_qy = NULL;
		_qz = NULL;
	}

	Container2D::Container2D(std::string const& folder,
		hashfunctions::HashType hashtype, Quantity *qx, Quantity *qy,
		Quantity *qz) :
		Container1D(folder, hashtype, qx, qy), _qz(qz)
	{}
	
	/* virtual */ void Container2D::initialize(std::string const& folder, 
		hashfunctions::HashType hashtype,
		Quantity *qx, Quantity *qy, Quantity *qz,
		int debug/*=0*/)
	{
		Container1D::initialize(folder, hashtype, qx, qy, debug);
		_qz = qz;
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did)
	{
		_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
			_qy->getValue(did));
	}

	//	HcalDetId based
	/* virtual */ void Container2D::fill(HcalDetId const& did, int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		int x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		int x, int y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		double x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	//	by ElectronicsId	
	/* virtual */ void Container2D::fill(HcalElectronicsId const& did)
	{
		_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
			_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& did, int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalEletronicsId const& did, double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& did, 
		int x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& did, 
		int x, int y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& did, 
		double x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	//	by TrigTowerDetId
	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did)
	{
		_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
			_qy->getValue(did));
	}

	//	HcalDetId based
	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did, int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did, 
		double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(did), x);
		else if (_qx->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did),
				_qy->getValue(x));
		else if (_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did, 
		int x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did, 
		int x, int y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& did, 
		double x, double y)
	{
		if (_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(did), 
				_qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(did), y);
		else if (!_qx->isCoordinate() && !_qy->isCoordinate())
			_mes[_hashmap.getHash(did)]->Fill(_qx->getValue(x), 
				_qy->getValue(y));
	}
	
	/* virtual */ void Container2D::book(DQMStore::IBooker &ib, 
		HcalElectronicsMap const *emap,
		std::string subsystem, std::string aux)
	{

		//	full path as in Container1D.cc
		//
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qz->name()+"vs"+
			_qy->name()+"vs"+_qx->name()+(aux==""?aux:"_"+aux)+
			"/"_hashmap.getHashTypeName());
		if (_hashmap.isDHash())
		{
			//	for Detector Hashes
			std::vector<HcalGenericDetId> dids = emap->allPrecisionId();
			for (std::vector<HcalGenericDetId>::const_iterator it=
				dids.begin(); it!=dids.end(); ++it)
			{
				//	skip trigger towers and calib
				if (!it->isHcalDetId())
					continue;

				HcalDetId did = HcalDetId(it->rawId());
				uint32_t hash = _hashmap.getHash(did);
				_mes.insert(
					std::make_pair(hash, ib.book2D(_hashmap.getName(did),
					_hashmap.getName(did), _qx->nbins(), _qx->min(),
					_qx->max(), _qy->nbins(), _qy->min(), _qy->max())));
			}
		}
	}
}


