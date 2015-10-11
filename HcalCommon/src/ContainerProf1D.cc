
#include "DQM/HcalCommon/interface/ContainerProf1D.h"


namespace hcaldqm
{
	/* virtual */ void ContainerProf1D::book(DQMStore::IBooker &ib)
	{
		unsigned int size = _mapper.getSize();
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.bookProfile(_name+"_"+hname, 
				_title+" "+hname, _xaxis._nbins, _xaxis._min, _xaxis.max,
				_yaxis._min, _yaxis._max);
			TObject *o = me->getRootObject();
			_xaxis.setAxisLog(o);
			_yaxis.setAxisLog(o);
			me->SetAxisTitle(_xaxis._title, 1);
			me->SetAxisTitle(_yaxis._title, 2);
			_mes.push_back(me);
		}
	}
}








