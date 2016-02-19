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
	'runFiles', [""],
	VarParsing.VarParsing.multiplicity.list,
	VarParsing.VarParsing.varType.string,
	"Run Files"
)
options.register(
	'runNumbers', 5,
	VarParsing.VarParsing.multiplicity.list,
	VarParsing.VarParsing.varType.int,
	"Run Numbers"
)

options.register(
	'processEvents',
	1,
	VarParsing.VarParsing.multiplicity.singleton,
	VarParsing.VarParsing.varType.int,
	"Number of Events to process"
)

options.register(
	'runType',
	'UNKNOWN',
	VarParsing.VarParsing.multiplicity.singleton,
	VarParsing.VarParsing.varType.string,
	"Local Run Type: pedestal, led, laser(various), raddam"
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
cmsnet			= True

print debugstr, "Input Files= ", options.inputFiles
print debugstr, "Run over #events=", options.processEvents


#-------------------------------------
#	Central DQM Stuff imports
#-------------------------------------
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(options.processEvents)
)
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
process.dqmSaver.convention = 'Offline'
process.dqmSaver.workflow = cms.untracked.string('/%s/Trend2016/DQMIO' % 
	options.runType.upper())
process.dqmSaver.referenceHandling = 'all'
process.dqmSaver.dirName = '/data/hcaldqm/DQMIO/local'
process.dqmSaver.producer = 'DQM'
process.dqmSaver.saveByLumiSection = -1
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = False
process.dqmSaver.forceRunNumber = cms.untracked.int32(1)
process.DQMStore.verbose = 0
process.dqmEnv.subSystemFolder = subsystem

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module import
#-------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load("CondCore.DBCommon.CondDBSetup_cfi")

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
#process.essourceSev = cms.ESSource(
#		"EmptyESSource",
#		recordName		= cms.string("HcalSeverityLevelComputerRcd"),
#		firstValid		= cms.vuint32(1),
#		iovIsRunNotTime	= cms.bool(True)
#)
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#-------------------------------------
#	Hcal DQM Tasks and Clients import
#-------------------------------------
process.load("DQM.Harvesting.LaserHarvesting")
process.load("DQM.Harvesting.LEDHarvesting")
process.load("DQM.Harvesting.PedestalHarvesting")

if "pedestal" in options.runType.lower():
	process.harvestingSequence = cms.Sequence(process.pedestalHarvesting)
	process.pedestalHarvesting.files = cms.untracked.vstring(options.runFiles)
	process.pedestalHarvesting.runs = cms.untracked.vstring(options.runNumbers)
elif "led" in options.runType.lower():
	process.harvestingSequence = cms.Sequence(process.ledHarvesting)
	process.ledHarvesting.files = cms.untracked.vstring(options.runFiles)
	process.ledHarvesting.runs = cms.untracked.vstring(options.runNumbers)
elif "laser" in options.runType.lower():
	process.harvestingSequence = cms.Sequence(process.laserHarvesting)
	process.laserHarvesting.files = cms.untracked.vstring(options.runFiles)
	process.laserHarvesting.runs = cms.untracked.vstring(options.runNumbers)

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
#	Execution Sequence Definition
#-------------------------------------
process.p = cms.Path(
	process.harvestingSequence
	*process.dqmEnv
	*process.dqmSaver
)
