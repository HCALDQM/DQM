
#include "DQM/HcalCommon/interface/Container1D.h"

namespace hcaldqm
{
	using namespace mapper;
	using namespace axis;
	using namespace constants;

	Container1D::Container1D(std::string const& folder, 
		std::string const& nametitle, mapper::MapperType mt, axis::Axis *xaxis,
		axis::Axis *yaxis):
		Container(folder, nametitle), _mapper(mt), _xaxis(xaxis), _yaxis(yaxis)
	{}

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
		_mes[_mapper.index(did)]->Fill(_xaxis->get(did));
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, int x, double y)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x, y);
	}
	/* virtual */ void Container1D::fill(HcalDetId const& did, double x , 
			double y)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(did)]->Fill(x, y );
		else if (act==fCoordinate)
			_mes[_mapper.index(did)]->Fill(_xaxis->get(did), x, y);
	}

	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid)
	{
		_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid));
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, int x)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, double x)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, 
		int x, double y)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x, y);
	}
	/* virtual */ void Container1D::fill(HcalElectronicsId const& eid, 
		double x, double y)
	{
		AxisQType act = _xaxis->getType();
		if (act==fValue || act==fFlag)
			_mes[_mapper.index(eid)]->Fill(x, y);
		else if (act==fCoordinate)
			_mes[_mapper.index(eid)]->Fill(_xaxis->get(eid), x, y);
	}

	/* virtual */ void Container1D::book(DQMStore::IBooker& ib, 
		std::string subsystem, std::string aux)
	{

		unsigned int size = _mapper.getSize();
		ib.setCurrentFolder(subsystem+"/"+_folder+aux);
		for (unsigned int i=0; i<size; i++)
		{
//			utilities::log(_name);			
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.book1D(_name+"_"+hname, _name +" "+hname,
				_xaxis->_nbins, _xaxis->_min, _xaxis->_max);
			TObject *o = me->getRootObject();
			_xaxis->setLog(o);
			_yaxis->setLog(o);
			me->setAxisTitle(_xaxis->_title, 1);
			me->setAxisTitle(_yaxis->_title, 2);
			for (unsigned int i=0; i<_xaxis->_labels.size(); i++)
				me->setBinLabel(i+1, _xaxis->_labels[i]);
			_mes.push_back(me);
		}
	}

	/* virtual */ void Container1D::reset()
	{
		for (MEVector::const_iterator it=_mes.begin(); it!=_mes.end(); ++it)
			(*it)->Reset();
	}
}







