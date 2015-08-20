import FWCore.ParameterSet.Config as cms

#------------------------------------------------------------
#	Import standard cfg things and redefine what's neccessary
#------------------------------------------------------------
import DQM.HcalCommon.HcalDQStandard as standard
StandardSet = standard.StandardSet.clone()

subsystem = 'Hcal'
tc = 'Client'
mn = "HcalDigi"
moduleName = mn + tc
taskName = mn + "Task"
StandardSet.moduleParameters.name = cms.untracked.string(moduleName)
StandardSet.moduleParameters.subsystem = cms.untracked.string(subsystem)
StandardSet.mtype = cms.untracked.string("CLIENT")
StandardSet.EventsProcessed.path = cms.untracked.string("%s/" % taskName)
StandardSet.EventsProcessedPerLS.path = cms.untracked.string("%s/" % taskName)

#------------------------------------------------------------
#	Declare the actual client
#------------------------------------------------------------
hcalDigiClient = cms.EDAnalyzer(
	moduleName,
	moduleParameters = StandardSet.moduleParameters,
	bookMEs		= cms.untracked.PSet(

	),
	retrieveMEs = cms.untracked.PSet(
		EventsProcessed				= StandardSet.EventsProcessed,
		EventsProcessedPerLS		= StandardSet.EventsProcessedPerLS
	)
)



