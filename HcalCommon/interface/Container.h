#ifndef Container_h
#define Container_h

/*
 *	file:		Container.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container Base class
 *		
 *
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"
#include "DQM/HcalCommon/interface/Mapper.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class Container
	{
		public:
			Container():
				_folder("HcalInfo"), _name("SOME_MONITOR")
			{}
			Container(std::string const& folder, std::string const &name):
				_folder(folder), _name(name)
			{}
			virtual ~Container() {}

			virtual void fill(double);
			virtual void fill(int);
//			virtual void fill(std::string const&);

			virtual void book(DQMStore::IBooker&);

		private:
			MonitorElement				*_me;
		protected:
			std::string					_folder;
			std::string					_name;
	};
}


#endif








