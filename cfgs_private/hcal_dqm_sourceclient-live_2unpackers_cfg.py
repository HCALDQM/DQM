#-------------------------------------
#	Hcal DQM Application using New DQM Sources/Clients
#	Online Mode
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
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")
process.load('CondCore.CondDB.CondDB_cfi')

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

#	init 2 emulators
process.emulTPPrim = \
		process.simHcalTriggerPrimitiveDigis.clone()
process.emulTPSec = \
		process.simHcalTriggerPrimitiveDigis.clone()
#	settings for emulators
process.emulTPPrim.inputLabel = \
		cms.VInputTag("primDigis", 'primDigis')
process.emulTPSec.inputLabel = \
		cms.VInputTag("secDigis", 'secDigis')
process.emulTPPrim.FrontEndFormatError = \
		cms.bool(True)
process.emulTPSec.FrontEndFormatError = \
		cms.bool(True)
process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(False)
process.emulTPPrim.FG_threshold = cms.uint32(2)
process.emulTPPrim.InputTagFEDRaw = rawTag
process.emulTPSec.FG_threshold = cms.uint32(2)
process.emulTPSec.InputTagFEDRaw = rawTag

#	set the tag for default unpacker
process.hcalDigis.InputLabel = rawTag

#-------------------------------------
#	Hcal DQM Tasks and Clients import
#	New Style
#-------------------------------------
process.load("DQM.HcalTasks.RawTask")
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
				file = cms.FileInPath('version_G_emap_all_24feb.txt')
#				file = cms.FileInPath('version_G_emap_all_16feb.txt')
			#	file = cms.FileInPath('emap_all_16022016.txt')
#				file = cms.FileInPath('version_G_emap_all.txt')
			)
		)
	)
	process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-------------------------------------
#	For Debugginb
#-------------------------------------

#-------------------------------------
#	prep 2 unpackers
#-------------------------------------
process.primDigis = process.hcalDigis.clone()
process.primDigis.InputLabel = rawTag
primFEDs = [x*2+1100 for x in range(9)]
primFEDs[len(primFEDs):] = [x+724 for x in range(8)]
primFEDs[len(primFEDs):] = [1118, 1120, 1122]
print "Primary FEDs to be Unpacked:", primFEDs
process.primDigis.FEDs = cms.untracked.vint32(primFEDs)

process.secDigis = process.hcalDigis.clone()
process.secDigis.InputLabel = rawTag
secFEDs = [x+700 for x in range(18)]
print "Secondary FEDs to be Unpacked:", secFEDs
process.secDigis.FEDs = cms.untracked.vint32(secFEDs)

process.secDigiTask = process.digiTask.clone()
process.secDigiTask.name = cms.untracked.string("secDigiTask")
process.secDigiTask.tagHBHE = cms.untracked.InputTag('secDigis')
process.secDigiTask.tagHO = cms.untracked.InputTag('secDigis')
process.secDigiTask.tagHF = cms.untracked.InputTag('secDigis')

process.secTPTask = process.tpTask.clone()
process.secTPTask.name = cms.untracked.string('secTPTask')
process.secTPTask.tagData = cms.untracked.InputTag("secDigis")
process.secTPTask.tagEmul = cms.untracked.InputTag('emulTPSec')

process.digiComparisonTask.tagHBHE1 = cms.untracked.InputTag('primDigis')
process.digiComparisonTask.tagHBHE2 = cms.untracked.InputTag('secDigis')
process.tpComparisonTask.tag1 = cms.untracked.InputTag('primDigis')
process.tpComparisonTask.tag2 = cms.untracked.InputTag('secDigis')

#-------------------------------------
#	Settigns for the Primary Modules
#-------------------------------------
oldsubsystem = subsystem
process.rawTask.tagFEDs = rawTagUntracked
process.rawTask.runkeyVal = runType
process.rawTask.runkeyName = runTypeName

process.digiTask.tagHBHE = cms.untracked.InputTag('primDigis')
process.digiTask.tagHO = cms.untracked.InputTag('primDigis')
process.digiTask.tagHF = cms.untracked.InputTag('primDigis')
process.digiTask.runkeyVal = runType
process.digiTask.runkeyName = runTypeName

process.tpTask.runkeyVal = runType
process.tpTask.runkeyName = runTypeName
process.tpTask.tagData = cms.untracked.InputTag("primDigis")
process.tpTask.tagEmul = cms.untracked.InputTag("emulTPPrim")

#-------------------------------------
#	Hcal DQM Tasks/Clients Sequences Definition
#-------------------------------------
process.tasksSequence = cms.Sequence(
		process.digiTask
		+process.tpTask
		+process.secDigiTask
		+process.secTPTask
		+process.digiComparisonTask
		+process.tpComparisonTask
)

#-------------------------------------
#	Paths/Sequences Definitions
#-------------------------------------
process.preRecoSequence = cms.Sequence(
		process.primDigis
		*process.secDigis
)

process.recoSequence = cms.Sequence(
		process.emulTPPrim
		+process.emulTPSec
)

process.dqmSequence = cms.Sequence(
		process.dqmEnv
		*process.dqmSaver
)

process.p = cms.Path(
		process.preRecoSequence
		*process.recoSequence
		*process.tasksSequence
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
