#-------------------------------------
#	Hcal DQM Application using New DQM Sources/Clients
#	Description: For Local Use(a la DetDiag)
#-------------------------------------

#-------------------------------------
#	Standard Python Imports
#-------------------------------------
import os, sys, socket, string

#-------------------------------------
#	Input Options
#-------------------------------------
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

options.register(
	'inputFiles',
	"root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/LS1/USC_248441.root", #default
	VarParsing.VarParsing.multiplicity.list,
	VarParsing.VarParsing.varType.string,
	"Input Files"
)

options.register(
	'processEvents',
	-1,
	VarParsing.VarParsing.multiplicity.singleton,
	VarParsing.VarParsing.varType.int,
	"Number of Events to process"
)

options.register(
	'runType',
	'UNKNOWN',
	VarParsing.VarParsing.multiplicity.singleton,
	VarParsing.VarParsing.varType.string,
	"Local Run Type: pedestal, led, laser, raddam"
)

options.parseArguments()

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
local			= True
useMap			= False
dbMap			= False
cmsnet			= False

print debugstr, "Input Files= ", options.inputFiles
print debugstr, "Run over #events=", options.processEvents


#-------------------------------------
#	Central DQM Stuff imports
#-------------------------------------
if local:
	process.source = cms.Source("HcalTBSource",
		fileNames = cms.untracked.vstring(options.inputFiles)
	)
	process.maxEvents = cms.untracked.PSet(
			input = cms.untracked.int32(options.processEvents)
	)
else:
	print errorstr + "There is an error with the Source. Exiting..."
	sys.exit(1)
process.load('DQMServices.Components.DQMEnvironment_cfi')
process.load('DQMServices.Core.DQMStore_cfi')

#-------------------------------------
#	DQM Customization
#-------------------------------------
process.DQM = cms.Service(
	"DQM",
	debug = cms.untracked.bool(False),
	publishFrequency = cms.untracked.double(1.0),
	collectorPort = cms.untracked.int32(8061),
	collectorHost = cms.untracked.string('hcaldqm03.cms'),
	filter = cms.untracked.string('')
)
process.dqmSaver.convention = 'Online'
process.dqmSaver.referenceHandling = 'all'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmSaver.saveByLumiSection = 10
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = False
process.DQMStore.verbose = 0
process.dqmEnv.subSystemFolder = subsystem

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module import
#-------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module Settings
#	-> GlobalTag
#	-> Generic Input tag for the Raw Collection
#	-> cmssw version
#	-> Turn off default blocking of dead channels from rechit collection
#	-> Drop Channel Status Bits (had benn 'HcalCellOff', "HcalCellDead")
#	-> For Trigger Primitives
#	-> L1 GT setting
#	-> Rename the hbheprereco to hbhereco
#-------------------------------------
process.GlobalTag.globaltag = '74X_dataRun2_Express_v2'
if cmsnet:
	process.GlobalTag.connect = 'frontier://(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_CONDITIONS'
cmssw			= os.getenv("CMSSW_VERSION").split("_")
rawTagStr		= "source"
rawTag			= cms.InputTag(rawTagStr)
process.essourceSev = cms.ESSource(
		"EmptyESSource",
		recordName		= cms.string("HcalSeverityLevelComputerRcd"),
		firstValid		= cms.vuint32(1),
		iovIsRunNotTime	= cms.bool(True)
)
process.hcalRecAlgos.DropChannelStatusBits = cms.vstring('')
process.valHcalTriggerPrimitiveDigis = \
		process.simHcalTriggerPrimitiveDigis.clone()
process.valHcalTriggerPrimitiveDigis.inputLabel = \
		cms.VInputTag("hcalDigis", 'hcalDigis')
process.valHcalTriggerPrimitiveDigis.FrontEndFormatError = \
		cms.bool(True)
process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(False)
process.valHcalTriggerPrimitiveDigis.FG_threshold = cms.uint32(2)
process.valHcalTriggerPrimitiveDigis.InputTagFEDRaw = rawTag
process.hbhereco = process.hbheprereco.clone()
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#-------------------------------------
#	Hcal DQM Tasks and Clients import
#-------------------------------------
process.load("DQM.HcalTasks.PedestalTask")
process.load("DQM.HcalTasks.LaserTask")
process.load("DQM.HcalTasks.LEDTask")
process.load("DQM.HcalTasks.RadDamTask")

#-------------------------------------
#	To force using uTCA
#	Will not be here for Online DQM
#-------------------------------------
if useMap==True and dbMap==True:
	process.es_pool = cms.ESSource("PoolDBESSource",
			process.CondDBSetup,
			timetype = cms.string('runnumber'),
			toGet = cms.VPSet(
				cms.PSet(
					record = cms.string(
						"HcalElectronicsMapRcd"
					),
					tag = cms.string(
						"HcalElectronicsMap_v7.00_offline"					  )
				)
			),
			connect = cms.string(
				'frontier://FrontierProd/CMS_CONDITIONS'),
			authenticationMethod = cms.untracked.uint32(0)
	)	
	process.es_prefer_es_pool = cms.ESPrefer('PoolDBESSource', 'es_pool')
elif useMap==True and dbMap==False:
	process.es_ascii = cms.ESSource(
		'HcalTextCalibrations',
		input = cms.VPSet(
			cms.PSet(
				object = cms.string('ElectronicsMap'),
				file = cms.FileInPath('version_G_emap_2015_may_20')
			)
		)
	)
	process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-------------------------------------
#	To have vme Digis as a separate collection
#-------------------------------------
process.vmeDigis = process.hcalDigis.clone()
process.vmeDigis.InputLabel = rawTag
process.vmeDigis.FEDs = cms.untracked.vint32(719, 720)

process.utcaDigis = process.hcalDigis.clone()
process.utcaDigis.InputLabel = rawTag
process.utcaDigis.FEDs = cms.untracked.vint32(1100, 1102, 1104, 1106,
	1108, 1110, 1112, 1114, 1116)

#-------------------------------------
#	Sequences Definition
#-------------------------------------
if options.runType=="pedestal":
	process.tasksSequence = cms.Sequence(process.pedestalTask)
elif options.runType=='led':
	process.tasksSequence = cms.Sequence(process.ledTask)
elif options.runType=="laser":
	process.tasksSequence = cms.Sequence(process.laserTask)
elif options.runType=="raddam":
	process.tasksSequence = cms.Sequence(process.raddamTask)
else:
	print "### Exiting. Wrong Run Type: " + options.runType
	sys.exit(0)

#-------------------------------------
#	Some Settings for Local(a la DetDiag)
#	All Modules are muted by default
#	isGlobal must be set to False!
#	Get the Local Trigger Information	
#-------------------------------------
process.hcalDigis.InputLabel = rawTag
process.pedestalTask.tagRaw = cms.untracked.InputTag('source')
process.laserTask.tagRaw = cms.untracked.InputTag('source')
process.ledTask.tagRaw = cms.untracked.InputTag('source')
process.raddamTask.tagRaw = cms.untracked.InputTag("source")
process.pedestalTask.subsystem = cms.untracked.string("Hcal")
process.laserTask.subsystem = cms.untracked.string("Hcal")
process.ledTask.subsystem = cms.untracked.string("Hcal")
process.pedestalTask.ptype = cms.untracked.int32(2)
process.ledTask.ptype = cms.untracked.int32(2)
process.laserTask.ptype = cms.untracked.int32(2)
process.raddamTask.subsystem = cms.untracked.string('Hcal')
process.raddamTask.ptype = cms.untracked.int32(2)

#-------------------------------------
#	To see Laser Board info and trigger info
#-------------------------------------
process.tbunpacker = cms.EDProducer(
	"HcalTBObjectUnpacker",
	IncludeUnmatchedHits	= cms.untracked.bool(False),
	HcalTriggerFED			= cms.untracked.int32(1)
)
process.tbunpacker.fedRawDataCollectionTag = rawTag

#-------------------------------------
#	Execution Sequence Definition
#-------------------------------------
process.p = cms.Path(
					process.tbunpacker
					*process.hcalDigis
					*process.tasksSequence
                    *process.dqmEnv
                    *process.dqmSaver
)

#-------------------------------------
#	Outputs the event Content for Debugging mostly
#-------------------------------------
#process.Out = cms.OutputModule(
#	"PoolOutputModule",
#	fileName = cms.untracked.string("test.root")
#)
#process.finalize = cms.EndPath(process.Out)

#-------------------------------------
#	Scheduling
#-------------------------------------
process.options = cms.untracked.PSet(
		Rethrow = cms.untracked.vstring(
			"ProductNotFound",
			"TooManyProducts",
			"TooFewProducts"
		)
)
