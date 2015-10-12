
#include "DQM/HcalCommon/interface/Container.h"

namespace hcaldqm
{
	/* virtual */ void Container::fill(double x) 
	{
		_me->Fill(x);	
	}

	/* virtual */ void Container::fill(int x)
	{
		_me->Fill(x);
	}

	/* virtual */ /*void Container::fill(std::string const& x)
	{
		_me->Fill(x);
	}*/

	/* virtual */ void Container::book(DQMStore::IBooker& ib)
	{
	}
}





