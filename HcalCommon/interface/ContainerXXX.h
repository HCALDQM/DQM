#ifndef ContainerXXX_h
#define ContainerXXX_h

/*
 *	file:			ContainerXXX.h
 *	Author:			Viktor Khristenko
 *
 *	Description:
 *		Templated Container to hold std types, bool, double, int
 */

#include "DQM/HcalCommon/interface/Container1D.h"
#include "DQM/HcalCommon/interface/Logger.h"

namespace hcaldqm
{
	using namespace constants;

	template<STDTYPE>
	class ContainerXXX
	{
		public:
			ContainerXXX() {}
			ContainerXXX(hashfunctions::HashType ht)
				: _hashmap(ht)
			{}
			virtual ~ContainerXXX() {_cmap.clear();}

			//	initializer
			virtual void initialize(hashfunctions::HashType,int debug=0);

			//	book
			virtual void book(HcalElectronicsMap const*);
			virtual void book(HcalElectronicsMap const*, 
				filter::HashFilter const&);

			//	set
			virtual void set(HcalDetId const&, STDTYPE);
			virtual void set(HcalElectronicsId const&, STDTYPE);
			virtual void set(HcalTrigTowerDetId const&, STDTYPE);

			//	to modify...
			virtual STDTYPE& get(HcalDetId const&);
			virtual STDTYPE& get(HcalElectronicsId const&);
			virtual STDTYPE& get(HcalTrigTowerDetId const&);

			//	dump all the contents into the Container
			virtual void dump(Container1D*);
			virtual void dump(std::vector<Container1D*> const&);

			//	get all the contents from the Container
			virtual void load(Container1D*);

			//	reset
			virtual void reset();

			//	size
			virtual uint32_t size();

			//	print
			virtual void print();

		protected:
			typedef boost::unordered_map<uint32_t, STDTYPE> CompactMap;
			CompactMap				_cmap;
			mapper::HashMapper		_hashmap;
			Logger					_logger;
	};
}

#endif


