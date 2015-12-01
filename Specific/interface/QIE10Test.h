#ifndef QIE10Test_h
#define QIE10Test_h

/*
 *	file:			QIE10Test.h
 *	Author:			Viktor KHristenko
 *	Description:
 *		Test of QIE10 Read out
 */

#include "DQM/HcalCommon/interface/DQTask.h"
#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/Container2D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf1D.h"
#include "DQM/HcalCommon/interface/ContainerSingleProf1D.h"

using namespace hcaldqm;
class QIE10Test : public DQTask
{
	public:
		QIE10Test(edm::ParameterSet const&);
		virtual ~QIE10Test(){}

		virtual void bookHistograms(DQMStore::IBooker&,
			edm::Run const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

	protected:
		virtual void _process(edm::Event const&, edm::EventSetup const&);
		virtual void _resetMonitors(UpdateFreq);

		//	tags
		edm::InputTag	_tagHF;

		//	Containers
};

#endif



