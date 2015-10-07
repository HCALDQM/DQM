
#include "DQM/HcalCommon/src/Axis.h"

namespace hcaldqm
{
	Axis::Axis():
		_type(fXaxis)_title("Undefined"), _log(false), 
		_nbins(10), _min(0), _max(10)
	{}

	Axis::Axis(AxisType type):
		_type(type)_title("Undefined"), _log(false), 
		_nbins(10), _min(0), _max(10)
	{}

	Axis::Axis(AxisType type, std::string const& title):
		_type(type), _title(title), _nbins(10), _min(0), _max(10), _log()
	{}

	Axis::Axis(AxisType type, std::string const& title, 
		int nbins, double min, double max,
		bool log):
		_type(type), _title(title), _nbins(nbins), _max(max), _min(min), _log(log)
	{

	}

	/* virtual */Axis::~Axis()
	{

	}

	void Axis::setAxisLog(TObject* o)
	{
		o->SetBit(BIT(constants::FREE_BIT_START+_type));
	}

	void Axis::setAxisTitle(TAxis* ax)
	{
		ax->SetTitle(_title.c_str());
	}

	void Axis::setAxisBins(TAxis* ax)
	{
		ax->Set(nbins, min, max);
	}
}













