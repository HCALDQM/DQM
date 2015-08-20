
"""
file:				ImageCustomizer.py
author:				Viktor Khristenko
"""

#
#	imports
#
import ROOT

#
#	Global variables
#
bitshift = 19

kLogx = 0
kLogy = 1
kLogz = 2
nObjectBits = 3

#
#	Image Customizer
#
class ImageCustomizer:
	def __init__(self):
		pass

	def for2D(self, h):
		h.SetStats(ROOT.kFALSE)
		ROOT.gStyle.SetPalette(1)
		self.setLog(h)

	def for1D(self, h):
		h.SetStats(ROOT.kTRUE)
		self.setLog(h)

	def isBitSet(self, ibit, obj):
		return obj.TestBit(1<< (ibit+bitshift))
	
	def setBit(self, ibit):
		obj.SetBit(1<< (ibit+bitshift))

	def setLog(self, h):
		for i in range(nObjectBits):
			if not self.isBitSet(i, h):
				continue
			if i==kLogx:
				ROOT.gPad.SetLogx(1)
			elif i==kLogy:
				ROOT.gPad.SetLogy(1)
			elif i==kLogz:
				ROOT.gPad.SetLogz(1)

	#	Assuming TObject Inheritance
	def customize(self, obj):
		if obj.InheritsFrom("TH2"):
			self.for2D(obj)
		elif obj.InheritsFrom("TH1"):
			self.for1D(obj)





