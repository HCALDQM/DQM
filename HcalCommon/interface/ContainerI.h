#ifndef ContainerI_h
#define ContainerI_h

#include "DQM/HcalCommon/interface/Container.h"

namespace hcaldqm
{
	using namespace constants;
	class ContainerI : public Container
	{
		public:
			ContainerI():
				Container()
			{}
			ContainerI(std::string const& folder, std::string const& name):
				Container(folder, name)
			{}
			virtual ~ContainerI() {}

			virtual void fill(int x)
			{
				_me->Fill(x);
			}
			
			virtual void book(DQMStore::IBooker &ib, std::string subsystem="Hcal")
			{
				ib.setCurrentFolder(subsystem+"/"+_folder);
				_me = ib.bookInt(_name);
			}

		protected:
			MonitorElement			*_me;
	};
}

#endif
















