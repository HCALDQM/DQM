
#include "DQM/HcalCommon/interface/CoordinateAxis.h"

namespace hcaldqm
{
	namespace axis
	{
		CoordinateAxis::CoordinateAxis():
			Axis(), _ctype(fSubDet)
		{}

		CoordinateAxis::CoordinateAxis(AxisType type, CoordinateType ctype,
			int n, double min, double max, std::string title, bool log):
			Axis(title, type, fCoordinate, n, min, max, log), _ctype(ctype)
		{}

		CoordinateAxis::CoordinateAxis(AxisType type, CoordinateType ctype,
			bool log):
			Axis(ctitle[ctype], type, fCoordinate, cnbins[ctype],
				cmin[ctype], cmax[ctype], log), _ctype(ctype)
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
					if (x<0)
						x = did.subdet()==HcalForward ? x+41 : x+42;
					else 
						x = did.subdet()==HcalForward ? x+42 : x+41;
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
						x = i-FED_uTCA_MIN+FED_VME_NUM;
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
				case fSubDet:
					for (int i=HB; i<=HF; i++)
						_labels.push_back(SUBDET_NAME[i-1]);
					break;
				case fieta:
					for (int ieta=-41; ieta<=41; ieta++)
					{
						sprintf(name, "%d", ieta);
						if (ieta==-29 || ieta==29)
							_labels.push_back(name);
						_labels.push_back(name);
					}
					break;
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




