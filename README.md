#Heading 1 **YWZ**


*Set up instructions on lxplus (sl7)*  

mkdir someDummyWorkAreaName # change this to whatever you want :)
cd someDummyWorkAreaName
cmsrel CMSSW_10_6_2 #10_6_X needed for UL
cd CMSSW_10_6_2/src
git clone https://github.com/MaryHadley/YWZ
This is a bit of a hack to get things to compile, will try to fix later. In the meantime, the hack works.
cd YWZ
mv * ..
cd ..
rm -rf YWZ
cmsenv
scram b

git init
git set remote YWZ https://github.com/MaryHadley/YWZ

To run
cmsRun ZmuonAnalyzer_cfg.py
By default, the filter will be run before the analyzer. If you want to change this, do:
cmsRun ZmuonAnalyzer_cfg.py applyZmuonFilter=False at the command line

Source file (aka analyzer file) is analyzers/analyzers/src/ZmuonAnalyzer.cc
BuildFile.xml in analyzers/analyzers

Filter file is filters/ZmuonFilter/plugins/ZmuonFilter.cc
BuildFile.xml in filters/ZmuonFilter/plugins


