import ROOT
import pprint #remember if you do it like this, you need to call it as pprint.pprint(<someThing>)
#If you want to just be able to write pprint(<someThing>) you need to do from pprint import pprint \

from os import listdir, path 
from collections import OrderedDict

sampleDir='/afs/cern.ch/work/m/mhadley/public/submitToCrab_2018ULData_YZ_106X/CMSSW_10_6_2/src/phase2_ntuplemaker' #this is just a placeholder


sampleDict = OrderedDict()
chainDict = OrderedDict()
#sampleDict = {}
i = -1 
root_file_list = ['ntuple_skimmed_maryTest_23Nov2020.root']
for myFile in root_file_list:
    i += 1
    sampleDict['sample_' + str(i)] =  path.join(sampleDir,myFile)


#print "sampleDict: \n", sampleDict
#print "###############"

#for key, value in sampleDict.iteritems():
#   print key, value 

myChain = ROOT.TChain('tree')



for sample in sampleDict.keys():
    print sampleDict[sample]
    myChain.Add(sampleDict[sample])
    chainDict[sample] = myChain

#print "myChain: \n", myChain
print "chainDict: \n"
pprint.pprint(chainDict)



varNames = []

for branch in myChain.GetListOfBranches():
    varNames.append(branch.GetName())


print "varNames: \n"
pprint.pprint(varNames)

myHists = []

for sample in sampleDict.keys():
    for var in varNames:
        histName = "h_%s_%s" %(var, sample)
        print 'histName:', histName
        myHists.append(ROOT.TH1F(histName, histName, 600, -100, 200)) #dummy  binning FIX ME
        myChain.Draw("%s>> %s"%(var, histName))
        

print "myHists: \n"
pprint.pprint(myHists)


    