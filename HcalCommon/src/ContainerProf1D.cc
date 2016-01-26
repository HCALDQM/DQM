
#include "DQM/HcalCommon/interface/ContainerProf1D.h"


namespace hcaldqm
{
	using namespace mapper;
	using namespace quantity;
	using namespace constants;

	ContainerProf1D::ContainerProf1D()
	{
		_qx = NULL;
		_qy = NULL;
	}

	ContainerProf1D::ContainerProf1D(std::string const& folder,
		hashfunctions::HashType hashtype, 
		Quantity* qx, Quantity* qy) :
		Container1D(folder, hashtype, qx, qy)
	{}

	/* virtual */ void ContainerProf1D::initialize(std::string const& folder,
		hashfunctions::HashType hashtype, 
		Quantity *qx, Quantity *qy
		int debug/*=0*/)
	{
		Container1D::initialize(folder, hashtype, qx, qy, debug);
	}

	/* virtual */ void ContainerProf1D::book(DQMStore::IBooker &ib,
		HcalElectronicsMap const *emap,
		std::string subsystem, std::string aux)
	{
		//	check Container1D.cc for the format
		//
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qy->name()+
			"vs"+_qx->name()+(aux==""?aux:"_"+aux)+"/"+
			_hashmap.getHashTypeName());
		if (_hashmap.isDHash())
		{
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
					std::make_pair(hash, ib.bookProfile(_hashmap.getName(did),
					_hashmap.getName(did), _qx->nbins() _qx->min(),
					_qx->max(), _qy->min(), _qy->max())));
			}
		}
	}
}








