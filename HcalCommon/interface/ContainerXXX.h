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
#include "DQM/HcalCommon/interface/ContainerCompact.h"

namespace hcaldqm
{
	using namespace constants;
	
	class ContainerXXX
	{
		public:
			ContainerXXX() {}
			ContainerXXX(hashfunctions::HashType){}
			virtual ~ContainerXXX() {_cmap.clear();}

			//	initializer
			virtual void initialize(hashfunctions::HashType);

			//	book
			virtual void book(HcalElectronicsMap const*);

			//	fills
			virtual void fill(HcalDetId const&, double);

			//	dump all the contents into the Container
			virtual void dump(Container1D*, bool);

			//	get all the contents from the Container
			virtual void load(Container1D*);

			//	compare 2 sets of data
			virtual void compare(ContainerXXX const&, Container1D*);

			//	print
			virtual void print();

		protected:
			typedef boost::unordered_map<uint32_t, CompactX> CompactMap;
			CompactMap				_cmap;
			mapper::HashMapper		_hashmap;
	};
}

#endif


