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
#include "DQM/HcalCommon/interface/Constants.h"

namespace hcaldqm
{
	using namespace constants;
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
		double aveTS(DIGI digi, double ped=0, int i=0, int j=3)
		{
			double sumQ = 0;
			double sumQT = 0;
			for (int ii=i; ii<=j; ii++)
			{
				sumQ+=digi.sample(ii).nominal_fC()-ped;
				sumQT +=(ii+1)*(digi.sample(ii).nominal_fC()-ped);
			}
			
			return sumQ>0 ? sumQT/sumQ-1 : GARBAGE_VALUE;
		}

		template<typename DIGI>
		double sumQ(DIGI digi, double ped, int i=0, int j=3)
		{
			double sum=0;
			for (int ii=i; ii<=j; ii++)
				sum+=(digi.sample(ii).nominal_fC()-ped);
			return sum;
		}

		template<typename DIGI>
		double aveQ(DIGI digi, double ped, int i=0, int j=3)
		{
			return sumQ<DIGI>(digi, ped, i, j)/(j-i+1);
		}

		template<typename DIGI>
		double sumADC(DIGI digi, double ped, int i=0, int j=3)
		{
			double sum = 0;
			for (int ii=i; ii<=j; ii++)
				sum+=digi.sample(ii).adc()-ped;
			return sum;
		}

		template<typename DIGI>
		double aveADC(DIGI digi, double ped, int i=0, int j=3)
		{
			return sumADC<DIGI>(digi, ped, i, j)/(j-i+1);
		}

		template<typename DIGI>
		bool isError(DIGI digi);

		/*
		 *	Log Functions
		 */
		template<typename STDTYPE>
		void log(STDTYPE x)
		{
			std::cout << "%MSG" << std::endl;
			std::cout << "%MSG-d HCALDQM::" << x;
			std::cout << std::endl;
		}

		/*
		 *	Useful Detector Functions. For Fast Detector Validity Check
		 */
		bool validDetId(HcalSubdetector, int , int, int );
		bool validDetId(HcalDetId const&);

	}
}

#endif







