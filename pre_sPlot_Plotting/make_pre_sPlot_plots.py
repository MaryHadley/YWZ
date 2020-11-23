import ROOT
import pprint
from os import listdir, path

#sampleDir = '/afs/cern.ch/work/m/mhadley/public/submitToCrab_2021ULData_YZ_106X/CMSSW_10_6_2/src/phase2_ntuplemaker/'
sampleDir = '/afs/cern.ch/work/m/mhadley/public/submitToCrab_2018ULData_YZ_106X/CMSSW_10_6_2/src/phase2_ntuplemaker'
print sampleDir 

root_file = 'ntuple_skimmed_maryTest_23Nov2020.root'
sample =  path.join(sampleDir,root_file)

print sample

myChain = ROOT.TChain('tree')

myChain.Add(sample)
print myChain


hists = []
varNames=[]
for var in myChain.GetListOfBranches():
    print var
    varNames.append(var.GetName())

for varName in varNames:
    myCanvas = ROOT.TCanvas()
    myCanvas.Clear()
    #print varNames
    histLabel = "%s" % (varName)
    print histLabel
    #ROOT.SetOwnership(histLabel, False)
    hists.append(ROOT.TH1F(histLabel, histLabel, 100, -100, 100))
    ROOT.SetOwnership(ROOT.TH1F(histLabel, histLabel, 100, -100, 100), False)
    myCanvas.cd()
    myChain.Draw("%s>>%s" % (varName, histLabel))
    myCanvas.SaveAs("%s.pdf" %(varName))

#    if varName == "Z_mass":
  #  myChain.Draw("%s>>%s" % (varName, histLabel))
 #       myChain.Draw("Z_mass>>Z_mass")

print 'hists:', hists  


myCanvas = ROOT.TCanvas()
hists[0].Draw()
myCanvas.SaveAs("test.pdf")

#myHist = ROOT.TH1F("blah", "blah", 100, -100, 100)
#myChain.Draw(Z_mass >> myHist)
