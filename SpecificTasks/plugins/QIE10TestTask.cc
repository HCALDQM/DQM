
#include "DQM/SpecificTasks/interface/QIE10TestTask.h"

using namespace hcaldqm;
QIE10TestTask::QIE10TestTask(edm::ParameterSet const& ps):
	DQTask(ps)
{
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 10, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 11, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 12, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 1, 13, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 10, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 11, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 12, 1));
	_cDetIds.push_back(HcalDetId(HcalForward, 2, 13, 1));

	char name[200];
	for (unsigned int i=0; i<_cDetIds.size(); i++)
	{
		sprintf(name, "_iphi%d_ieta%d", _cDetIds[i].iphi(), _cDetIds[i].ieta());
		_vcShape.push_back(ContainerSingle1D(_name+"/Shape",
			"Shape"+std::string(name), 
			new axis::ValueAxis(axis::fXaxis, fTimeTS),
			new axis::ValueAxis(axis::fYaxis, fNomFC)));
		for (unsigned int j=0; j<10; j++)
		{
			char aux[10];
			sprintf(aux, "_TS%d", j);
			_vcLETDCvsADC[j].push_back(ContainerSingle2D(_name+"/LETDCvsADC",
				"LETDCvsADC"+std::string(name)+std::string(aux),
				new axis::ValueAxis(axis::fXaxis, axis::fQIE10ADC256),
				new axis::ValueAxis(axis::fYaxis, axis::fQIE10TDC64),
				new axis::ValueAxis(axis::fZaxis, axis::fEntries)));
			_vcTETDCvsADC[j].push_back(ContainerSingle2D(_name+"/TETDCvsADC",
				"TETDCvsADC"+std::string(name)+std::string(aux),
				new axis::ValueAxis(axis::fXaxis, axis::fQIE10ADC256),
				new axis::ValueAxis(axis::fYaxis, axis::fQIE10TDC64),
				new axis::ValueAxis(axis::fZaxis, axis::fEntries)));
		}
	}

	//	tags
	_tagHF = ps.getUntrackedParameter<edm::InputTag>("tagHF",
		edm::InputTag("hcalDigis"));
}

/* virtual */ void QIE10TestTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{
	DQTask::bookHistograms(ib, r, es);

	for (unsigned int i=0; i<_cDetIds.size(); i++)
	{
		_vcShape[i].book(ib, _subsystem);
		for (unsigned int j=0; j<10; j++)
		{
			_vcLETDCvsADC[j][i].book(ib, _subsystem);
			_vcTETDCvsADC[j][i].book(ib, _subsystem);
		}
	}
}

/* virtual */ void QIE10TestTask::endLuminosityBlock(edm::LuminosityBlock const& lb,
	edm::EventSetup const& es)
{

	//	finish
	DQTask::endLuminosityBlock(lb, es);
}

/* virtual */ void QIE10TestTask::_process(edm::Event const& e, 
	edm::EventSetup const&)
{
	edm::Handle<QIE10DigiCollection> cqie10;
	if (!e.getByLabel(_tagHF, cqie10))
		std::cout << "Collection isn't availalbe" << std::endl;

//	std::cout << "Event" << std::endl;
	for (int i=0; i<cqie10->size(); i++)
	{
		HcalDetId did = (*cqie10)[i].detid();

		std::cout << "#unpacked=" << cqie10->size() << std::endl;
		for (unsigned int ii=0; ii<_cDetIds.size(); ii++)
			std::cout << _cDetIds[ii].iphi() << " " << _cDetIds[ii].ieta()
				<< " " << _cDetIds[ii].depth() << std::endl;

		std::cout << "#digis=" << cqie10->size() << std::endl;
		for (unsigned int ii=0; ii<_cDetIds.size(); ii++)
		{
			if (did==_cDetIds[i])
			{
				std::cout << "Matched " << i << std::endl;
				std::cout << did.iphi() << " " << did.ieta() << " " 
					<< did.depth() << " " << did.rawId() << std::endl;
				for (int j=0; j<(*cqie10)[i].samples(); j++)
				{
					_vcShape[i].fill(j, adc2fC[(*cqie10)[i][j].adc()]);	
					std::cout << (*cqie10)[i][j].adc() << "  " << 
					" " << (*cqie10)[i][j].le_tdc() << " "
						<< (*cqie10)[i][j].te_tdc() << std::endl;
					_vcLETDCvsADC[j][i].fill((*cqie10)[i][j].adc(), 
						(*cqie10)[i][j].le_tdc());
					_vcTETDCvsADC[j][i].fill((*cqie10)[i][j].adc(), 
						(*cqie10)[i][j].te_tdc());
				}
				std::cout << std::endl;
			}
		}
//			else 
//			{
//				std::cout << "Didn't match" << std::endl;
//				std::cout << did.iphi() << " " << did.ieta() << " "
//					<< did.depth() << " " << did.rawId() << std::endl;
//			}
	}
}

/* virtual */ void QIE10TestTask::_resetMonitors(UpdateFreq)
{
}

DEFINE_FWK_MODULE(QIE10TestTask);
