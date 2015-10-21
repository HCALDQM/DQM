#ifndef Utilities_h
#define Utilities_h

/*
 *	file:			Utilities.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Utility functions
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

namespace hcaldqm
{
	namespace utilities
	{
		/*
		 *	Some useful functions on digis
		 */
		template<typename DIGI>
		int maxTS(DIGI digi, double ped=0)
		{
			int maxTS = -1;
			double maxQ = -100;
			for (int i=0; i<digi.size(); i++)
				if((digi.sample(i).nominal_fC()-ped)>maxQ)
				{
					maxQ = digi.sample(i).nominal_fC()-ped;
					maxTS = i;
				}
			return maxTS;
		}

		template<typename DIGI>
		double aveTS(DIGI digi, double ped=0, int i=0, int j=digi.size()-1)
		{
			double sumQ = 0;
			double sumQT = 0;
			for (int ii=i; ii<=j; ii++)
			{
				sumQ+=digi.sample(ii).nominal_fC-ped;
				sumQT +=(ii+1)(digi.sample(ii).nominal_fC()-ped);
			}
			
			return sumQT/sumQ-1;
		}

		template<typename DIGI>
		double sumQ(DIGI digi, double ped, int i=0, int j=digi.size()-1)
		{
			double sum=0;
			for (int ii=i; ii<=j; ii++)
				sum+=(digi.sample(ii).nominal_fC()-ped);
			return sum;
		}

		template<typename DIGI>
		double aveQ(DIGI digi, double ped, int i=0, int j=digi.size()-1)
		{
			return sumQ<DIGI>(digi, ped, i, j)/(j-i+1);
		}
	}
}

#endif







