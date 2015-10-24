
#include "DQM/HcalCommon/CoordinateAxis.h"

namespace hcaldqm
{
	namespace axis
	{
		CoordinateAxis::CoordinateAxis(AxisType type, CoordinateType ctype,
			int n, double min, double max, std::string title, bool log):
			Axis(title, type, fCoordinate, n, min, max, log), _ctype(ctype)
		{}

		/* virtual */ int CoordinateAxis::get(HcalDetId const& did)
		{
			int x;
			switch(_ctype)
			{
				case fSubDet:
					x = did.subdet();
					break;
				case fiphi:
					x = did.iphi();
					break;
				case fieta:
					x = did.ieta();
					break;
				case fdepth:
					x = did.depth();
					break;
				default:
					x = -100;
					break;
			}
			return x;
		}

		/* virtual */ int CoordinateAxis::get(HcalElectronicsId const& eid)
		{
			int x;
			switch(_ctype)
			{
				case fCrate:
					x = eid.crateId();
					break;
				case fSlot:
					x = eid.slot();
					break;
				case fFiber:
					x = eid.fiberIndex();
					break;
				case fFiberCh:
					x = eid.fiberChanId();
					break;
				default :
					x = -100;
					break;
			}
			return x;
		}

		/* virtual */ int CoordinateAxis::get(int i)
		{
			int x = 0;
			switch(_ctype)
			{
				case fFED:
					if (i<=FED_VME_MAX)
						x = i-FED_VME_MIN;
					else 
						x = i-FED_uTCA_MIN+FED_VME+NUM;
					break;
				default :
					break;
			}
			return x;
		}

		/* virtual */ void CoordinateAxis::_setup()
		{
			char name[20];
			switch (_ctype)
			{
				case fFED:
					for (int i=FED_VME_MIN; i<=FED_VME_MAX; i++)
					{
						sprintf(name, "FED%d", i);
						_labels.push_back(std::string(name));
					}
					for (int i=FED_uTCA_MIN; i<=FED_uTCA_MAX; i+=2)
					{
						sprintf(name, "FED%d", i);
						_labels.push_back(std::string(name));
					}
					break;
				default:
					break;
			}
		}
	}
}




