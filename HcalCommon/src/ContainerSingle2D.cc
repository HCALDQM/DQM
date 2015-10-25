
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"

namespace hcaldqm
{
	using namespace axis;

	ContainerSingle2D::ContainerSingle2D(std::string const& folder,
		std::string const& nametitle,
		Axis *xaxis, Axis *yaxis, Axis *zaxis):
		Container(folder, nametitle),
		_xaxis(xaxis), _yaxis(yaxis), _zaxis(zaxis)
	{}

	/* virtual */ void ContainerSingle2D::book(DQMStore::IBooker &ib,
		std::string subsystem)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder);
		_me = ib.book2D(_name, _name,
			_xaxis->_nbins, _xaxis->_min, _xaxis->_max,
			_yaxis->_nbins, _yaxis->_min, _yaxis->_max);
		TObject *o = _me->getRootObject();
		_xaxis->setLog(o);
		_yaxis->setLog(o);
		_zaxis->setLog(o);
		_me->setAxisTitle(_xaxis->_title, 1);
		_me->setAxisTitle(_yaxis->_title, 2);
		_me->setAxisTitle(_zaxis->_title, 3);
		for (unsigned int i=0; i<_xaxis->_labels.size(); i++)
			_me->setBinLabel(i+1, _xaxis->_labels[i], 1);
		for (unsigned int i=0; i<_yaxis->_labels.size(); i++)
			_me->setBinLabel(i+1, _yaxis->_labels[i], 2);
	}

	/* virtual */ void ContainerSingle2D::fill(int x, int y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(int x, double y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(int x, double y, double z)
	{
		_me->Fill(x, y, z);
	}

	/* virtual */ void ContainerSingle2D::fill(double x, int y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(double x, double y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(double x, double y, double z)
	{
		_me->Fill(x, y, z);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id)
	{
		_me->Fill(_xaxis->get(id), _yaxis->get(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_me->Fill(_xaxis->get(id), _yaxis->get(id), x);
		else if (xact==fCoordinate)
			_me->Fill(_xaxis->get(id), x);
		else if (yact==fCoordinate)
			_me->Fill(x, _yaxis->get(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x,
		double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_me->Fill(_xaxis->get(id), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_me->Fill(x, _yaxis->get(id), y);
		else if (xact!=fCoordinate && yact!=fCoordinate)
			_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id)
	{
		_me->Fill(_xaxis->get(id), _yaxis->get(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_me->Fill(_xaxis->get(id), _yaxis->get(id), x);
		else if (xact==fCoordinate)
			_me->Fill(_xaxis->get(id), x);
		else if (yact==fCoordinate)
			_me->Fill(x, _yaxis->get(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_me->Fill(_xaxis->get(id), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_me->Fill(x, _yaxis->get(id), y);
		else if (xact!=fCoordinate && yact!=fCoordinate)
			_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did, 
		HcalElectronicsId const& eid)
	{
		_me->Fill(_xaxis->get(did), _yaxis->get(eid));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did, 
		HcalElectronicsId const& eid, double x)
	{
		_me->Fill(_xaxis->get(did), _yaxis->get(eid), x);
	}
}


