#-----------------
#	HCAL DQM Offline Sequence Definition
#	To be used for Offline DQM importing
#-----------------

#	import the tasks
process.load('DQM.HcalTasks.DigiTask')
process.load('DQM.HcalTasks.RawTask')
process.load('DQM.HcalTasks.TPTask')
process.load('DQM.HcalTasks.RecHitTask')

#	set processing type to Offine
process.digiTask.ptype = cms.untracked.int32(1)
process.tpTask.ptype = cms.untracked.int32(1)
process.recHitTask.ptype = cms.untracked.int32(1)
process.rawTask.ptype = cms.untracked.int32(1)

hcalOfflineSequence = cms.Sequence(
	process.digiTask
	+process.tpTask
	+process.recHitTask
	process.rawTask
)

