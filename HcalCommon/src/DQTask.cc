
#include "DQM/HcalCommon/interface/DQTask.h"

namespace hcaldqm
{
	DQTask::DQTask(edm::ParameterSet const& ps):
		DQModule(ps),
		_cEvsTotal(_name, "EvsTotal"),
		_cEvsPerLS(_name, "EvsPerLS");
	{}

	/*
	 *	By design, all the sources will ahve this function inherited and will
	 *	never override. 
	 */
	/* virtual */ void DQTask::analyze(edm::Event const& e,
		edm::EventSetup const& es)
	{
		try
		{
			this->_resetMonitors(0);
			this->_debug(_name+" processing");
			_evsTotal++; _cEvsTotal.fill(_evsTotal);
			_evsPerLS++; _cEvsPerLS.fill(_evsPerLS);
			_currentLS = e.luminosityBlock();
			this->_process(e, es);
		}
		catch (cms::Exception &exc)
		{
			this->_warn("We have cms::Exception Triggered. " +
				std::string(exc.what()));
		}
		catch (std::exception &exc)
		{
			this->_warn("we have STD Exception Triggered. " +
				std::string(exc.what()));
		}
		catch (...)
		{
			this->_warn("Unknown Exception Triggered");
		}
	}

	/* virtual */ void DQTask::bookHistograms(DQMStore::IBooker &ib,
		edm::Run const& r,
		edm::EventSetup const& es)
	{
		_cEvsTotal.book(ib);
		_cEvsPerLS.book(ib);
	}

	/* virtual */ void DQTask::dqmBeginRun(edm::Run const& r,
		edm::EventSetup const& es)
	{
		this->_resetMonitors(0);
		this->_resetMonitors(1);
	}

	/* virtual */ void DQTask::beginLuminosityBlock(
		edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{
		this->_resetMonitors(1);
	}

	/* virtual */ void DQTask::endLuminosityBlock(
		edm::LuminosityBlock const& lb,
		edm::EventSetup const& es)
	{

	}

	/* virtual */ void DQTask::resetMonitors(int pflag)
	{
		//	reset per event
		if (pflag==0)
		{
			
		}
		else if (pflag==1)
		{
			//	reset per LS
			_evsPerLS = 0;
		}
	}

}






