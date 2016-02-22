#-------------------------------------
#	Hcal DQM Application using New DQM Sources/Clients
#	Old Modules are being run as well.
#	Offline Mode
#-------------------------------------

#-------------------------------------
#	Standard Python Imports
#-------------------------------------
import os, sys, socket, string

#-------------------------------------
#	Standard CMSSW Imports/Definitions
#-------------------------------------
import FWCore.ParameterSet.Config as cms
process			= cms.Process('HCALDQM')
subsystem		= 'Hcal'
cmssw			= os.getenv("CMSSW_VERSION").split("_")
debugstr		= "### HcalDQM::cfg::DEBUG: "
warnstr			= "### HcalDQM::cfg::WARN: "
errorstr		= "### HcalDQM::cfg::ERROR:"
useOfflineGT	= False
useFileInput	= False
useMapDB		= False
useMapText		= True

#-------------------------------------
#	Central DQM Stuff imports
#-------------------------------------
from DQM.Integration.config.online_customizations_cfi import *
if useOfflineGT:
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
	process.GlobalTag.globaltag = '74X_dataRun2_HLT_v1'
else:
	process.load('DQM.Integration.config.FrontierCondition_GT_cfi')
if useFileInput:
	process.load("DQM.Integration.config.fileinputsource_cfi")
else:
	process.load('DQM.Integration.config.inputsource_cfi')
process.load('DQMServices.Components.DQMEnvironment_cfi')
process.load('DQM.Integration.config.environment_cfi')

#-------------------------------------
#	Central DQM Customization
#-------------------------------------
process.dqmEnv.subSystemFolder = subsystem
process.dqmSaver.tag = subsystem
referenceFileName = '/dqmdata/dqm/reference/hcal_reference.root'
process.DQMStore.referenceFileName = referenceFileName
process = customise(process)
process.DQMStore.verbose = 0
process.DQM.collectorPort = cms.untracked.int32(9190)
process.DQM.collectorHost = cms.untracked.string("fu-c2f11-21-03.cms")
process.dqmSaver.path = "/data/hcaldqm/DQMIO/ONLINE_PLAYBACK"

#	Note, runType is obtained after importing DQM-related modules
#	=> DQM-dependent
runType			= process.runType.getRunType()
print debugstr, "Running with run type= ", runType

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module import
#-------------------------------------
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("L1Trigger.Configuration.L1DummyConfig_cff")
process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module Settings
#	-> runType
#	-> Generic Input tag for the Raw Collection
#	-> cmssw version
#	-> Turn off default blocking of dead channels from rechit collection
#	-> Drop Channel Status Bits (had benn 'HcalCellOff', "HcalCellDead")
#	-> For Trigger Primitives Emulation
#	-> L1 GT setting
#	-> Rename the hbheprereco to hbhereco
#-------------------------------------
runType			= process.runType.getRunType()
runTypeName		= process.runType.getRunTypeName()
isCosmicRun     = runTypeName=="cosmic_run" or runTypeName=="cosmic_run_stage1"
isHeavyIon      = runTypeName=="hi_run"
cmssw			= os.getenv("CMSSW_VERSION").split("_")
rawTag			= cms.InputTag("rawDataCollector")
rawTagUntracked	= cms.untracked.InputTag("rawDataCollector")
if isHeavyIon:
	rawTag			= cms.InputTag("rawDataRepacker")
	rawTagUntracked	= cms.untracked.InputTag("rawDataRepacker")

process.essourceSev = cms.ESSource(
		"EmptyESSource",
		recordName		= cms.string("HcalSeverityLevelComputerRcd"),
		firstValid		= cms.vuint32(1),
		iovIsRunNotTime	= cms.bool(True)
)
process.hcalRecAlgos.DropChannelStatusBits = cms.vstring('')
process.emulTPDigis = \
		process.simHcalTriggerPrimitiveDigis.clone()
process.emulTPDigis.inputLabel = \
		cms.VInputTag("hcalDigis", 'hcalDigis')
process.emulTPDigis.FrontEndFormatError = \
		cms.bool(True)
process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(False)
process.emulTPDigis.FG_threshold = cms.uint32(2)
process.emulTPDigis.InputTagFEDRaw = rawTag
process.l1GtUnpack.DaqGtInputTag = rawTag
process.hbhereco = process.hbheprereco.clone()
process.hcalDigis.InputLabel = rawTag

#-------------------------------------
#	Hcal DQM Tasks and Clients import
#	New Style
#-------------------------------------
process.load("DQM.HcalTasks.RawTask")
process.load("DQM.HcalTasks.RecHitTask")
process.load("DQM.HcalTasks.DigiTask")
process.load('DQM.HcalTasks.TPTask')
process.load('DQM.SpecificTasks.DigiComparisonTask')
process.load('DQM.SpecificTasks.TPComparisonTask')

#-------------------------------------
#	To force using uTCA
#	Will not be here for Online DQM
#-------------------------------------
if useMapDB:
	process.es_pool = cms.ESSource("PoolDBESSource",
			process.CondDBSetup,
			timetype = cms.string('runnumber'),
			toGet = cms.VPSet(
				cms.PSet(
					record = cms.string(
						"HcalElectronicsMapRcd"
					),
					tag = cms.string(
						"HcalElectronicsMap_v7.05_hlt"
					)
				)
			),
			connect = cms.string(
				'frontier://FrontierProd/CMS_CONDITIONS'),
			authenticationMethod = cms.untracked.uint32(0)
	)	
	process.es_prefer_es_pool = cms.ESPrefer('PoolDBESSource', 'es_pool')

if useMapText:
	process.es_ascii = cms.ESSource('HcalTextCalibrations',
		input = cms.VPSet(
			cms.PSet(
				object = cms.string('ElectronicsMap'),
				file = cms.FileInPath('version_G_emap_all_16feb.txt')
			#	file = cms.FileInPath('emap_all_16022016.txt')
#				file = cms.FileInPath('version_G_emap_all.txt')
			)
		)
	)
	process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-------------------------------------
#	For Debugginb
#-------------------------------------
#process.hcalDigiTask.moduleParameters.debug = 10

#-------------------------------------
#	Some Settings before Finishing up
#	New Style Modules
#-------------------------------------
oldsubsystem = subsystem
process.rawTask.tagFEDs = rawTagUntracked
process.digiTask.runkeyVal = runType
process.digiTask.runkeyName = runTypeName
process.rawTask.runkeyVal = runType
process.rawTask.runkeyName = runTypeName
process.recHitTask.runkeyVal = runType
process.recHitTask.runkeyName = runTypeName
process.tpTask.runkeyVal = runType
process.tpTask.runkeyName = runTypeName

#-------------------------------------
#	Prep uHBHE FEDs
#	Clone the unpacker first
#	then clone the analyzers
#-------------------------------------
process.uHBHEDigis = process.hcalDigis.clone()
process.uHBHEDigis.InputLabel = rawTag

process.uHBHEDigis.FEDs = cms.untracked.vint32(1100, 1102, 1104, 1106, 1108,
	1110, 1112, 1114, 1116)
process.uHBHEdigiTask = process.digiTask.clone()
process.uHBHEdigiTask.name = cms.untracked.string("uHBHEDigiTask")
process.uHBHEdigiTask.tagHBHE = cms.untracked.InputTag('uHBHEDigis')
process.uHBHEdigiTask.tagHO = cms.untracked.InputTag('uHBHEDigis')
process.uHBHEdigiTask.tagHF = cms.untracked.InputTag('uHBHEDigis')

process.uHBHEtpTask = process.tpTask.clone()
process.uHBHEtpTask.name = cms.untracked.string('uHBHETPTask')
process.uHBHEtpTask.tagData = cms.untracked.InputTag("uHBHEDigis")

#-------------------------------------
#	Hcal DQM Tasks/Clients Sequences Definition
#-------------------------------------
process.tasksSequence = cms.Sequence(
		process.recHitTask
		+process.digiTask
		+process.tpTask
		+process.uHBHEdigiTask
		+process.uHBHEtpTask
		+process.digiComparisonTask
		+process.tpComparisonTask
#		process.hcalDigiTask
#		+process.hcalRawTask
#		+process.hcalRecHitTask
#		+process.hcalTPTask
#		+process.hcalTimingTask
#		+process.hcalMonitor
#		+process.hcalMonitorTasksOnlineSequence
)

#process.clientsSequence = cms.Sequence(
#	process.hcalClient
#)

#-------------------------------------
#	Quality Tester
#-------------------------------------
process.qTester = cms.EDAnalyzer(
	"QualityTester",
	prescaleFactor = cms.untracked.int32(1),
	qtList = cms.untracked.FileInPath(
		"DQM/HcalMonitorClient/data/hcal_qualitytest_config.xml"),
	getQualityTestsFromFile = cms.untracked.bool(True),
	qtestOnEndLumi = cms.untracked.bool(True),
	qtestOnEndRun = cms.untracked.bool(True)
)

#-------------------------------------
#	Paths/Sequences Definitions
#-------------------------------------
process.preRecoSequence = cms.Sequence(
		process.hcalDigis
		*process.uHBHEDigis
#		*process.l1GtUnpack
)

process.recoSequence = cms.Sequence(
		process.emulTPDigis
		+process.hfreco
		+process.hbhereco
		+process.horeco
)

process.dqmSequence = cms.Sequence(
		process.dqmEnv
		*process.dqmSaver
)

process.p = cms.Path(
		process.preRecoSequence
		*process.recoSequence
		*process.tasksSequence
#		*process.clientsSequence
		*process.qTester
		*process.dqmSequence
)

#process.schedule = cms.Schedule(process.p)

#-------------------------------------
#	Scheduling and Process Customizations
#-------------------------------------
process.options = cms.untracked.PSet(
		Rethrow = cms.untracked.vstring(
			"ProductNotFound",
			"TooManyProducts",
			"TooFewProducts"
		)
)
process.options.wantSummary = cms.untracked.bool(True)
