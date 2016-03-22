import FWCore.ParameterSet.Config as cms

pedestalHarvesting = cms.EDAnalyzer(
	"PedestalHarvesting",

	files = cms.untracked.vstring(
		"DQM_V0001_Hcal_R000260781.root", "DQM_V0001_Hcal_R000260836.root",
		"DQM_V0001_Hcal_R000260928.root", "DQM_V0001_Hcal_R000261048.root",
		"DQM_V0001_Hcal_R000262042.root", "DQM_V0001_Hcal_R000262665.root",
		"DQM_V0001_Hcal_R000264054.root"
	),
	runs = cms.untracked.vint32(
		260781, 260836, 260928, 261048, 262042, 262665, 264054
	)
)
