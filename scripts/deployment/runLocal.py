#!/usr/bin/python

#------------------------------------------------------------------
#	Local HCALDQM (a la DetDiag) Wrapper Script.
#	Do the main imports now
#------------------------------------------------------------------
from os import listdir
from os.path import isfile, join
from runManager import RunManager
from subprocess import call

#------------------------------------------------------------------
#	Define the Global Constants
#------------------------------------------------------------------
username			= "hcaldqm"
userDir				= "/nfshome0/%s/" % username
cmsswSrcDir			= userDir + "CMSSW/src/"
cfg					= "new_hcal_dqm_Local.py"
dqmInCMSSW			= "DQM/Integration/python/test/%s" % cfg
cmsswCfg			= cmsswSrcDir + dqmInCMSSW
eoslocalfile		= "file:"
mailList			= "vkhriste@cern.ch"
dataSourceDir		= "/hcaldepot1/data/"
logFilePerRun		= "/tmp/log."
logFile				= "/tmp/log.runLocal"
runInfo				= userDir + 

#------------------------------------------------------------------
#	Auxillary Functions
#------------------------------------------------------------------
def listRuns(dirName):
	files = [join(dirName, f) 
			for f in listdir(dirName) if isfile(join(dirName, f))]
	return files

#------------------------------------------------------------------
#	Starts the Actual Execution
#------------------------------------------------------------------
def runLocal():
	myRunManager = RunManager()
	for inputFile in listRuns(dataSourceDir):
		#	If this run has been executed, continue
		if myRunManager.exists(inputFile):
			continue

		#	For new Runs, process them
		runNum = myRunManager.runNumber(inputFile)
		call("cmsRun %s inputFiles=%s" % (
			cmsswCfg, )
		)


if __name__=="__main__":
	runLocal()












