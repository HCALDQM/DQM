import FWCore.ParameterSet.Config as cms

qie10TestTask = cms.EDAnalyzer(
	"QIE10TestTask",

	#	standard
	name = cms.untracked.string("QIE10TestTask"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),

	tagQIE10 = cms.untracked.InputTag("qie10Digis")
)
