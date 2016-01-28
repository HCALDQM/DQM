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
#include "DQM/HcalCommon/interface/Compact.h"

namespace hcaldqm
{
	using namespace constants;
	using namespace compact;

	double ratio(double, double);
	double diff(double, double);
	bool pedestal_quality(double); // diff to be  provided typically
	bool led_quality(double); // ratio 
	bool laser_quality(double); // ratio
	typedef double (*comparison_function)(double, double);
	typedef bool (*quality_function)(double);

	class ContainerXXX
	{
		public:
			ContainerXXX() {}
			ContainerXXX(hashfunctions::HashType ht, CompactUsageType 
				ut=fHistogram)
				: _hashmap(ht), _usetype(ut)
			{}
			virtual ~ContainerXXX() {_cmap.clear();}

			//	initializer
			virtual void initialize(hashfunctions::HashType,
				CompactUsageType=fHistogram);

			//	book
			virtual void book(HcalElectronicsMap const*);

			//	fills - only in histo mode
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalElectronics const&, double);
			virtual void fill(HcalTrigTowerDetid const&, double);

			//	sets - only in non-histo mode
			virtual void set(HcalDetId const&, double);
			virtual void set(HcalElectronics const&, double);
			virtual void set(HcalTrigTowerDetid const&, double);

			//	get the number of entries
			virtual uint32_t getEntries(HcalDetId const&);
			virtual uint32_t getEntries(HcalElectronicsId const&);
			virtual uint32_t getEntries(HcalTrigTowerDetId const&);

			//	dump all the contents into the Container
			//	for mean bool=true
			virtual void dump(Container1D*, bool q=true);
			virtual void dump(std::vector<Container1D*> const&, bool q=true);

			//	get all the contents from the Container
			virtual void load(Container1D*);
			virtual void load(Container1D*, Container1D*);

			//	compare 2 sets of data
			//	1-st Container1D is to where to dump the comparison
			//	2-nd Container1D is to where to dump the non-present channels
			//	3-rd COntainer1D is to where to dump the quality faulire channels.
			//	
			virtual void compare(ContainerXXX const&, Container1D*,
				Container1D*, Container1D*, comparison_function, 
				quality_function, bool q=true);
			virtual void compare(ContainerXXX const&, 
				std::vector<Container1D*> const&, Container1D*,
				Container1D*, comparison_function, quality_function, 
				bool q=true);

			//	reset
			virtual void reset();

			//	size
			virtual uint32_t size();

			//	print
			virtual void print();

		protected:
			typedef boost::unordered_map<uint32_t, Compact> CompactMap;
			CompactMap				_cmap;
			mapper::HashMapper		_hashmap;
			CompactUsageType		_usetype;
	};
}

#endif


