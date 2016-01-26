
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"
#include "DQM/HcalCommon/interface/Utilities.h"

namespace hcaldqm
{
	using namespace axis;

	ContainerSingle2D::ContainerSingle2D()
	{
		_qx = NULL;
		_qy = NULL;
		_qz = NULL;
	}

	ContainerSingle2D::ContainerSingle2D(std::string const& folder,
		Quantity *qx, Quantity *qy, Quantity *qz):
		Container(folder, qz->name()+"vs"+qy->name()+"vs"+qx->name()),
		_qx(qx), _qy(qy), _qz(qz)
	{}

	/* virtual */ void ContainerSingle2D::initialize(std::string const& folder,
		Quantity *qx, Quantity *qy, Quantity *qz, int debug/*=0*/)
	{
		Container::initialize(folder, qz->name()+"vs"+qy->name()+"vs"+
			qx->name(), debug);
		_qx = qx;
		_qy = qy;
		_qz = qz;
	}

	/* virtual */ void ContainerSingle2D::initialize(std::string const& folder,
		std::string const& qname,
		Quantity *qx, Quantity *qy, Quantity *qz, int debug/*=0*/)
	{
		Container::initialize(folder, qname, debug);
		_qx = qx;
		_qy = qy;
		_qz = qz;
	}
	
	/* virtual */ void ContainerSingle2D::book(DQMStore::IBooker &ib,
		 std::string subsystem, std::string aux)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qname+aux);
		_me = ib.book2D(_qname, _qname,
			_qx->nbins(), _qx->min(), _qx->max(),
			_qy->nbins(), _qy->min(), _qy->max());
	}

	/* virtual */ void ContainerSingle2D::fill(int x, int y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle2D::fill(int x, double y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle2D::fill(int x, double y, double z)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y), z);
	}

	/* virtual */ void ContainerSingle2D::fill(double x, int y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle2D::fill(double x, double y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle2D::fill(double x, double y, double z)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y), z);
	}

	/* virtual */ void ContainerSingle2D::fill(int x, int y, double z)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y), z);
	}

	/* virtual */ void ContainerSingle2D::fill(int x, int y, int z)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y), z);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id)
	{
		_me->Fill(_qx->getValue(id), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x,
		double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, int x,
		int y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, int x,
		double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	//	by ElectronicsId
	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id)
	{
		_me->Fill(_qx->getValue(id), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x,
		double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		int x,
		int y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		int x,
		double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	//	by TrigTowerDetId
	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id)
	{
		_me->Fill(_qx->getValue(id), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id, 
		double x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id, 
		int x)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else if (_qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id, 
		double x, double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id, 
		int x, int y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalTrigTowerDetId const& id, 
		int x, double y)
	{
		if (_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(id), x);
		else if (_qx->isCoordinate() && !_qy->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else if (!_qx->isCoordinate() && _qy->isCoordinate())
			_me->Fill(_qx->getValue(x), _qy->getValue(id), y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did,
		HcalElectronicsId const& eid)
	{
		if (_qx->type()==DetectorQuantity)
			_me->Fill(_qx->getValue(did), _qy->getValue(eid));
		else
			_me->Fill(_qx->getValue(eid), _qy->getValue(did));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did,
		HcalElectronicsId const& eid, double x)
	{
		if (_qx->type()==DetectorQuantity)
			_me->Fill(_qx->getValue(did), _qy->getValue(eid), x);
		else
			_me->Fill(_qx->getValue(eid), _qy->getValue(did), x);
	}
}

