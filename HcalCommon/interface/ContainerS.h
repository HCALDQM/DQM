#ifndef ContainerS_h
#define ContainerS_h

#include "DQM/HcalCommon/interface/Container.h"

namespace hcaldqm
{
	class ContainerS : public Container
	{
		using namespace constants;
		public:
			ContainerS():
				Container()
			{}
			ContainerS(std::string const& folder, std::string const& name):
				Container(folder, name)
			{}
			virtual ~ContainerS() {}

			virtual void fill(std::string const& x)
			{
				_me->Fill(x);
			}
			
			virtual void book(DQMStore::IBooker &ib)
			{
				ib.setCurrentFolder(SUBSYSTEM+"/"+_folder);
				_me = ib.bookString(_name);
			}

		protected:
			MonitorElement			*_me;
	};
}

#endif
















