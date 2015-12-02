import FWCore.ParameterSet.Config as cms

tpTask = cms.EDAnalyzer(
	"TPTask",
	
	#	standard parameters
	name = cms.untracked.string("TPTask"),
	debug = cms.untracked.int32(10),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(0),
	mtype = cms.untracked.bool(1),
	ctype = cms.untracked.bool(0),
	subsystem = cms.untracked.string("HcalCalib"),

	#	tags
	tagData = cms.untracked.InputTag("hcalDigis"),
	tagEmul = cms.untracked.InputTag("emulTPDigis"),

	#	some speacial features
	skip1x1 = cms.untracked.bool(True)
)












