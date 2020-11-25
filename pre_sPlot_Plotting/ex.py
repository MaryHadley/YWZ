import ROOT as r
chain = r.TChain("Events")
chain.Add("root://cmsxrootd.fnal.gov//store/mc/RunIIAutumn18NanoAODv7/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/130000/7E835C98-04BC-1348-B707-440BD82F2AAA.root")
varNames = []
for branch in chain.GetListOfBranches():
  varNames.append(branch.GetName())

hists = []
for var in varNames[:3]: # just do first three as an example
  histName = "hist_%s"%var
  hists.append(r.TH1F(histName, histName, 100, 0, 0))
  chain.Draw("%s>> %s"%(var, histName))
