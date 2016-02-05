import FWCore.ParameterSet.Config as cms

hbmruns = [260918, 261050, 262031, 262398, 262670]
hbmfiles = ["DQM_V0001_Hcal_R000260918.root", "DQM_V0001_Hcal_R000261050.root",
	"DQM_V0001_Hcal_R000262031.root", "DQM_V0001_Hcal_R000262398.root",
	"DQM_V0001_Hcal_R000262670.root"]

hbpruns = [260919, 261053, 262032, 262399, 262673]
hbpfiles = ["DQM_V0001_Hcal_R000260919.root", "DQM_V0001_Hcal_R000261053.root",
	"DQM_V0001_Hcal_R000262032.root", "DQM_V0001_Hcal_R000262399.root",
	"DQM_V0001_Hcal_R000262673.root"]

hemruns = [260921, 261054, 262035, 262400]
hemfiles = ["DQM_V0001_Hcal_R000260921.root", "DQM_V0001_Hcal_R000261054.root",
	"DQM_V0001_Hcal_R000262035.root", "DQM_V0001_Hcal_R000262400.root"]

hepruns = [260926, 261055, 262039, 262401]
hepfiles = ["DQM_V0001_Hcal_R000260926.root", "DQM_V0001_Hcal_R000261055.root",
	"DQM_V0001_Hcal_R000262039.root", "DQM_V0001_Hcal_R000262401.root"]

laserHarvesting = cms.EDAnalyzer(
	"LaserHarvesting",

	#	list of path/name of files to generate trend plots
	files = cms.untracked.vstring(hepfiles),

	runs = cms.untracked.vint32(hepruns)
)
