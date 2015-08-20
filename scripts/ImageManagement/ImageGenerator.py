
"""
file:			ImageGenerator.py
Author:			Viktor Khristenko
"""

#
#	Imports
#
import ROOT
from utilities import *
import ImageCustomizer as IC

#
#	Image Generator Class
#
class ImageGenerator:
	def __init__(self, rootFile, filetype, outDirPath, filters):
		self.rootFile = rootFile
		self.traversed = False
		self.filetype = filetype
		self.outDirPath = outDirPath
		self.lDirFilters = filters
		self.customizer = IC.ImageCustomizer()

		ROOT.gROOT.SetBatch(True)
		self.canvas = ROOT.TCanvas("c", "c", 100, 100, 700, 500)

	#	generate the images
	def generate(self, dataDirName):
		self.rootFile.cd(dataDirName)
		for dirKey in ROOT.gDirectory.GetListOfKeys():
			dirName = dirKey.GetName()
			if not (dirName in self.lDirFilters):
				continue

			ROOT.gDirectory.cd(dirName)
			mkdir(self.outDirPath+dirName)
			self.traverse(self.outDirPath+dirName+"/")
			ROOT.gDirectory.cd("..")

	# save 2D
	def saveTH2(self, path, h):
		self.canvas.cd()
		opts = "colz"
		self.customizer.customize(h)
		h.Draw(opts)
		self.canvas.SaveAs(path+h.GetName()+".%s" % self.filetype)

	def saveTH1(self, path, h):
		self.canvas.cd()
		self.customizer.customize(h)
		h.Draw()
		self.canvas.SaveAs(path+h.GetName()+".%s" % self.filetype)

	#	recursively traverse and save
	#	assume we are in one of the data Directories as HcalPedestalTask or
	#	HcalLEDTask
	def traverse(self, path):
		for key in ROOT.gDirectory.GetListOfKeys():
			obj = key.ReadObj()
			name = key.GetName()
			if obj.InheritsFrom("TDirectoryFile"):
				ROOT.gDirectory.cd(name)
				mkdir(path+name)
				self.traverse(path+name+"/")
				ROOT.gDirectory.cd("..")
			#	Important TH2 goes first, then TH1
			elif obj.InheritsFrom("TH2"):
				self.saveTH2(path, obj)
			elif obj.InheritsFrom("TH1"):
				self.saveTH1(path, obj)











