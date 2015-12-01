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

			virtual void initialize(std::string const &folder, 
				std::string const &name)
			{
				_folder = folder;
				_name = name;
			}

		protected:
			std::string					_folder;
			std::string					_name;
	};
}


#endif








