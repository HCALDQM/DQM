/*
 *	file:		HcalColorManger.h
 *	author:		Viktor
 *	Revision:		
 *
 *	Description:
 *		A wrapper around all of the possible Color Schemes. 
 *		The basic reason for this class is to keep all the colors in one place.
 */

//	ROOT includes
#include "TPalleteAxis.h"
#include "TH2.h"

//	STD and STL
#include "string"
#include "vector"

//	hcaldqm namespace
namespace hcaldqm
{
	enum ColorTypes
	{
		kRainbow = 0,
		kSummary = 1,
		kError =2,
		nColorTypes = 
	};

	//	Class HcalColorManager
	class HcalColorManager
	{
		public:
			//	Constructor
			HcalColorManager()
			{
				_colors = new Double_t*[nColorTypes];
			}

			//	Destructor
			~HcalColorManger()
			{
				
				for (int i=0; i<nColorTypes; i++)
					//	first delete the array for a particular color type
					delete [] _colors[i];
				// then delete the array of types
				delete [] _colors;
			}

			void customize_Colors(TH2*, std::string)
			{
				
			}
			
			//	members
			//	2D array of all colors. First index is the color type
			//	and then the array of colors
			Double_t **_colors;
	}
}

