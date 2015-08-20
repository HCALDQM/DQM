import FWCore.ParameterSet.Config as cms 

import DQM.HcalCommon.HcalDQStandard as standard
StandardSet = standard.StandardSet.clone()

#	List of FEDs
lFEDs = [x+700 for x in range(32)] + [929, 1118, 1120, 1122]

moduleName = "HcaluTCATask"
#	Modify whatever is in StandardSet importing
StandardSet.moduleParameters.name		= cms.untracked.string(moduleName)
StandardSet.EventsProcessed.path		= cms.untracked.string(
	"%s/" % moduleName)
StandardSet.EventsProcessedPerLS.path	= cms.untracked.string(
	"%s/" % moduleName)
StandardSet.Standard2DMap.path			= cms.untracked.string(
	"%s/" % moduleName)
StandardSet.Standard2DMap.desc			= cms.untracked.string(
	"Some uTCA Task 2D Map")

hfVMEFEDs		= [718, 719]
hfuTCAFEDs		= [1118, 1119, 1120]
hbheVMEFEDs		= []
hbheuTCAFEDs	= []

#	Main Task Description
hcaluTCATask = cms.EDAnalyzer(
	moduleName,
	moduleParameters	= StandardSet.moduleParameters,
	MEs					= cms.untracked.PSet(
		EventsProcessed			= StandardSet.EventsProcessed,
		EventsProcessedPerLS	= StandardSet.EventsProcessedPerLS,

		#	Plots to compare uTCA vs VME
		#--------------------------------------------------
		#	HF
		#--------------------------------------------------
		HF_ADC			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HF" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string("ADC Comparison"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("VME FEDs (%s)" % hfVMEFEDs)
			),
			yaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("uTCA FEDs (%s)" % hfuTCAFEDs)
			)
		),
		HF_Samples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HF" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE Samples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# Samples")
			),
		),
		HF_PreSamples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HF" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE PreSamples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# PreSamples")
			),
		),

		#--------------------------------------------------
		#	HB
		#--------------------------------------------------
		HB_ADC			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HB" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string("ADC Comparison"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("VME FEDs (%s)" % hfVMEFEDs)
			),
			yaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("uTCA FEDs (%s)" % hfuTCAFEDs)
			)
		),
		HB_Samples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HB" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE Samples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# Samples")
			),
		),
		HB_PreSamples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HB" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE PreSamples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# PreSamples")
			),
		),

		#--------------------------------------------------
		#	HE
		#--------------------------------------------------
		HE_ADC			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HE" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string("ADC Comparison"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("VME FEDs (%s)" % hfVMEFEDs)
			),
			yaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(130),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(129.5),
				title	= cms.untracked.string("uTCA FEDs (%s)" % hfuTCAFEDs)
			)
		),
		HE_Samples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HE" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE Samples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# Samples")
			),
		),
		HE_PreSamples			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/HE" % moduleName),
			kind	= cms.untracked.string("TH1D"),
			desc	= cms.untracked.string("# QIE PreSamples"),
			xaxis	= cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(20),
				min		= cms.untracked.double(-0.5),
				max		= cms.untracked.double(19.5),
				title	= cms.untracked.string("# PreSamples")
			),
		),

		#--------------------------------------------------
		#	2D Plots
		#-------------------------------------------------
		HBHEHFD1_Matched			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string(
				"Identify which HBHEHF D1 DetId matched"),
			xaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(83),
				min		= cms.untracked.double(-41.5),
				max		= cms.untracked.double(41.5),
				title	= cms.untracked.string('ieta')
			),
			yaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(72),
				min		= cms.untracked.double(0.5),
				max		= cms.untracked.double(72.5),
				title	= cms.untracked.string('iphi')
			)
		),
		HBHEHFD2_Matched			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string(
				"Identify which HBHEHF D2 DetId matched"),
			xaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(83),
				min		= cms.untracked.double(-41.5),
				max		= cms.untracked.double(41.5),
				title	= cms.untracked.string('ieta')
			),
			yaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(72),
				min		= cms.untracked.double(0.5),
				max		= cms.untracked.double(72.5),
				title	= cms.untracked.string('iphi')
			)
		),
		HBHEHFD3_Matched			= cms.untracked.PSet(
			path	= cms.untracked.string("%s/" % moduleName),
			kind	= cms.untracked.string("TH2D"),
			desc	= cms.untracked.string(
				"Identify which HBHEHF D3 DetId matched"),
			xaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(83),
				min		= cms.untracked.double(-41.5),
				max		= cms.untracked.double(41.5),
				title	= cms.untracked.string('ieta')
			),
			yaxis = cms.untracked.PSet(
				edges	= cms.untracked.bool(False),
				nbins	= cms.untracked.int32(72),
				min		= cms.untracked.double(0.5),
				max		= cms.untracked.double(72.5),
				title	= cms.untracked.string('iphi')
			)
		),
	)
)
