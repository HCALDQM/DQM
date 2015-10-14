#ifndef ContainerD_h
#define ContainerD_h

#include "DQM/HcalCommon/interface/Container.h"

namespace hcaldqm
{
	using namespace constants;
	class ContainerD : public Container
	{
		public:
			ContainerD():
				Container()
			{}
			ContainerD(std::string const& folder, std::string const& name):
				Container(folder, name)
			{}
			virtual ~ContainerD() {}

			virtual void fill(double x)
			{
				_me->Fill(x);
			}
			
			virtual void book(DQMStore::IBooker &ib)
			{
				ib.setCurrentFolder(SUBSYSTEM+"/"+_folder);
				_me = ib.bookFloat(_name);
			}

		protected:
			MonitorElement			*_me;
	};
}

#endif















