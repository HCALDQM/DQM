#ifndef Axis_h
#define Axis_h

/*
 *	file:			Axis.h
 *	Author			Viktor Khristenko
 *
 *	Description:
 *		Base Axis Class
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

#include <string>
#include <vector>

namespace hcaldqm
{
	namespace axis
	{
		enum AxisType
		{
			fXaxis = 0,
			fYaxis = 1,
			fZaxis = 2,
			nAxisType = 3
		};
		enum AxisQType
		{
			fValue = 0,
			fCoordinate = 1,
			fFlag = 2,
			nAxisQType = 3
		};

		/*
		 *	Base Class for Axis
		 */
		class Container;
		class Axis
		{
			friend class Container;

			public:
				Axis():
					_type(fXaxis), _qtype(fValue), _log(false)
				{
					this->_setup();
				}
				Axis(std::string title,AxisType type, AxisQType qtype, 
					int n, double min, double max, bool log) : 
					_nbins(n), _min(min), _max(max), _title(title)
					_type(type), _qtype(qtype), _log(log),
				{
					this->_setup();
				}
				virtual ~Axis() {}

				virtual int get(HcalDetId const&) {return 0;}
				virtual int get(HcalElectronicsId const&) {return 0;}
				virtual int get(int) {return 0;}
			protected:
				virtual void setup() {}
				
			public:
				AxisQType getType() {return _qtype;}
				void setLog(TObject* o)
				{
					if (_log)
						o->SetBit(BIT(FIRST_BIT_TO_USE+_type));
				}
			protected:

				//	some member variables should be public
				int _nbins;
				double _min;
				double _max;
				std::string _title;
				std::vector<std::string> _labels;

				AxisType	_type;
				AxisQType	_qtype;
				bool		_log;
		}
	}
}

#endif
