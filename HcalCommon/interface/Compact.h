#ifndef Compact_h
#define Compact_h

/**
 *	file:		Compact.h
 *	Auhtor:		Viktor Khristenko
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Constants.h"

namespace hcaldqm
{
	namespace compact
	{
		enum CompactUsageType
		{
			fHistogram = 0,
			f1 = 1, // just load 1 value from 1 hist
			fMeanRMS1 = 2, // load mean/rms from 1 histo - not being used
			fMeanRMS2 = 3, // load mean rms from 2 histos - 1 for each
			nCompactUsageType = 4
		};

		struct Compact 
		{
			Compact(){_x=0; _x2=0; _n=0;}
			void reset() {_x=0; _x2=0; _n=0;}
			
			double mean() {return _n>0?_x/_n:constants::GARBAGE_VALUE;}
			double rms() 
			{
				if (mean()==constants::GARBAGE_VALUE)
					return constants::GARBAGE_VALUE;

				double m = this->mean();
				return sqrt(_x2/_n-m*m);
			}
			std::pair<double, double> getValues(CompactUsageType utype)
			{
				std::pair<double, double> p;
				if (utype==fHistogram)
					p = std::make_pair<double, double>(mean(), rms());
				else
					p = std::make_pair<double, double>((double)_x, (double)_x2);

				return p;
			}

			double		_x;
			double		_x2;
			uint32_t	_n;
		};
		std::ostream& operator<<(std::ostream&, Compact const&);
	}
}

#endif
