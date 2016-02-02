#ifndef Container1D_h
#define Container1D_h

/*
 *	file:		Container1D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		1D Container
 */
#include "DQM/HcalCommon/interface/DetectorQuantity.h"
#include "DQM/HcalCommon/interface/ElectronicsQuantity.h"
#include "DQM/HcalCommon/interface/TrigTowerQuantity.h"
#include "DQM/HcalCommon/interface/ValueQuantity.h"
#include "DQM/HcalCommon/interface/Container.h"
#include "DQM/HcalCommon/interface/HashMapper.h"
#include "DQM/HcalCommon/interface/Utilities.h"

#include <vector>
//#include <unordered_map>
//#include "boost/ptr_container/ptr_map.hpp"
#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
#include <string>

namespace hcaldqm
{

	using namespace quantity;
	using namespace mapper;
	class Container1D : public Container
	{
		public:
			Container1D();
			//	Initialize Container
			//	@folder - folder name where to start saving - root.
			//	By default it will be the Task's name.
			//
			Container1D(std::string const& folder,
				hashfunctions::HashType, 
				Quantity*,
				Quantity *qy =  new ValueQuantity(quantity::fN));
			virtual ~Container1D() {}

			//	Initialize Container
			//	@folder - folder name where to save. Should already include the
			//	Tasks's name
			//	@nametitle - namebase of the name and of the title
			//
			virtual void initialize(std::string const& folder, 
				hashfunctions::HashType,  Quantity*,
				Quantity *qy = new ValueQuantity(quantity::fN), 
				int debug=0);

			//	@qname - to replace the QyvsQx naming
			virtual void initialize(std::string const& folder, 
				std::string const &qname,
				hashfunctions::HashType,  Quantity*,
				Quantity *qy = new ValueQuantity(quantity::fN), 
				int debug=0);

			//	filling by hash
			virtual void fill(uint32_t);
			virtual void fill(uint32_t, int);
			virtual void fill(uint32_t, double);
			virtual void fill(uint32_t, int, double);
			virtual void fill(uint32_t, int, int);
			virtual void fill(uint32_t, double, double);

			//	using DetId as mapper
			virtual void fill(HcalDetId const&);
			virtual void fill(HcalDetId const&, int);
			virtual void fill(HcalDetId const&, double);
			virtual void fill(HcalDetId const&, int, double);
			virtual void fill(HcalDetId const&, int, int);
			virtual void fill(HcalDetId const&, double, double);

			virtual double getBinContent(HcalDetId const&);
			virtual double getBinContent(HcalDetId const&, int);
			virtual double getBinContent(HcalDetId const&, double);
			virtual double getBinContent(HcalDetId const& did, int x, int)
			{return getBinContent(did, x);}
			virtual double getBinContent(HcalDetId const& did, int x, double)
			{return getBinContent(did, x);}
			virtual double getBinContent(HcalDetId const& did, double x, double)
			{return getBinContent(did, x);}
			virtual double getMean(HcalDetId const&, int axis=1);

			//	using ElectronicsId 
			virtual void fill(HcalElectronicsId const&);
			virtual void fill(HcalElectronicsId const&, int);
			virtual void fill(HcalElectronicsId const&, double);
			virtual void fill(HcalElectronicsId const&, int, double);
			virtual void fill(HcalElectronicsId const&, int, int);
			virtual void fill(HcalElectronicsId const&, double, double);

			virtual double getBinContent(HcalElectronicsId const&);
			virtual double getBinContent(HcalElectronicsId const&, int);
			virtual double getBinContent(HcalElectronicsId const&, double);
			virtual double getBinContent(HcalElectronicsId const& eid, 
				int x, int)
			{return getBinContent(eid, x);}
			virtual double getBinContent(HcalElectronicsId const& eid, 
				int x, double)
			{return getBinContent(eid, x);}
			virtual double getBinContent(HcalElectronicsId const& eid, 
				double x, double)
			{return getBinContent(eid, x);}
			virtual double getMean(HcalElectronicsId const&, int axis=1);

			//	using DetId as mapper
			virtual void fill(HcalTrigTowerDetId const&);
			virtual void fill(HcalTrigTowerDetId const&, int);
			virtual void fill(HcalTrigTowerDetId const&, double);
			virtual void fill(HcalTrigTowerDetId const&, int, double);
			virtual void fill(HcalTrigTowerDetId const&, int, int);
			virtual void fill(HcalTrigTowerDetId const&, double, double);

			virtual double getBinContent(HcalTrigTowerDetId const&);
			virtual double getBinContent(HcalTrigTowerDetId const&, int);
			virtual double getBinContent(HcalTrigTowerDetId const&, double);
			virtual double getBinContent(HcalTrigTowerDetId const& tid, 
				int x, int)
			{return getBinContent(tid, x);}
			virtual double getBinContent(HcalTrigTowerDetId const& tid, 
				int x, double)
			{return getBinContent(tid, x);}
			virtual double getBinContent(HcalTrigTowerDetId const& tid, 
				double x, double)
			{return getBinContent(tid, x);}
			virtual double getMean(HcalTrigTowerDetId const&, int axis=1);

			//	booking using IBooker
			//	@aux - typically a cut or anything else
			//	@subsystem - subsystem under which to save
			//
			virtual void book(DQMStore::IBooker&, HcalElectronicsMap const*,
				std::string subsystem="Hcal",
				std::string aux="");

			//	loading using DQMStore
			//	@DQMStore
			//	@emap
			//	@prepend - name to prepend to /<subsystem> - used when load
			//	a file to DQMStore without stripping and in case a name is 
			//	appended 
			//	@mode - if StripRunDirs - then there will be no Run Summary
			//	otherwise there is Run Summary folde. Used for retrieving 
			//	MEs which were loaded into the DQM store from a file
			virtual void load(DQMStore*, HcalElectronicsMap const*,
				std::string const& subsystem="Hcal", 
				std::string const& aux="",
				std::string const& prepend="", DQMStore::OpenRunDirs
				mode=DQMStore::StripRunDirs);

			//	reset all the elements
			virtual void reset();

			//	print all the elements
			virtual void print();

		protected:
			virtual void customize(MonitorElement*);

			typedef boost::unordered_map<uint32_t, MonitorElement*> MEMap;
			MEMap									_mes;
			mapper::HashMapper						_hashmap;
			Quantity								*_qx;
			Quantity								*_qy;
	};
}

#endif








