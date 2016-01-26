#ifndef ContainerXXX_h
#define ContainerXXX_h

/*
 *	file:			ContainerXXX.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		1D Compact Container
 */

#include "DQM/HcalCommon/interface/Container1D.h"

namespace hcaldqm
{
	using namespace constants;
	
	struct Compact
	{
		double _sum;
		double _sum2;
		unsigned int _entries;
	};

	class ContainerXXX
	{
		public:
			ContainerXXX(hashfunctions::HashType){}
			virtual ~ContainerXXX() {_cmap.clear();}

			//	initializer
			virtual void initialize(hashfunctions::HashType);

			//	book
			virtual void book(HcalElectronicsMap const*);

			//	fills
			virtual void fill(HcalDetId const&, double);
			virtual void dump(Container1D*, bool);

		protected:
			typedef boost::unordered_map<uint32_t, Compact> CompactMap;
			CompactMap				_cmap;
			mapper::HashMapper		_hashmap;
	};
}

#endif


