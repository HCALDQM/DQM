import FWCore.ParameterSet.Config as cms

rawTask = cms.EDAnalyzer(
	"RawTask",
	
	#	standard parameters
	name = cms.untracked.string("RawTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),

	#	tags
	tagFEDs = cms.untracked.InputTag("rawDataCollector")
)












