# YWZ

#Set up instructions on lxplus (sl7)

mkdir <work area>
cd <work area>
cmsrel CMSSW_10_2_9
cd CMSSW_10_2_9/src
git clone https://github.com/MaryHadley/YWZ
cmsenv
scram b
 
#To run
cmsRun ZmuonAnalyzer_cfg.py

Source file is analyzers/analyzers/src/ZmuonAnalyzer.cc
BuildFile.xml in analyzers/analyzers
