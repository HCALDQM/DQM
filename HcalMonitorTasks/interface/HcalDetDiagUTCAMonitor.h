#ifndef DQM_HCALMONITORTASKS_HCALUTCADQMONITOR_H
#define DQM_HCALMONITORTASKS_HCALUTCADQMONITOR_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DQM/HcalMonitorTasks/interface/HcalBaseDQMonitor.h"
#include "DQM/HcalMonitorTasks/interface/HcalEtaPhiHists.h"

#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"

class HcalDetDiagUTCAMonitor: public HcalBaseDQMonitor{
    public:
	HcalDetDiagUTCAMonitor(){}
	HcalDetDiagUTCAMonitor (const edm::ParameterSet& ps);
	~HcalDetDiagUTCAMonitor(){}

    protected:
	void analyze(const edm::Event& e, const edm::EventSetup& c);
	void beginRun(const edm::Run& run, const edm::EventSetup& c);
	void beginLuminosityBlock(const edm::LuminosityBlock& l, const edm::EventSetup& c){}
	void endLuminosityBlock(const edm::LuminosityBlock& l,const edm::EventSetup& c) {}
	void endRun(const edm::Run& run, const edm::EventSetup& c);
	void reset(void){}
	void cleanup(void){}
	void setup(void){}


    private:

	static const int NMAXCHANNELS=100;

	MonitorElement *BadChannelsUTCA;
	MonitorElement *BadChannelsVME;
	MonitorElement *BadChannelsMismatch;
        MonitorElement *ChannelsSummaryUTCA; 
        MonitorElement *ChannelsSummaryVME;
        MonitorElement *ChannelsSummaryMismatch;

	EtaPhiHists mStatusDG[5];
	edm::InputTag vmeLabel_;
	edm::InputTag utcaLabel_;
	edm::EDGetTokenT<HFDigiCollection> tok_hfvme_;
	edm::EDGetTokenT<HFDigiCollection> tok_hfutca_;
	edm::EDGetTokenT<HcalTrigPrimDigiCollection> tok_tpvme_;
	edm::EDGetTokenT<HcalTrigPrimDigiCollection> tok_tputca_;
	std::vector<int> splitcrateslots_;

	MonitorElement *mStatusTP[5];

	MonitorElement *dgVMEvsUTCA;
	MonitorElement *tpVMEvsUTCA;

	const HcalElectronicsMap  *emap;
	std::vector <HcalDetId> hfDetIDs;
	std::vector<HcalElectronicsId> allEleIDs;

	bool fillBadChannels(MonitorElement *m, int bin, HcalDetId hid, int value);
};

#endif
