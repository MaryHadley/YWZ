#!/bin/bash

##Notes##
#Single parton scattering!
#Y1S3S11 + Z --> 4 mu
#Process to generate this sample is written below:
# set qcd = 1
# define p = g u u~ d d~ s s~ c c~
# generate p p > bb~(3s11) z
# decay bb~(3s11) > m+ m- @ 1d0
# decay z > m+ m- @ 1d0

#H0 = abbreviation for Helac-Onia 

export SCRAM_ARCH=slc7_amd64_gcc700

source /cvmfs/cms.cern.ch/cmsset_default.sh
# 
# 10_6_19_patch3 used for both LHE steps
#  
# if [ -r CMSSW_10_6_19_patch3/src ] ; then
#   echo release CMSSW_10_6_19_patch3 already exists
# else
#   scram p CMSSW CMSSW_10_6_19_patch3
# fi
# 
# cd CMSSW_10_6_19_patch3/src
# eval `scram runtime -sh`
# 
# 
# 
# cd ../..
# 
# mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
# cd $CMSSW_BASE/src
# 
# scram b
# 
# cd - #this one back is important!

EVENTS=44

#echo BEGINNING 0th LHE step
#cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_cfg_2017.py --eventcontent LHE --customise Configuration/DataProcessing/Utils.addMonitoring --datatier LHE --fileout file:Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_2017.root --conditions 106X_mc2017_realistic_v6 --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(100)" --step NONE --filein file:bigOut12_Test.lhe --era Run2_2017 --no_exec --mc -n $EVENTS
#echo FINISHED 0th LHE step

#echo BEGINNING 1st LHE step

#cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_cfg_2017.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN --fileout file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_2017.root --conditions 106X_mc2017_realistic_v6 --beamspot Realistic25ns13TeVEarly2017Collision --step GEN --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_2017.root --era Run2_2017 --no_exec --mc -n $EVENTS

#echo FINISHED 1st LHE step

#10_6_17_patch1 used for SIM, DIGIPremix, AOD

# if [ -r CMSSW_10_6_17_patch1/src ] ; then
#   echo release CMSSW_10_6_17_patch1 already exists
# else
#   scram p CMSSW CMSSW_10_6_17_patch1
# fi
# 
# cd CMSSW_10_6_17_patch1/src
# eval `scram runtime -sh`
# cd ../..
# 
# #### THIS PART IS UNNECESSARY BECAUSE YOU NO LONGER ARE USING THE FRAGMENT AT THIS STAGE
# #mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# #cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
# ### END UNNECESSARY PART THAT I WILL PROBABLY REMOVE LATER
# 
# cd $CMSSW_BASE/src
# # 
# scram b
# 
# cd -

#echo BEGINNING SIM Step

#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_SIM_cfg_2017.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_SIM_2017.root --conditions 106X_mc2017_realistic_v6 --beamspot Realistic25ns13TeVEarly2017Collision --step SIM --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_2017.root --era Run2_2017 --runUnscheduled --no_exec --mc -n $EVENTS

#echo FINISHED SIM Step 


#echo BEGINNING DIGIPremix step

#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_cfg_2017.py --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_2017.root --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer20ULPrePremix-UL17_106X_mc2017_realistic_v6-v3/PREMIX" --conditions 106X_mc2017_realistic_v6 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_SIM_2017.root --datamix PreMix --era Run2_2017 --runUnscheduled --no_exec --mc -n $EVENTS

#echo Finished DIGIPremix Step




#9_4_4_UL_patch1 used for HLT Step, also uses different scram arch

# echo AM HERE 95
# export SCRAM_ARCH=slc7_amd64_gcc630
# 
# source /cvmfs/cms.cern.ch/cmsset_default.sh
# echo AM HERE 99
# 
# if [ -r CMSSW_9_4_14_UL_patch1/src ] ; then
#   echo release CMSSW_9_4_14_UL_patch1 already exists
# else
#   scram p CMSSW CMSSW_9_4_14_UL_patch1
# fi
# 
# echo AM HERE 107
# cd CMSSW_9_4_14_UL_patch1/src
# eval `scram runtime -sh`
# cd ../..
# 
# #### THIS PART IS UNNECESSARY BECAUSE YOU NO LONGER ARE USING THE FRAGMENT AT THIS STAGE
# #mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# #cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
# ### END UNNECESSARY PART THAT I WILL PROBABLY REMOVE LATER
# 
# cd $CMSSW_BASE/src
# # 
# scram b
# 
# cd -
# 
# echo BEGINNING HLT Step
# cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_HLT_cfg_2017.py  --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --fileout file:Y1S3S11_Z_4Mu_HO_SPS_HLT_2017.root --conditions 94X_mc2017_realistic_v15 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:2e34v40 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_2017.root --era Run2_2017 --no_exec --mc -n $EVENTS
# 
# echo FINISHED HLT Step 

# echo BEGINNING AOD Step
# 
# cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_AOD_cfg_2017.py --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_AOD_2017.root --conditions 106X_mc2017_realistic_v6 --step RAW2DIGI,L1Reco,RECO,RECOSIM --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_HLT_2017.root --era Run2_2017 --runUnscheduled --no_exec --mc -n $EVENTS
# 
# echo FINISHED AOD step

#For MiniAOD Step, 10_6_20

if [ -r CMSSW_10_6_20/src ] ; then
  echo release CMSSW_10_6_20 already exists
else
  scram p CMSSW CMSSW_10_6_20
fi
cd CMSSW_10_6_20/src
eval `scram runtime -sh`

scram b
cd ../..

cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_MiniAOD_cfg_2017.py  --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_MiniAOD_2017.root --conditions 106X_mc2017_realistic_v9 --step PAT --procModifiers run2_miniAOD_UL --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_AOD_2017.root --era Run2_2017 --runUnscheduled --no_exec --mc -n $EVENTS
