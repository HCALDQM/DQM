
#include "DQM/HcalCommon/interface/ContainerSingle2D.h"

namespace hcaldqm
{
	using namespace axis;

	/* virtual */ void ContainerSingle2D::book(DQMStore::IBooker &ib,
		std::string subsystem)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder);
		_me = ib.book2D(_name, _name,
			_xaxis._nbins, _xaxis._min, _xaxis._max,
			_yaxis._nbins, _yaxis._min, _yaxis._max);
		TObject *o = _me->getRootObject();
		_xaxis.setAxisLog(o);
		_yaxis.setAxisLog(o);
		_zaxis.setAxisLog(o);
		_me->setAxisTitle(_xaxis._title, 1);
		_me->setAxisTitle(_yaxis._title, 2);
		_me->setAxisTitle(_zaxis._title, 3);
		for (unsigned int i=0; i<_xaxis._labels.size(); i++)
			_me->setBinLabel(i+1, _xaxis._labels[i], 1);
		for (unsigned int i=0; i<_yaxis._labels.size(); i++)
			_me->setBinLabel(i+1, _yaxis._labels[i], 2);
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
		_me->Fill(_xaxis.resolve(id), _yaxis.resolve(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_me->Fill(_xaxis.resolve(id), _yaxis.resolve(id), x);
		else if (xact==fCoordinate)
			_me->Fill(_xaxis.resolve(id), x);
		else if (yact==fCoordinate)
			_me->Fill(x, _yaxis.resolve(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& id, double x,
		double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_me->Fill(_xaxis.resolve(id), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_me->Fill(x, _xaxis.resolve(id), y);
		else if (xact!=fCoordinate && yact!=fCoordinate)
			_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id)
	{
		_me->Fill(_xaxis.resolve(id), _yaxis.resolve(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_me->Fill(_xaxis.resolve(id), _yaxis.resolve(id), x);
		else if (xact==fCoordinate)
			_me->Fill(_xaxis.resolve(id), x);
		else if (yact==fCoordinate)
			_me->Fill(x, _yaxis.resolve(id));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalElectronicsId const& id, 
		double x, double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_me->Fill(_xaxis.resolve(id), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_me->Fill(x, _xaxis.resolve(id), y);
		else if (xact!=fCoordinate && yact!=fCoordinate)
			_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did, 
		HcalElectronicsId const& eid)
	{
		_me->Fill(_xaxis.resolve(did), _yaxis.resolve(eid));
	}

	/* virtual */ void ContainerSingle2D::fill(HcalDetId const& did, 
		HcalElectronicsId const& eid, double x)
	{
		_me->Fill(_xaxis.resolve(did), _yaxis.resolve(eid), x);
	}
}


