import FWCore.ParameterSet.Config as cms

#----------------------------------------
#	Import Standard Set
#----------------------------------------
import DQM.HcalCommon.HcalDQStandard as standard
StandardSet = standard.StandardSet.clone()

#----------------------------------------
#	Configure
#----------------------------------------
moduleName = "HcalSummaryTask"
StandardSet.moduleParameters.name = cms.untracked.string(moduleName)
StandardSet.EventsProcessed.path = cms.untracked.string("%s/" % moduleName)
StandardSet.EventsProcessedPerLS.path = cms.untracked.string(
	"%s/" % moduleName)


#----------------------------------------
#	Declare the EDAnalyzer itself
#----------------------------------------
hcalSummaryTask = cms.EDAnalyzer(
		moduleName,
		moduleParameters = StandardSet.moduleParameters,
		MEs = cms.untracked.PSet(
			EventsProcessed				= StandardSet.EventsProcessed,
			EventsProcessedPerLS		= StandardSet.EventsProcessedPerLS,
			DcsStatus					= cms.untracked.PSet(
				path	= cms.untracked.string("%s" % moduleName),
				kind	= cms.untracked.string("TH2D"),
				desc	= cms.untracked.string("Dcs Status"),
				xaxis	= cms.untracked.PSet(
					edges	= cms.untracked.bool(False),
					nbins	= cms.untracked.int32(4),
					min		= cms.untracked.double(-0.5),
					max		= cms.untracked.double(3.5),
					title	= cms.untracked.string("Sub Detector")
				),
				xaxis	= cms.untracked.PSet(
					edges	= cms.untracked.bool(False),
					nbins	= cms.untracked.int32(1),
					min		= cms.untracked.double(-0.5),
					max		= cms.untracked.double(0.5),
					title	= cms.untracked.string("Sub Detector")
				)
			)
		)
)



