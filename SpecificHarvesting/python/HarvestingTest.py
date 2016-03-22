import FWCore.ParameterSet.Config as cms

harvestingTest = cms.EDAnalyzer(
	"HarvestingTest",

	#	list of path/name of files to generate trend plots
	files = cms.untracked.vstring("DQM_V0001_Hcal_R000264052.root",
		"DQM_V0001_Hcal_R000262668.root", "DQM_V0001_Hcal_R000262040.root",
		"DQM_V0001_Hcal_R000261057.root", "DQM_V0001_Hcal_R000260930.root",
		"DQM_V0001_Hcal_R000260838.root", "DQM_V0001_Hcal_R000260782.root",
		"DQM_V0001_Hcal_R000260763.root"),

	runs = cms.untracked.vint32(264052, 262668, 262040,
		261057, 260930, 260838, 260782, 260763)
)
