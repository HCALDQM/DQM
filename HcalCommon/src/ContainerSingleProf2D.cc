#include "DQM/HcalCommon/interface/ContainerSingleProf2D.h"

namespace hcaldqm
{
	ContainerSingleProf2D::ContainerSingleProf2D()
	{
		_qx = NULL;
		_qy = NULL;
		_qz = NULL;
	}

	ContainerSingleProf1D::ContainerSingleProf1D(std::string const& folder,
		Quantity *qx, Quantity *qy, Quantity *qz):
		ContainerSingle1D(folder, 
			qz->name()+"vs"+qy->name()+"vs"+qx->name(), qx, qy);
	{}
	
	/* virtual */ void ContainerSingleProf1D::initialize(std::string const& 
		folder, std::string const& qname,
		Quantity *qx, Quantity *qy, Quantity *qz,
		int debug/*=0*/)
	{
		ContainerSingle1D::initialize(folder, qname, qx, qy, qz, debug);
	}

	/* virtual */ void ContainerSingleProf1D::book(DQMStore::IBooker& ib,
		std::string subsystem, std::string aux)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qname+aux);
		_me = ib.bookProfile2D(_qname, _qname,
			_qx->nbins(), _qx->min(), _qx->max(),
			_qy->nbins(), _qy->min(), _qy->max(),
			_qz->min(), _qz->max());
	}
}



