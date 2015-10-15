
#include "DQM/HcalCommon/interface/Container1D.h"

namespace hcaldqm
{
	using namespace mapper;
	using namespace axis;
	using namespace constants;
	/* virtual */ void Container1D::fill(int id, int x)
	{
		_mes[_mapper.index(id)]->Fill(x);
	}
	/* virtual */ void Container1D::fill(int id, double x)
	{
		_mes[_mapper.index(id)]->Fill(x);
	}
	/* virtual */ void Container1D::fill(int id, int x, double y)
	{
		_mes[_mapper.index(id)]->Fill(x, y);
	}
	/* virtual */ void Container1D::fill(int id, double x, double y)
	{
		_mes[_mapper.index(id)]->Fill(x, y);
	}

	/* virtual */ void Container1D::fill(HcalDetId const& did)
	{
		_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did));
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x, double y)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x, y);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x , 
			double y)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x, y );
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis.resolve(did), x, y);
	}

	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid)
	{
		_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid));
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, int x)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, double x)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
		{
			_mes[_mapper.index(eid)]->Fill(x);
		}
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, 
		int x, double y)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x, y);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, 
		double x, double y)
	{
		AxisQ act = _xaxis.getAxisQ();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis.resolve(eid), x, y);
	}

	/* virtual */ void Container1D::book(DQMStore::IBooker& ib)
	{
		unsigned int size = _mapper.getSize();
		ib.setCurrentFolder(SUBSYSTEM+"/"+_folder);
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.book1D(_name+"_"+hname, _title+" "+hname,
				_xaxis._nbins, _xaxis._min, _xaxis._max);
			TObject *o = me->getRootObject();
			_xaxis.setAxisLog(o);
			_yaxis.setAxisLog(o);
			me->setAxisTitle(_xaxis._title, 1);
			me->setAxisTitle(_yaxis._title, 2);
			for (int i=0; i<_xaxis._labels.size(); i++)
				me->SetBinLabel(i+1, _xaixs._labels[i]);
			_mes.push_back(me);
		}
	}
}







