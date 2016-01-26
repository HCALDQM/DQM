
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

namespace hcaldqm
{
	using namespace mapper;
	using namespace quantity;
	using namespace constants;

	ContainerProf2D::ContainerProf2D()
	{
		_qx = NULL;
		_qy = NULL;
		_qz = NULL;
	}

	ContainerProf2D::ContainerProf2D(std::string const& folder,
		hashfunctions::HashType hashtype, Quantity *qx, Quantity *qy,
		Quantity *qz) :
		Container2D(folder, hashtype, qx, qy, qz)
	{}
	
	/* virtual */ void ContainerProf2D::initialize(std::string const& folder,
		hashfunctions::HashType hashtype, Quantity *qx, Quantity *qy,
		Quantity *qz,
		int debug/*=0*/)
	{
		Container2D::initialize(folder, hashtype, qx, qy, qz, 
			debug);
	}

	/* virtual */ void ContainerProf2D::book(DQMStore::IBooker &ib,
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
				//	skip trigger towers and calibration
				if (!it->isHcalDetId())
					continue;

				HcalDetId did = HcalDetId(it->rawId());
				uint32_t hash = _hashmap.getHash(did);
				_mes.insert(
					std::make_pair(hash, ib.bookProfile2D(
					_hashmap.getName(did), _hashmap.getName(did),
					_qx->nbins(), _qx->min(), _qx->max(),
					_qy->nbins(), _qy->min(), _qy->max(),
					_qz->min(), _qz->max())));
			}
		}
	}
}










