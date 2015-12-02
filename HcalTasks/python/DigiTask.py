import FWCore.ParameterSet.Config as cms

digiTask = cms.EDAnalyzer(
	"DigiTask",
	
	#	standard parameters
	name = cms.untracked.string("DigiTask"),
	debug = cms.untracked.int32(10),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(0),
	mtype = cms.untracked.bool(1),
	subsystem = cms.untracked.string("Hcal"),

	#	tags
	tagHBHE = cms.untracked.InputTag("hcalDigis"),
	tagHO = cms.untracked.InputTag("hcalDigis"),
	tagHF = cms.untracked.InputTag("hcalDigis"),

	#	Cuts
	cutsumQ = cms.untracked.double(20)
)












