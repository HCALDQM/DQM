#!/usr/bin/python

"""
file:				RunInfoDBWrapper.py
Author:				Viktor Khristenko
"""

#
#	Imports
#
import os, sys, subprocess
import utilities
import xml.etree.ElementTree as ET
from xml.dom import minidom

def prettify(elem):
	"""Return a pretty-printed XML string for the Element."""
	rough_string = ET.tostring(elem, 'utf-8')
	reparsed = minidom.parseString(rough_string)
	return reparsed.toprettyxml(indent="  ")

#
#	A Simple Run Info Wrapper
#
class RunInfoDBWrapper:
	def __init__(self, runNumber, pathToSave, templ):
		#	inputs
		self.runNumber = runNumber
		self.pathToSave = pathToSave
		self.templ = templ

		#	Set up DB things
		self.lNames = {"NEVENTS" : "CMS.HCAL%:EventsRequested", 
				"CONFIGURATION" : "CMS.HCAL%:FM_FULLPATH"}
		self.lSels = ["STRING_VALUE", "TIME"]
		self.lInfo = {}

	def generate(self):
		for key in self.lNames:
			self.lInfo[key] = {}
			for sel in self.lSels:
				cmd = (
					"sqlplus -S cms_hcl_runinfo/run2009info@cms_rcms @%s %s %s %s"
					% (self.templ, sel, self.lNames[key], self.runNumber))
				cmd = cmd.split(" ")
				output = utilities.do(cmd)
				for o in output.split("\n"):
					if o=="":
						continue
					elif o=="no rows selected":
						continue
					self.lInfo[key][sel] = o
					break
		self.saveInfo()

	def stripConfig(self, config):
		v = config.split("/")
		return v[len(v)-1]

	#	Save File with Run Information
	#	Format:
	#	
	#	1) nevents
	#	2) configuration
	#	3) timestamp
	def saveInfo(self):
		top = ET.Element("RunInfo")
		child_rn = ET.SubElement(top, "RunNumber")
		child_rn.text = self.runNumber
		child_config = ET.SubElement(top, "Configuration")
		child_config.text = self.stripConfig(
			self.lInfo["CONFIGURATION"]["STRING_VALUE"])
		child_nevs = ET.SubElement(top, "NumberEvents")
		if self.lInfo["NEVENTS"]=={}:
			child_nevs.text = "UNKNOWN"
		else:
			child_nevs.text = self.lInfo["NEVENTS"]["STRING_VALUE"]
		child_time = ET.SubElement(top, "TimeStamp")
		child_time.text = self.lInfo["CONFIGURATION"]["TIME"]
		top = ET.fromstring(prettify(top))
		tree = ET.ElementTree(top)
		tree.write("%s.xml" % self.runNumber)

#
#	upon exec
#	a file <runnumber>.info will be generated in the folder specficed as 
#	<pathToSave>
#
if __name__=="__main__":
	runNumber = sys.argv[1]
	pathToSave = sys.argv[2]
	templ = sys.argv[3]
	wrapper = RunInfoDBWrapper(runNumber, pathToSave, templ)
	wrapper.generate()




