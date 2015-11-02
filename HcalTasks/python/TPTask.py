import FWCore.ParameterSet.Config as cms

tpTask = cms.EDAnalyzer(
	"TPTask",
	
	#	standard parameters
	name = cms.untracked.string("TPTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),

	#	tags
	tagData = cms.untracked.InputTag("hcalDigis"),
	tagEmul = cms.untracked.InputTag("emulTPDigis")
)











