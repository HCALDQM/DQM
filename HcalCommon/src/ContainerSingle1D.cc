
#include "DQM/HcalCommon/interface/ContainerSingle1D.h"

namespace hcaldqm
{
	using namespace axis;

	ContainerSingle1D::ContainerSingle1D(std::string const& folder,
		std::string const& nametitle,
		Axis *xaxis, Axis *yaxis):
		Container(folder, nametitle),
		_xaxis(xaxis), _yaxis(yaxis)
	{}

	/* virtual */ void ContainerSingle1D::book(DQMStore::IBooker &ib,
		 std::string subsystem, std::string aux)
	{
		ib.setCurrentFolder(subsystem+"/"+_folder+aux);
		_me = ib.book1D(_name, _name,
			_xaxis->_nbins, _xaxis->_min, _xaxis->_max);
		TObject *o = _me->getRootObject();
		_xaxis->setLog(o);
		_yaxis->setLog(o);
		_me->setAxisTitle(_xaxis->_title, 1);
		_me->setAxisTitle(_yaxis->_title, 2);
		for (unsigned int i=0; i<_xaxis->_labels.size(); i++)
			_me->setBinLabel(i+1, _xaxis->_labels[i], 1);
	}

	/* virtual */ void ContainerSingle1D::fill(int x)
	{
		_me->Fill(x);
	}

	/* virtual */ void ContainerSingle1D::fill(double x)
	{
		_me->Fill(double);
	}

	/* virtual */ void ContainerSingle1D::fill(int x, int y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle1D::fill(int x, double y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle1D::fill(double x, int y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle1D::fill(double x, double y)
	{
		_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id)
	{
		_me->Fill(_xaxis->get(id));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id, double x)
	{
		AxisQType xact = _xaxis->getType();
		if (xact==fCoordinate)
			_me->Fill(_xaxis->get(id), x);
		else
			_me->Fill(x);
	}

	/* virtual */ void ContainerSingle1D::fill(HcalDetId const& id, double x,
		double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate)
			_me->Fill(_xaxis->get(id), x, y);
		else
			_me->Fill(x, y);
	}

	/* virtual */ void ContainerSingle1D::fill(HcalElectronicsId const& id)
	{
		_me->Fill(_xaxis->get(id));
	}

	/* virtual */ void ContainerSingle1D::fill(HcalElectronicsId const& id, 
		double x)
	{
		AxisQType xact = _xaxis->getType();
		if (xact==fCoordinate)
			_me->Fill(_xaxis->get(id), x);
		else
			_me->Fill(x);
	}
}


