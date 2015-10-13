import FWCore.ParameterSet.Config as cms

recHitTask = cms.EDAnalyzer(
	"RecHitTask",
	
	#	standard parameters
	name = cms.untracked.string("RecHitTask"),
	debug = cms.untracked.int(10),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),

	#	tags
	tagHBHE = cms.untracked.InputTag("hbhreco"),
	tagHO = cms.untracked.InputTag("horeco"),
	tagHF = cms.untracked.InputTag("hfreco")
)












