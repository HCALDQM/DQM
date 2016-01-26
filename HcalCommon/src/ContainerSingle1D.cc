
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"

namespace hcaldqm
{
	using namespace quantity;
	ContainerSingle1D::ContainerSingle1D()
	{
		_qx = NULL;
		_qy = NULL;
	}

	ContainerSingle1D::ContainerSingle1D(std::string const& folder,
		Quantity *qx, Quantity *qy):
		Container(folder, qy->name()+"vs"+qx->name()), _qx(qx), _qy(qy)
	{}

	/* virtual */ void ContainerSingle1D::initialize(std::string const& folder,
		Quantity *qx, Quantity *qy, int debug/*=0*/)
	{
		Container::initialize(folder, qy->name()+"vs"+qx->name(), debug);
		_qx = qx;
		_qy = qy;
	}

	/* virtual */ void ContainerSingle1D::initialize(std::string const& folder,
		std::string const& qname,
		Quantity *qx, Quantity *qy, int debug/*=0*/)
	{
		Container::initialize(folder, qname, debug);
		_qx = qx;
		_qy = qy;
	}

	/* virtual */ void ContainerSingle1D::book(DQMStore::IBooker &ib,
		 std::string subsystem, std::string aux)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder+"/"+_qname+aux);
		_me = ib.book1D(_qname, _qname,
			_qx->nbins(), _qx->min(), _qx->max());
	}

	/* virtual */ void ContainerSingle1D::fill(int x)
	{
		_me->Fill(_qx->getValue(x));
	}

	/* virtual */ void ContainerSingle1D::fill(double x)
	{
		_me->Fill(_qx->getValue(x));
	}

	/* virtual */ void ContainerSingle1D::fill(int x, int y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(int x, double y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(double x, int y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(double x, double y)
	{
		_me->Fill(_qx->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id)
	{
		_me->Fill(_qx->getValue(id));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id, double x)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else
			_me->Fill(_qx->getValue(x));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id, double x,
		double y)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else
			_me->Fill(_qy->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalElectronicsId const& id)
	{
		_me->Fill(_qx->getValue(id));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalElectronicsId const& id, 
		double x)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else
			_me->Fill(_qx->getValue(x));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalElectronicsId const& id, 
		double x, double y)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else
			_me->Fill(_qy->getValue(x), _qy->getValue(y));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalTrigTowerDetId const& id)
	{
		_me->Fill(_qx->getValue(id));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalTrigTowerDetId const& id, 
		double x)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x));
		else
			_me->Fill(_qx->getValue(x));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalTrigTowerDetId const& id, 
		double x,
		double y)
	{
		if (_qx->isCoordinate())
			_me->Fill(_qx->getValue(id), _qy->getValue(x), y);
		else
			_me->Fill(_qy->getValue(x), _qy->getValue(y));
	}
}


