
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
		{
			this->_setup();
		}

		CoordinateAxis::CoordinateAxis(AxisType type, CoordinateType ctype,
			bool log):
			Axis(ctitle[ctype], type, fCoordinate, cnbins[ctype],
				cmin[ctype], cmax[ctype], log), _ctype(ctype)
		{
			this->_setup();
		}

		/* virtual */ int CoordinateAxis::get(HcalDetId const& did)
		{
			int x;
			switch(_ctype)
			{
				case fSubDet:
					x = did.subdet();
					break;
				case fSubDetPM:
					x = did.ieta()<0 ? 2*(did.subdet()-1) : 
						2*(did.subdet()-1)+1;
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

		/* virtual */ int CoordinateAxis::get(HcalTrigTowerDetId const& tid)
		{
			int x;
			switch(_ctype)
			{
				case fiphi:
					x = tid.iphi();
					break;
				case fTPieta:
					x = tid.ieta()<0 ? tid.ieta()+32 : tid.ieta()+31;
					break;
				case fTPSubDet:
					x = tid.ietaAbs()<29 ? 0 : 1;
					break;
				case fTPSubDetPM:
				{
					int ieta = tid.ieta();
					if (ieta<0 && ieta>-29)
						x = 0;
					else if (ieta>0 && ieta<29)
						x = 1;
					else if (ieta<0 && ieta<=-29)
						x = 2;
					else 
						x = 3;
					break;
				}
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
				case fSubDetPM:
					for (int i=0; i<2*SUBDET_NUM; i++)
						_labels.push_back(SUBDETPM_NAME[i]);
					break;
				case fTPSubDetPM:
					for (int i=0; i<2*TPSUBDET_NUM; i++)
						_labels.push_back(TPSUBDETPM_NAME[i]);
					break;
				case fieta:
					for (int ieta=-41; ieta<=41; ieta++)
					{
						if (ieta==0)
							continue;
						sprintf(name, "%d", ieta);
						if (ieta==-29 || ieta==29)
							_labels.push_back(std::string(name));
						_labels.push_back(std::string(name));
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
				case fTPSubDet:
					_labels.push_back(std::string("HBHE"));
					_labels.push_back(std::string("HF"));
					break;
				case fTPieta:
					for (int ieta=-32; ieta<=32; ieta++)
					{
						if (ieta==0)
							continue;
						sprintf(name, "%d", ieta);
						_labels.push_back(std::string(name));
					}
				default:
					break;
			}
		}
	}
}




