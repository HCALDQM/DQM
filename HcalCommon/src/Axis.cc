
#include "DQM/HcalCommon/interface/Axis.h"
#include <sstream>

namespace hcaldqm
{
	using namespace constants;
	namespace axis
	{
		Axis::Axis():
			_type(fXaxis), _qtype(fEnergy), _title(axisTitle[fEnergy]),
			_nbins(axisNbins[fEnergy]), _min(axisMin[fEnergy]),
			_max(axisMax[fEnergy]), _log(axisLogs[fEnergy])
		{}
	
		Axis::Axis(AxisType type, AxisQType qtype, mapper::MapperType mtype):
			_type(type), _qtype(qtype), _title(axisTitle[qtype]),
			_nbins(axisNbins[qtype]), _min(axisMin[qtype]),
			_max(axisMax[qtype]), _log(axisLogs[qtype])
		{
			char name[20];
			switch(_qtype)
			{
				case fFED:
					
					for (i=FED_VME_MIN;i<=FED_VME_MAX; i++)
					{
						sprintf(name, "FED%d", i);
						_labels.push_back(std::string(name));
					}
					for (i=FED_uTCA_MIN; i<=FED_uTCA_MAX; i+=2)
					{
						sprintf(name, "FED%d", i);
						_labels.push_back(std::string(name));
					}
					break;
				case fSubDet:
					for (i=HB; i<=HF; i++)
						_labels.push_back(SUBDET_NAME[i-1]);
					break;
				default:
					break;
			}
		}

		/* virtual */ int Axis::resolve(HcalDetId const& did)
		{
			int x;
			switch(_qtype)
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

		/* virtual */ int Axis::resolve(HcalElectronicsId const& eid)
		{
			int x;
			switch(_qtype)
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
				default:
					x=-100;
					break;
			}
			return x;
		}

		/* virtual */ int Axis::resolve(int i)
		{
			int x;
			switch(_qtype)
			{
				case fFED:
					if (i<=FED_VME_MAX)
						x = i -FED_VME_MIN;
					else
						x = i-FED_uTCA_MIN+FED_VME_NUM;
					break;
				default:
					break;
			}
		}

		/* virtual */ AxisQ Axis::getAxisQ()
		{
			int t = _qtype;
			if (t<=8)
				return fCoordinate;
			else if (t<=12)
				return fValue;
			else 
				return fFlag;

			return fCoordinate;
		}

		void Axis::setAxisLog(TObject* o)
		{
			if (axisLogs[_qtype])
				o->SetBit(BIT(FIRST_BIT_TO_USE+_type));
		}
	}
}













