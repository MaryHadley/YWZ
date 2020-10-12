# YWZ

**Set up instructions on lxplus (sl7)**   

mkdir someDummyWorkAreaName # change this to whatever you want :)  
cd someDummyWorkAreaName  
cmsrel CMSSW_10_6_2  #10_6_X needed for UL  
cd CMSSW_10_6_2/src  
git clone https://github.com/MaryHadley/YWZ  
cmsenv  
scram b  
 
**To run**  
cmsRun ZmuonAnalyzer_cfg.py  
By default, the filter will be run before the analyzer. If you want to change this, do:  
cmsRun ZmuonAnalyzer_cfg.py applyZmuonFilter=False at the command line  

**Source file (aka analyzer file)** is analyzers/analyzers/src/ZmuonAnalyzer.cc  
BuildFile.xml in analyzers/analyzers

**Filter file** is filters/ZmuonFilter/plugins/ZmuonFilter.cc  
BuildFile.xml in filters/ZmuonFilter/plugins  
