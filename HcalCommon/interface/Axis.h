#ifndef Axis_h
#define Axis_h

/*
 *	file:			Axis.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *
 *
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

#include <string>

namespace hcaldqm
{
	namespace axis
	{
		enum AxisType
		{
			fXaxis = 0,
			fYaxis = 1.
			fZaxis = 2,
			nAxisType = 3
		};
	
		enum AxisClassType
		{
			fValue = 0,
			fCoordinate = 1,
			fFlag = 2,
			nAxisClassType = 3
		};
	
		class Axis
		{
			public:
				Axis();
				Axis(AxisType);
				Axis(AxisType, std::string const&);
				Axis(AxisType, std::string const&, 
					int, double, double, bool log=false);
				virtual ~Axis() {}

				//	
				virtual void customize(TAxis* ax, TObject* ob)
				{
					this->setAxisLog(ob);
					this->setAxisTitle(ax);
					this->setAxisBins(ax);
				}
	
				virtual int resolve(HcalDetId const&) {}
				virtual int resolve(HcalElectronicsId const&) {}
				virtual int resolve(int) {}
				virtual int resolve(double) {}
	
				//	
				void setAxisLog(TObject*);
				void setAxisTitle(TAxis*);
				void setAxisBins(TAxis*);
	
			protected:
				AxisType			_type;
				std::string			_title;
				int					_nbins;
				double				_min;
				double				_max;
				bool				_log;
		};
	}
}

#endif














