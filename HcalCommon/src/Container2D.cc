
#include "DQM/HcalCommon/interface/Container2D.h"

namespace hcaldqm
{
	using namespace hcaldqm::axis;
	using namespace hcaldqm::mapper;

	/* virtual */ void Container2D::fill(HcalDetId const& did)
	{
		_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did),
			_yaxis.resolve(did));
	}

	//	HcalDetId based
	/* virtual */ void Container2D::fill(HcalDetId const& did, int x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did),
				_yaxis.resolve(did), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis.resolve(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, double x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did),
				_yaxis.resolve(did), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _yaxis.resolve(did));
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		int x, double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _xaxis.resolve(did), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalDetId const& did, 
		double x, double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, _xaxis.resolve(did), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(did)]->Fill(x, y);
	}

	//	HcalElectronicsId based
	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid)
	{
		_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid),
			_yaxis.resolve(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, int x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid),
				_yaxis.resolve(eid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis.resolve(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, double x)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid),
				_yaxis.resolve(eid), x);
		else if (xact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid),
				x);
		else if (yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _yaxis.resolve(eid));
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, 
		int x, double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _xaxis.resolve(eid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::fill(HcalElectronicsId const& eid, 
		double x, double y)
	{
		AxisQ xact = _xaxis.getAxisQ();
		AxisQ yact = _yaxis.getAxisQ();
		if (xact==fCoordinate && yact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x, y);
		else if (xact!=fCoordinate && yact==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, _xaxis.resolve(eid), y);
		else if (yact!=fCoordinate && xact!=fCoordinate)
			_mes[_mapper.index(eid)]->Fill(x, y);
	}

	/* virtual */ void Container2D::book(DQMStore::IBooker &ib)
	{
		unsigned int size = _mapper.getSize();
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.book2D(_name+"_"+hname,
				_title+" "+hname, _xaxis._nbins, _xaxis._min, _xaxis._max,
				_yaxis._nbins, _yaxis._min, _yaxis._max);
			TObject *o = me->getRootObject();
			_xaxis.setAxisLog(o);
			_yaxis.setAxisLog(o);
			_zaxis.setAxisLog(o);
			me->setAxisTitle(_xaxis._title, 1);
			me->setAxisTitle(_yaxis._title, 2);
			me->setAxisTitle(_zaxis._title, 3);
			_mes.push_back(me);
		}
	}
}


