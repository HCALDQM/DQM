
#include "DQM/HcalCommon/interface/Axis.h"

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
	
		Axis::Axis(AxisType type, AxisQType qtype):
			_type(type), _qtype(qtype), _title(axisTitle[qtype]),
			_nbins(axisNbins[qtype]), _min(axisMin[qtype]),
			_max(axisMax[qtype]), _log(axisLogs[qtype])
		{}
		
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
			o->SetBit(BIT(FIRST_BIT_TO_USE+_type));
		}
	}
}













