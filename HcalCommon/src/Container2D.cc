
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/Utilities.h"

namespace hcaldqm
{
	using namespace hcaldqm::axis;
	using namespace hcaldqm::mapper;
	using namespace constants;

	Container2D::Container2D(std::string const& folder, std::string nametitle,
		mapper::MapperType mt, axis::Axis *xaxis, axis::Axis* yaxis,
		axis::Axis *zaxis):
		Container1D(folder, nametitle, mt, xaxis, yaxis), _zaxis(zaxis)
	{}

	/* virtual */ void Container2D::fill(HcalDetId const& did)
	{
		_mes[_mapper.index(did)]->Fill(_xaxis->get(did),
			_yaxis->get(did));
	}

	//	HcalDetId based
	/* virtual */ void Container2D::fill(HcalDetId const& did, int x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did),
				_yaxis->get(did), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis->get(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, double x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did),
				_yaxis->get(did), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis->get(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		int x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis->get(did), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		double x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis->get(did), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, y);
	}

	//	HcalElectronicsId based
	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid)
	{
		_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid),
			_yaxis->get(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, int x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid),
				_yaxis->get(eid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis->get(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, double x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid),
				_yaxis->get(eid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis->get(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, 
		int x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis->get(eid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, 
		double x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis->get(eid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid)
	{
		_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid), _yaxis->get(tid));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid, int x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid),
				_yaxis->get(tid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, _yaxis->get(tid));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid, double x)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid),
				_yaxis->get(tid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, _yaxis->get(tid));
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid, 
		int x, int y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, _yaxis->get(tid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid, 
		int x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, _yaxis->get(tid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalTrigTowerDetId const& tid, 
		double x, double y)
	{
		AxisQType xact = _xaxis->getType();
		AxisQType yact = _yaxis->getType();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(_xaxis->get(tid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, _yaxis->get(tid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(tid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::book(DQMStore::IBooker &ib, 
		std::string subsystem, std::string aux)
	{
		unsigned int size = _mapper.getSize();
		ib.setCurrentFolder(subsystem+"/"+_folder+aux);
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.book2D(_name+"_"+hname,
				_name+" "+hname, _xaxis->_nbins, _xaxis->_min, _xaxis->_max,
				_yaxis->_nbins, _yaxis->_min, _yaxis->_max);
			TObject *o = me->getRootObject();
			_xaxis->setLog(o);
			_yaxis->setLog(o);
			_zaxis->setLog(o);
			me->setAxisTitle(_xaxis->_title, 1);
			me->setAxisTitle(_yaxis->_title, 2);
			me->setAxisTitle(_zaxis->_title, 3);
			for (unsigned int i=0; i<_xaxis->_labels.size(); i++)
				me->setBinLabel(i+1, _xaxis->_labels[i], 1);
			for (unsigned int i=-1; i<_yaxis->_labels.size(); i++)
				me->setBinLabel(i+1, _yaxis->_labels[i], 2);
			_mes.push_back(me);
		}
	}
}


