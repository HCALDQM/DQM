import FWCore.ParameterSet.Config as cms
import os
import string
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('analysis')
options.register('inputfiles',	'', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, '') 
options.parseArguments()
print options.inputfiles
inputfiles=[f for f in options.inputfiles.split(",")]
print inputfiles

process = cms.Process("HCALDQM")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
)

process.source = cms.Source("HcalTBSource",
        fileNames = cms.untracked.vstring(inputfiles),
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.load("DQMServices.Core.DQMStore_cfi")
process.DQM = cms.Service("DQM",
		  debug = cms.untracked.bool(False),
                  publishFrequency = cms.untracked.double(1.0),
                  collectorPort = cms.untracked.int32(8061),
                  collectorHost = cms.untracked.string('hcaldqm03.cms'),
                  filter = cms.untracked.string('')
                  )      

process.dqmSaver.convention = 'Online'
process.dqmSaver.referenceHandling = 'all'
process.dqmSaver.dirName = 'outputDQM'
process.dqmSaver.producer = 'DQM'
process.dqmSaver.saveByLumiSection = -1
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = False
process.DQMStore.verbose    = 0
process.dqmEnv.subSystemFolder = 'Hcal'

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi") 
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.connect = "frontier://(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_CONDITIONS"
process.GlobalTag.globaltag = "GR_H_V38"
#process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://localhost:8000/FrontierOnProd)(serverurl=http://localhost:8000/FrontierOnProd)(retrieve-ziplevel=0)(failovertoserver=no)/CMS_CONDITIONS"
#process.GlobalTag.globaltag = "GR_P_V49"
#process.es_prefer_GlobalTag = cms.ESPrefer("PoolDBESSource", "GlobalTag")

process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("DQM.HcalMonitorModule.HcalMonitorModule_cfi")
process.load("DQM.HcalMonitorTasks.HcalMonitorTasks_cfi")
process.load("DQM.HcalMonitorClient.HcalMonitorClient_cfi")

process.hcalClient.debug=0
process.hcalClient.baseHtmlDir='outputHTML/'
process.hcalClient.databaseDir=''
process.hcalClient.minevents=1
process.hcalDetDiagLaserMonitor.LaserReferenceData       = 'Reference/HcalDetDiagLaserData_run191238_1.root'
process.hcalDetDiagPedestalMonitor.PedestalReferenceData = 'Reference/HcalDetDiagPedestalData_run225529_1.root'  
process.hcalDetDiagLEDMonitor.LEDReferenceData           = 'Reference/HcalDetDiagLEDData_run225538_1.root'	
process.hcalDetDiagLaserMonitor.OutputFilePath           = 'outputDATA/'	
process.hcalDetDiagPedestalMonitor.OutputFilePath        = 'outputDATA/'
process.hcalDetDiagLEDMonitor.OutputFilePath             = 'outputDATA/'	
process.hcalDetDiagLaserMonitor.XmlFilePath              = 'outputDATA/'	
process.hcalDetDiagPedestalMonitor.XmlFilePath           = 'outputDATA/'
process.hcalDetDiagLEDMonitor.XmlFilePath                = 'outputDATA/'
process.hcalDetDiagLaserMonitor.Overwrite=False
process.hcalDetDiagPedestalMonitor.Overwrite=False

process.tbunpack = cms.EDProducer("HcalTBObjectUnpacker",
    IncludeUnmatchedHits = cms.untracked.bool(False),
    HcalTriggerFED       = cms.untracked.int32(1)
)

process.tbunpack.fedRawDataCollectionTag = cms.InputTag("source")
process.hcalDigis.InputLabel="source"
process.hcalMonitor.FEDRawDataCollection="source"
process.hcalDetDiagNoiseMonitor.RawDataLabel="source"
process.hcalDetDiagPedestalMonitor.rawDataLabel="source"
process.hcalRawDataMonitor.FEDRawDataCollection="source"
process.hcalClient.enabledClients = cms.untracked.vstring(  "DetDiagPedestalMonitor",
							    "DetDiagLaserMonitor",
                                                            "DetDiagLEDMonitor",
                                                            #"Summary"
							    )

process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
#            file = cms.FileInPath('version_E_emap_both_jm_8oct.txt')
#            file = cms.FileInPath('version_G_emap.txt')
            file = cms.FileInPath('version_G_emap_March10.txt')
            ),
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')


process.hcalDetDiagUTCAMonitor=cms.EDAnalyzer("HcalDetDiagUTCAMonitor",
            subSystemFolder        = cms.untracked.string("Hcal/"),
            TaskFolder             = cms.untracked.string("DetDiagUTCAMonitor_Hcal/"),
            vmeLabel               = cms.untracked.InputTag("hcalDigis"),
            utcaLabel              = cms.untracked.InputTag("utcaDigis"),
	    splitcrateslots	   = cms.untracked.vint32(2911, 2912) ### 100xUtcaCrateID+Slot, only these will be considered bad in channels summary plots.
        )


process.utcaDigis       = process.hcalDigis.clone()
process.utcaDigis.FEDs  = cms.untracked.vint32(929, 1118, 1120, 1122)

process.hcalMonitorTasksCalibrationSequence=cms.Sequence( process.hcalDetDiagPedestalMonitor
							 +process.hcalDetDiagLaserMonitor
							 +process.hcalDetDiagLEDMonitor
							 +process.hcalDetDiagUTCAMonitor
                                                         )

# to Make uTCA to be the default 
process.hcalDetDiagPedestalMonitor.digiLabelHF = cms.untracked.InputTag("hcalDigis")
process.hcalDetDiagLaserMonitor.digiLabelHF = cms.untracked.InputTag("hcalDigis")
process.hcalDetDiagLEDMonitor.digiLabelHF = cms.untracked.InputTag("hcalDigis")
defaultUTCA = 1
if defaultUTCA==1:
  process.hcalDetDiagPedestalMonitor.digiLabelHF = cms.untracked.InputTag("utcaDigis")
  process.hcalDetDiagLaserMonitor.digiLabelHF = cms.untracked.InputTag("utcaDigis")
  process.hcalDetDiagLEDMonitor.digiLabelHF = cms.untracked.InputTag("utcaDigis")

process.p = cms.Path(  process.tbunpack
                      *process.hcalDigis
                      *process.utcaDigis
                      *process.hcalMonitor
                      *process.hcalMonitorTasksCalibrationSequence
                      *process.hcalClient
                      *process.dqmEnv
                      *process.dqmSaver
)
