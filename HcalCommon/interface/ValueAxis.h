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
			fADC_5 = 6,
			fADC_15 = 7,
			fNomFC = 8,
			fNomFC_1000 = 9,
			fNomFC_3000 = 10,
			fTimeTS = 11,
			fTimeTS_200 = 12,
			fLS = 13,

			nValueType = 14
		};

		std::string const vtitle[nValueType] = {
			"Entries", "Events", "1/Events", "Energy (GeV)", "Time (ns)",
			"ADC", "ADC", "ADC", "nom. fC", "nom. fC", "nom. fC", "Time Slice",
			"Time Slice", "LS"
		};
		double const vmin[nValueType] = {
			0, 0, 0, 0, -50, 0, 0, 0, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5
		};
		double const vmax[nValueType] = {
			3000, 500, 1, 200, 50, 128, 5, 15, 10000, 1000, 3000, 9.5, 9.5, 4000.5
		};
		int const vnbins[nValueType] = {
			500, 100, 100, 400, 200, 128, 100, 300, 1000, 200, 600, 10, 200, 4001
		};

		class ValueAxis : public Axis
		{
			public:
				friend class hcaldqm::Container;
				friend class hcaldqm::Container1D;
				friend class hcaldqm::Container2D;
				friend class hcaldqm::ContainerProf1D;
				friend class hcaldqm::ContainerProf2D;
				friend class hcaldqm::ContainerSingle2D;
				friend class hcaldqm::ContainerSingle1D;

			public:
				ValueAxis();
				ValueAxis(AxisType type, ValueType vtype, bool log=false);
				ValueAxis(AxisType type, ValueType vtype, int n, 
					double min,	double max, std::string title, bool log=false);
				virtual ~ValueAxis() {}

			protected:
				virtual void _setup();

				ValueType _vtype;
		};
	}
}

#endif



