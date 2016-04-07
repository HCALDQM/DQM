import FWCore.ParameterSet.Config as cms

path = "/data/hcaldqm/DQMIO/LOCAL/"
basefilename = "DQM_V0001_R000%d__LASER__Commissioning2016__DQMIO.root"
runs = cms.untracked.vint32(260763, 260782, 260838,
		260930, 261057, 262040, 262668, 264052, 264260, 264509, 264548,
		264737)
filenames = [path+(basefilename%x) for x in runs]

laserHarvesting = cms.EDAnalyzer(
	"LaserHarvesting",
	name = cms.untracked.string("LaserHarvesting"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(2),
	subsystem = cms.untracked.string("Hcal"),

	files = cms.untracked.vstring(filenames),
	runs = cms.untracked.vint32(runs)
)
