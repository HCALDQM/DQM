#!/usr/bin/python

"""
file:			genPlots.py
author:			Viktor Khristenko
Description: 
	Produces .png files for a DQM ROOT file.
"""

#
#	Imports
#
import os, sys
import ROOT
import ImageGenerator as IG
from utilities import *

#
#	Global Vars
#
startSlicing = 4
endSlicing = 10

#
#	Extract the Run Number from FileName
#	Note: Slicing Format is predefined
#
def getRunNumber(fileName):
	v = fileName.split("_")
	runnumber = v[len(v)-1]
	runnumber = runnumber[startSlicing:endSlicing]
	return runnumber

#
#	main
#
def main():
	"""
	Entry Point into the script
	"""

	# inputs:
	#	1. DQM ROOT File Name
	#	2. Output Directory Path - a directory into which all the plots 
	#	will be saved. Directory must exist.
	dqmFileName = sys.argv[1]
	outDirPath = sys.argv[2]
	if outDirPath[len(outDirPath)-1]!="/":
		outDirPath+="/"

	# get the DQM ROOT File and a run number
	dqmFile = ROOT.TFile(dqmFileName)
	runNumber = getRunNumber(dqmFileName)

	#	set the outdir as outdir+runnumber
	#	set which directories to save = filters
	outDirPath += "%s/" % runNumber
	mkdir(outDirPath)
	filters = ["HcalLEDTask", "HcalLaserTask", "HcalPedestalTask"]
	ig = IG.ImageGenerator(dqmFile, "png", outDirPath, filters)
	ig.generate("DQMData/Run %s/Hcal/Run summary/" % runNumber)

#
#	Entry
#
if __name__=="__main__":
	main()




