#ifndef ValueAxis_h
#define ValueAxis_h

/*
 *	file:		ValueAxis.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Value Axis
 */

#include "DQM/HcalCommon/interface/Axis.h"

namespace hcaldqm
{
	namespace axis
	{
		enum ValueType
		{
			fEntries = 0,
			fEvents = 1,
			f1OverEvents = 2,
			fEnergy = 3,
			fTime = 4,
			fADC = 5,
			fADC_15 = 6,
			fNomFC = 7,
			fNomFC_1000 = 8,
			fNomFC_3000 = 9,
			fTimeTS = 10,

			nValueType = 11
		};

		std::string const vtitle[nValueType] = {
			"Entries", "Events", "1/Events", "Energy (GeV)", "Time (ns)",
			"ADC", "ADC", "nom. fC", "nom. fC", "nom. fC", "Time Slice"
		};
		double const vmin[nValueType] = {
			0, 0, 0, 0, -50, 0, 0, -0.5, -0.5, -0.5, -0.5
		};
		double const vmax[nValueType] = {
			3000, 500, 1, 200, 50, 128, 15, 10000, 1000, 3000, 9.5
		};
		int const vnbins[nValueType] = {
			500, 100, 100, 400, 200, 128, 15, 1000, 200, 600, 10
		};

		class ValueAxis : public Axis
		{
			public:
				ValueAxis() :
					Axis(), _vtype(fEnergy)
				{}
				ValueAxis(AxisType type, ValueType vtype, int n=vnbins[vtype], 
					double min=vmin[vtype],	double max=vmax[vtype], 
					std::string title=vtitle[vtype], bool log=false):
					Axis(title, type, fValue, n, min, max, log), _vtype(vtype)
				{}
				virtual ~ValueAxis() {}

			protected:
				virtual void setup();

				ValueType _vtype;
		};
	}
}

#endif



