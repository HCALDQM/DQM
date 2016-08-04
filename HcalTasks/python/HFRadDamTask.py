import FWCore.ParameterSet.Config as cms

hfRaddamTask = cms.EDAnalyzer(
	"HFRadDamTask",
	
	#	standard parameters
	name = cms.untracked.string("HFRadDamTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(0),
	mtype = cms.untracked.bool(True),
	subsystem = cms.untracked.string('HcalCalib'),

	#	tags
	tagHF = cms.untracked.InputTag("hcalDigis"),
	tagRaw = cms.untracked.InputTag('hltHcalCalibrationRaw'),
        laserType = cms.untracked.uint32(0)
)












