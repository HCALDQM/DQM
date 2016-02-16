import FWCore.ParameterSet.Config as cms

pathToFiles = "/data/hcaldqm/DQMIO/local"

files = ["DQM_V0001_Hcal_R000260763.root",
		"DQM_V0001_Hcal_R000260782.root", "DQM_V0001_Hcal_R000260838.root",
		"DQM_V0001_Hcal_R000260930.root", "DQM_V0001_Hcal_R000261057.root",
		"DQM_V0001_Hcal_R000262040.root", "DQM_V0001_Hcal_R000262668.root",
		"DQM_V0001_Hcal_R000264052.root", "DQM_V0001_Hcal_R000264260.root",
		"DQM_V0001_Hcal_R000264509.root", "DQM_V0001_Hcal_R000264548.root",
		"DQM_V0001_Hcal_R000264737.root"]
pathfiles = []
for f in files:
	f = pathToFiles+"/"+f
	pathfiles[len(pathfiles):] = [f]
ledHarvesting = cms.EDAnalyzer(
	"LEDHarvesting",

	#	list of path/name of files to generate trend plots
#	files = cms.untracked.vstring("DQM_V0001_Hcal_R000264052.root",
#		"DQM_V0001_Hcal_R000262668.root", "DQM_V0001_Hcal_R000262040.root",
#		"DQM_V0001_Hcal_R000261057.root", "DQM_V0001_Hcal_R000260930.root",
#		"DQM_V0001_Hcal_R000260838.root", "DQM_V0001_Hcal_R000260782.root",
#		"DQM_V0001_Hcal_R000260763.root"),

	files = cms.untracked.vstring(pathfiles),
	runs = cms.untracked.vint32(260763, 260782, 260838,
		260930, 261057, 262040, 262668, 264052, 264260, 264509, 264548,
		264737)

#	runs = cms.untracked.vint32(264052, 262668, 262040,
#		261057, 260930, 260838, 260782, 260763)
)
