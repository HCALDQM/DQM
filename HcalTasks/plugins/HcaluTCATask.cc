//	cmssw includes
#include "DQM/HcalTasks/interface/HcaluTCATask.h"

//	system includes
#include <iostream>
#include <string>

HcaluTCATask::HcaluTCATask(edm::ParameterSet const&ps):
	hcaldqm::HcalDQSource(ps)
{}

/* virtual */ HcaluTCATask::~HcaluTCATask()
{
}

/* virtual */void HcaluTCATask::beginLuminosityBlock(
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	HcalDQSource::beginLuminosityBlock(lb, es);
}

/* virtual */void HcaluTCATask::endLuminosityBlock(
		edm::LuminosityBlock const& lb, edm::EventSetup const& es)
{
	HcalDQSource::endLuminosityBlock(lb, es);
}

/* virtual */ void HcaluTCATask::doWork(edm::Event const& e,
		edm::EventSetup const& es)
{
	edm::Handle<HFDigiCollection>		chf_vme;
	edm::Handle<HFDigiCollection>		chf_utca;
	edm::Handle<HBHEDigiCollection>		chbhe_vme;
	edm::Handle<HBHEDigiCollection>		chbhe_utca;

	INITCOLL(_labels["HFDigi"], chf_utca);
	INITCOLL(_labels["HFDigiVME"], chf_vme);
	INITCOLL(_labels["HBHEDigi"], chbhe_vme);
	INITCOLL(_labels["HBHEDigiuTCA"], chbhe_utca);

	//	1 stands for what is the default readout.
	//	2 stands for what we are comparing defualt to/with
	this->process(*chf_utca, *chf_vme, std::string("HF"), 1);
	this->process(*chf_vme, *chf_utca, std::string("HF"), 2);
	this->process(*chbhe_vme, *chbhe_utca, std::string("HB"), 1);
	this->process(*chbhe_vme, *chbhe_utca, std::string("HE"), 1);
	this->process(*chbhe_utca, *chbhe_vme, std::string("HB"), 2);
	this->process(*chbhe_utca, *chbhe_vme, std::string("HE"), 2);
}

//	specializer
template<typename Hit>
void HcaluTCATask::specialize(Hit const& hit1, Hit const& hit2, 
		std::string const& nameRes, int const wtw)
{
	//	Skip wtw==2, cause that means we've already been here
	if (wtw==2)
		return;
	
	//	Get all the info you need
	double ps1 = hit1.presamples();
	double ps2 = hit2.presamples();
	int digisize1 = hit1.size();
	int digisize2 = hit2.size();
	int iphi = hit1.id().iphi();
	int ieta = hit1.id().ieta();
	int depth = hit1.id().depth();

	//	Fill Plots
	for (int i=0; i<hit1.size(); i++)
		_mes[nameRes + "_ADC"].Fill(hit2.sample(i).adc(),
			hit1.sample(i).adc());

	_mes[nameRes + "_Samples"].Fill(digisize1);
	_mes[nameRes + "_Samples"].Fill(digisize2);
	_mes[nameRes + "_PreSamples"].Fill(ps1);
	_mes[nameRes + "_PreSamples"].Fill(ps2);
	_mes["HBHEHFD" + boost::lexical_cast<std::string>(depth) + 
		"_Matched"].Fill(ieta, iphi);
}

//	checker
template<typename Hit>
void HcaluTCATask::check(Hit const& hit,
		std::string const& nameRes, int const wtw)
{

}


//	Reset
/* virtual */void HcaluTCATask::reset(int const periodflag)
{
	HcalDQSource::reset(periodflag);
	if (periodflag==0)
	{
		//	each event
	}
	else if (periodflag==1)
	{
		//	Each LS
	}
}

DEFINE_FWK_MODULE(HcaluTCATask);



