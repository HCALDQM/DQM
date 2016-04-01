import FWCore.ParameterSet.Config as cms

path = "/data/hcaldqm/DQMIO/LOCAL/"
basefilename = "DQM_V0001_R000%d__PEDESTAL__Commissioning2016__DQMIO.root"
runs = [260781, 260836, 260928, 261048, 262042, 262665, 264054]
filenames = [path+(basefilename%x) for x in runs]

pedestalHarvesting = cms.EDAnalyzer(
	"PedestalHarvesting",
	name = cms.untracked.string("PedestalHarvesting"),
	debug = cms.untracked.int32(0),
	runkeyVal = cms.untracked.int32(0),
	runkeyName = cms.untracked.string("pp_run"),
	ptype = cms.untracked.int32(2),
	subsystem = cms.untracked.string("Hcal"),

	files = cms.untracked.vstring(filenames),
	runs = cms.untracked.vint32(runs)
)
