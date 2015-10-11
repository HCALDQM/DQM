
#include "DQM/HcalCommon/interface/ContainerProf2D.h"

namespace hcaldqm
{
	using namespace mapper, axis;

	/* virtual */ void ContainerProf2D::book(DQMStore::IBooker &ib)
	{
		unsigned int size = _mapper.getSize();
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = =_mapper.buildName(i);
			MonitorElement *me = ib.bookProfile2D(_name+"_"+hname,
				_title+" "+hname, _xaxis._nbins, _xaxis._min, _xaxis._max,
				_yaxis._nbins, _yaxis._min, _yaxis._max, 
				_zaxis._min, _zaxis._max);
			TObject *o = me->getRootObject();
			_xaxis.setAxisLog(o);
			_yaxis.setAxisLog(o);
			_zaxis.setAxisLog(o);
			me->SetAxisTitle(_xaxis._title, 1);
			me->SetAxisTitle(_yaxis._title, 2);
			me->SetAxisTitle(_zaxis._title, 3);
		}
	}
}










