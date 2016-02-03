import FWCore.ParameterSet.Config as cms

harvestingTest = cms.EDAnalyzer(
	"HarvestingTest",

	#	list of path/name of files to generate trend plots
	files = cms.untracked.vstring("DQM_V0001_Hcal_R000264052.root",
		"DQM_V0001_Hcal_R000262668.root")
)
