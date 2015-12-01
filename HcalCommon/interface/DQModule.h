#ifndef DQModule_h
#define DQModule_h

/*
 *	file:			DQModule.h
 *	Author:			Viktor Khristenko
 *	date:			13.10.2015
 */

#include "DQM/HcalCommon/interface/HcalCommonHeaders.h"

#include <string>
#include <vector>
#include <iostream>

namespace hcaldqm
{
	enum ModuleType
	{
		fTask = 0,
		fClient = 1,
		nModuleType = 2
	};

	enum ProcessingType
	{
		fOnline = 0,
		fOffline = 1,
		fLocal = 2,
		nProcessingType = 3
	};

	std::string const pTypeNames[nProcessingType] = {
		"Online", "Offline", "Local"
	}

	class DQModule
	{
		public:
			DQModule(edm::ParameterSet const&);
			virtual ~DQModule() {}

		protected:
			//	protected Member functions
			inline void _throw(std::string const& msg) const
			{
				throw cms::Exception("HCALDQM") << _name << "::"
					<< msg;
			}
			inline void _throw(std::string const& msg1,
				std::string const& msg2) const
			{
				throw cms::Exception("HCALDQM") << _name << "::" << msg1
					<< msg2;
			}
			inline void _warn(std::string const& msg) const
			{
				edm::LogWarning("HCALDQM") << _name << "::" << msg;
			}
			inline void _warn(std::string const& msg1, 
				std::string const& msg2) const
			{
				edm::LogWarning("HCALDQM") << _name << "::" << msg1 << msg2;
			}
			inline void _info(std::string const& msg) const
			{
				if (_debug==0)
					return;
				edm::LogInfo("HCALDQM") << _name << "::" << msg;
			}
			inline void _logdebug(std::string const& msg) const
			{
				if (_debug==0)
					return;
				std::cout << "%MSG" << std::endl;
				std::cout << "%MSG-d HCALDQM::" << _name << "::" << msg;
				std::cout << std::endl;
			}

		protected:
			//	Member variables	
			std::string				_name;
			ModuleType				_mtype;
			ProcessingType			_ptype;
			int						_debug;

			int						_runkeyVal;
			std::string				_runkeyName;
			std::string				_subsystem;

			int						_evsTotal;
			int						_evsPerLS;
			int						_currentLS;
	};
}

#endif










