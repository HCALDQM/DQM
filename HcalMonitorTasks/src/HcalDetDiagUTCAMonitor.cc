#include "DQM/HcalMonitorTasks/interface/HcalDetDiagUTCAMonitor.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"


HcalDetDiagUTCAMonitor::HcalDetDiagUTCAMonitor (const edm::ParameterSet& ps){
    prefixME_	= ps.getUntrackedParameter<std::string>("subSystemFolder","Hcal/");
    subdir_     = ps.getUntrackedParameter<std::string>("TaskFolder","UtcaMonitor_Hcal/"); 
    subdir_	= prefixME_+subdir_;
    vmeLabel_	= ps.getUntrackedParameter<edm::InputTag>("vmeLabel");
    utcaLabel_	= ps.getUntrackedParameter<edm::InputTag>("utcaLabel");
    splitcrateslots_ = ps.getUntrackedParameter<std::vector<int> >("splitcrateslots");

    tok_hfvme_  = consumes<HFDigiCollection>(vmeLabel_);
    tok_hfutca_ = consumes<HFDigiCollection>(utcaLabel_);
    tok_tpvme_  = consumes<HcalTrigPrimDigiCollection>(vmeLabel_);
    tok_tputca_ = consumes<HcalTrigPrimDigiCollection>(utcaLabel_);
  
    //setup histograms
}

void 
HcalDetDiagUTCAMonitor::beginRun(const edm::Run& run, const edm::EventSetup& c){

    HcalBaseDQMonitor::beginRun(run,c);

    edm::ESHandle<HcalDbService> conditions_;
    c.get<HcalDbRecord>().get(conditions_);
    emap = conditions_->getHcalMapping();

    hfDetIDs.clear();
    std::vector <HcalGenericDetId> allGIds= emap->allPrecisionId();
    for (std::vector <HcalGenericDetId>::iterator gid = allGIds.begin(); gid != allGIds.end(); ++gid){
	if(gid->genericSubdet()!=HcalGenericDetId::HcalGenForward) continue;
	HcalDetId hid(gid->rawId());
	hfDetIDs.push_back(hid);
    }

    if (!dbe_) {
	std::cout << "there is no dbe_" << std::endl;
	return;
    }

    dbe_->setCurrentFolder(subdir_);
    std::string names[5]={"Both VME and uTCA absent", "VME present uTCA absent", "VME absent uTCA present", "Both present but not matched", "Both present and matched"};
    for(int i=0; i<5; ++i){
	SetupEtaPhiHists(mStatusDG[i],names[i],	    "digis");
	mStatusTP[i] = dbe_->book2D(names[i]+" TP", names[i]+" TP", 65, -32.5, 32.5, 72, 0.5, 72.5);
    }
    std::string name;
    name="VME vs UTCA ADC Counts";	    dgVMEvsUTCA  = dbe_->book2D(name, name, 130,0,130, 130,0,130);
    name="VME vs UTCA TP Compressed ET";    tpVMEvsUTCA  = dbe_->book2D(name, name, 260,0,260, 260,0,260);

    dbe_->setCurrentFolder(subdir_ + "channel status");
    name="Bad Channels missing uTCA";		BadChannelsUTCA     = dbe_->book2D(name,name,14,0,14, NMAXCHANNELS,0,NMAXCHANNELS); 
    name="Bad Channels missing VME";		BadChannelsVME	    = dbe_->book2D(name,name,14,0,14, NMAXCHANNELS,0,NMAXCHANNELS); 
    name="Bad Channels VME-uTCA mismatches";    BadChannelsMismatch = dbe_->book2D(name,name,14,0,14, NMAXCHANNELS,0,NMAXCHANNELS); 

    std::string binlabels[14]={	"DET", "ETA", "PHI", "DEPTH", 
				"VME crate", "VME slot", "VME t/b", "VME fiber", "VME channel",  
				"uTCA crate", "uTCA slot", "uTCA fiber", "uTCA channel", 
				"Events"};

    for(int i=0; i<14; ++i) {
	 BadChannelsUTCA->setBinLabel(i+1, binlabels[i]);
	 BadChannelsVME->setBinLabel(i+1, binlabels[i]);
	 BadChannelsMismatch->setBinLabel(i+1, binlabels[i]);
    }

    name="Channels Summary missing uTCA";	 ChannelsSummaryUTCA	 = dbe_->book2D(name, "Missing uTCA", 1,0,1, 7,0,7);	    ChannelsSummaryUTCA->setBinLabel(1,    "Missing uTCA");
    name="Channels Summary missing VME";	 ChannelsSummaryVME	 = dbe_->book2D(name, "Missing VME",  1,0,1, 7,0,7);	    ChannelsSummaryVME->setBinLabel(1,  "Missing VME");
    name="Channels Summary VME-uTCA mismatches"; ChannelsSummaryMismatch = dbe_->book2D(name, "Mismatches",   1,0,1, 7,0,7);        ChannelsSummaryMismatch->setBinLabel(1,"Mismatches");
    std::string binls[7]={"HO","HF-","HF+","HE-","HE+","HB-","HB+"};
    for(int i=0; i<7; ++i) {
	 ChannelsSummaryUTCA->setBinLabel(i+1, binls[i], 2);
	 ChannelsSummaryVME->setBinLabel(i+1, "", 2);
	 ChannelsSummaryMismatch->setBinLabel(i+1, "", 2);
    }

    allEleIDs=emap->allElectronicsIdPrecision();
}

void 
HcalDetDiagUTCAMonitor::endRun(const edm::Run& run, const edm::EventSetup& c){

    for(int i=0; i<5; ++i) FillUnphysicalHEHFBins(mStatusDG[i]);
     
    int nMisingUTCA=0;
    int nMisingVME=0;
    int nMisMatched=0;

    ChannelsSummaryUTCA->Reset();
    ChannelsSummaryVME->Reset();
    ChannelsSummaryMismatch->Reset();

    double detSummaryBins[7]={6.5, 5.5, 4.5, 3.5, 2.5, 1.5, 0.5};

    for(std::vector<HcalDetId>::const_iterator hid=hfDetIDs.begin(); hid!=hfDetIDs.end(); ++hid){
	int iphi=hid->iphi();
	int ieta=hid->ieta();
	int depth=hid->depth();
	int iEta = CalcEtaBin(int(HcalForward), ieta, depth)+1;

	int kSubDet;
	bool posEta=ieta>0;
	if(hid->subdetId()==HcalBarrel)		kSubDet=posEta?0:1;
	else if (hid->subdetId()==HcalEndcap)	kSubDet=posEta?2:3;
	else if (hid->subdetId()==HcalForward)	kSubDet=posEta?4:5;
	else					kSubDet=6;
	int ybin=detSummaryBins[kSubDet];

	int missingutc=mStatusDG[1].depth[depth-1]->getBinContent(iEta, iphi);
	int missingvme=mStatusDG[2].depth[depth-1]->getBinContent(iEta, iphi);
	int mismatches=mStatusDG[3].depth[depth-1]->getBinContent(iEta, iphi);

	if(mismatches>0 && fillBadChannels(BadChannelsMismatch, nMisMatched+1, *hid, mismatches)){
	    ChannelsSummaryMismatch->Fill(0, ybin); 
	    nMisMatched++;
	}
	if(missingvme>0 && fillBadChannels(BadChannelsVME, nMisingVME+1, *hid, missingvme)){
	    ChannelsSummaryVME->Fill(0, ybin); 
	    nMisingVME++;
	}
	if(missingutc>0 && fillBadChannels(BadChannelsUTCA, nMisingUTCA+1, *hid, missingutc)){
	    ChannelsSummaryUTCA->Fill(0, ybin); 
	    nMisingUTCA++;
	}
    }
}

void HcalDetDiagUTCAMonitor::analyze(edm::Event const& e, edm::EventSetup const& s){

    edm::Handle<HFDigiCollection> hfvme;		e.getByToken(tok_hfvme_, hfvme);
    edm::Handle<HFDigiCollection> hfutca;		e.getByToken(tok_hfutca_,hfutca);
    edm::Handle<HcalTrigPrimDigiCollection> tpvme;	e.getByToken(tok_tpvme_, tpvme);
    edm::Handle<HcalTrigPrimDigiCollection> tputca;	e.getByToken(tok_tputca_,tputca);


    for(std::vector<HcalDetId>::const_iterator hid=hfDetIDs.begin(); hid!=hfDetIDs.end(); ++hid){
	int iphi=hid->iphi();
	int ieta=hid->ieta();
	int depth=hid->depth();

	HFDigiCollection::const_iterator vme  = hfvme->find(*hid);
	HFDigiCollection::const_iterator utca = hfutca->find(*hid);
	bool isVME =  vme!=hfvme->end();
	bool isUTCA= utca!=hfutca->end();
	int status = (!isVME && !isUTCA) ? 0 :
		     (isVME  && !isUTCA) ? 1 : 
		     (!isVME &&  isUTCA) ? 2 : 4;

	if(status==4){
	    if(vme->size() != utca->size()) status=3;
	    else{
		const HFDataFrame& fvme  = *vme; 
		const HFDataFrame& futca = *utca; 
		for(int s=0; s<vme->size(); ++s){
		    int vmeadc=fvme[s].adc();
		    int utcadc=futca[s].adc();
		    if(vmeadc!=utcadc) status=3;
		    if(vmeadc>0 || utcadc>0) dgVMEvsUTCA->Fill(vmeadc, utcadc);
		}
	    }
	}
	int shift=ieta>0?1:-1;	
	mStatusDG[status].depth[depth-1]->Fill(ieta+shift, iphi, 1);
    }

    std::vector<HcalTrigTowerDetId> allTTs=emap->allTriggerId();
    for(std::vector<HcalTrigTowerDetId>::const_iterator tt=allTTs.begin(); tt!=allTTs.end(); ++tt){
	int iphi=tt->iphi();
	int ieta=tt->ieta();
	HcalTrigPrimDigiCollection::const_iterator vme  = tpvme->find(*tt);
	HcalTrigPrimDigiCollection::const_iterator utca = tputca->find(*tt);
	bool isVME =  vme!=tpvme->end();
	bool isUTCA= utca!=tputca->end();
	int status = (!isVME && !isUTCA) ? 0 :
		     (isVME  && !isUTCA) ? 1 : 
		     (!isVME &&  isUTCA) ? 2 : 
		     vme->SOI_compressedEt()!=utca->SOI_compressedEt() ? 3: 4;
	if(status>=3){
	    int vmeadc=vme->SOI_compressedEt();
	    int utcadc=utca->SOI_compressedEt();
	    if(vmeadc>0 || utcadc>0) tpVMEvsUTCA->Fill(vmeadc, utcadc);
	}
	mStatusTP[status]->Fill(ieta, iphi, 1);
    }
}

bool HcalDetDiagUTCAMonitor::fillBadChannels(MonitorElement *m, int bin, HcalDetId hid, int value){

    if(bin>NMAXCHANNELS) return false;

    bool fill=false;
    for(std::vector<HcalElectronicsId>::const_iterator it=allEleIDs.begin(); it!=allEleIDs.end(); ++it){
	if(emap->lookup(*it).rawId()!=hid.rawId()) continue;
	else if(it->isVMEid()){
	    m->setBinContent(5,  bin,  it->readoutVMECrateId());
	    m->setBinContent(6,  bin,  it->htrSlot());
	    m->setBinContent(7,  bin,  it->htrTopBottom());
	    m->setBinContent(8,  bin,  it->fiberIndex());
	    m->setBinContent(9,  bin,  it->fiberChanId());
	}
	else{
	    m->setBinContent(10, bin,  it->crateId());
	    m->setBinContent(11, bin,  it->slot());
	    m->setBinContent(12, bin,  it->fiberIndex());
	    m->setBinContent(13, bin,  it->fiberChanId());
	    for(size_t i=0; i<splitcrateslots_.size(); ++i){
		int crate=splitcrateslots_[i]/100;
		int slot =splitcrateslots_[i]%100;
		if(it->crateId()==crate && it->slot()==slot) fill=true;
	    }
	}
    }
    if(fill){
	m->setBinContent(1,  bin, hid.subdet());
	m->setBinContent(2,  bin, hid.ieta());
	m->setBinContent(3,  bin, hid.iphi());
	m->setBinContent(4,  bin, hid.depth());
	m->setBinContent(14, bin, value);
    }
    return fill;
}





DEFINE_FWK_MODULE(HcalDetDiagUTCAMonitor);


