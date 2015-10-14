
#include "DQM/HcalCommon/interface/ContainerProf1D.h"


namespace hcaldqm
{
	using namespace mapper;
	using namespace axis;
	using namespace constants;
	/* virtual */ void ContainerProf1D::book(DQMStore::IBooker &ib)
	{
		unsigned int size = _mapper.getSize();
		 ib.setCurrentFolder(SUBSYSTEM+"/"+_folder);
		for (unsigned int i=0; i<size; i++)
		{
			std::string hname = _mapper.buildName(i);
			MonitorElement *me = ib.bookProfile(_name+"_"+hname, 
				_title+" "+hname, _xaxis._nbins, _xaxis._min, _xaxis._max,
				_yaxis._min, _yaxis._max);
			TObject *o = me->getRootObject();
			_xaxis.setAxisLog(o);
			_yaxis.setAxisLog(o);
			me->setAxisTitle(_xaxis._title, 1);
			me->setAxisTitle(_yaxis._title, 2);
			_mes.push_back(me);
		}
	}
}








