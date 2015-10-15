
#include "DQM/HcalCommon/interface/Axis.h"

namespace hcaldqm
{
	using namespace constants;
	namespace axis
	{
		Axis::Axis():
			_type(fXaxis), _qtype(fEnergy), _title(axisTitle[fEnergy]),
			_nbins(axisNbins[fEnergy]), _min(axisMin[fEnergy]),
			_max(axisMax[fEnergy]), _log(axisLogs[fEnergy]),
			_mtype(mapper::fSubDet)
		{}
	
		Axis::Axis(AxisType type, AxisQType qtype, mapper::MapperType mtype):
			_type(type), _qtype(qtype), _title(axisTitle[qtype]),
			_nbins(axisNbins[qtype]), _min(axisMin[qtype]),
			_max(axisMax[qtype]), _log(axisLogs[qtype]), _mtype(mtype)
		{
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
					if (x<=CRATE_VME_MAX)
						x-= CRATE_VME_MIN;
					else 
						x = x-CRATE_uTCA_MIN+CRATE_VME_NUM;
					break;
				case fSlot:
					int crate = eid.crateId();
					int slot = eid.slot();
					if (crate<=CRATE_VME_MAX)
						x = slot<=SLOT_VME_MIN1? slot-SLOT_VME_MIN :
							SLOT_VME_NUM1+slot-SOT_VME_MIN2;
					else 
						x = SLOT_uTCA_MIN;
					break;
				case fFiber:
					int fiber = eid.fiberIndex();
					int crate = eid.crateId();
					if (crate<=CRATE_VME_MAX)
						x = fiber-FIBER_VME_MIN;
					else 
						x = fiber-FIBER_uTCA_MIN;
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
			if (axisLogs[_qtype])
				o->SetBit(BIT(FIRST_BIT_TO_USE+_type));
		}
	}
}













