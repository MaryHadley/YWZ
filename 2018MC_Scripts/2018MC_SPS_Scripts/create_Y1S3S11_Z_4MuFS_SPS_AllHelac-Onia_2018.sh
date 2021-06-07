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

#Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_cfg_2018.py
#Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_2018.root

#echo BEGINNING 0th LHE step
#cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_cfg_2018.py --eventcontent LHE --customise Configuration/DataProcessing/Utils.addMonitoring --datatier LHE --fileout file:Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_2018.root --conditions 106X_upgrade2018_realistic_v4 --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(100)" --step NONE --filein file:bigOut12_Test.lhe --era Run2_2018 --no_exec --mc -n $EVENTS
#echo FINISHED 0th LHE step

#echo BEGINNING 1st LHE step

#cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_cfg_2018.py  --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN --fileout file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_2018.root --conditions 106X_upgrade2018_realistic_v4 --beamspot Realistic25ns13TeVEarly2018Collision --step GEN --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_2018.root --era Run2_2018 --no_exec --mc -n $EVENTS 

#echo FINISHED 1st LHE step

#10_6_17_patch1 for SIM, DIGIPremix, AOD

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
# 
# 
# 
# 
# #### THIS PART IS UNNECESSARY BECAUSE YOU NO LONGER ARE USING THE FRAGMENT AT THIS STAGE
# #mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# #cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
# ### END UNNECESSARY PART THAT I WILL PROBABLY REMOVE LATER
# 
# cd $CMSSW_BASE/src
# # 
# scram b




#cd -


#echo BEGINNING SIM STEP
#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_SIM_cfg_2018.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_SIM_2018.root --conditions 106X_upgrade2018_realistic_v11_L1v1 --beamspot Realistic25ns13TeVEarly2018Collision --step SIM --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_2018.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $EVENTS 
#echo FINISHED SIM STEP

#echo BEGINNING DIGIPremix Step

#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_cfg_2018.py  --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_2018.root --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer20ULPrePremix-UL18_106X_upgrade2018_realistic_v11_L1v1-v2/PREMIX" --conditions 106X_upgrade2018_realistic_v11_L1v1 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_SIM_2018.root --datamix PreMix --era Run2_2018 --runUnscheduled --no_exec --mc -n $EVENTS

#echo FINISHED DIGIPremix Step





#10_2_16_UL for HLT step
# 
# if [ -r CMSSW_10_2_16_UL/src ] ; then
#   echo release CMSSW_10_2_16_UL already exists
# else
#   scram p CMSSW CMSSW_10_2_16_UL
# fi
# 
# cd CMSSW_10_2_16_UL/src
# eval `scram runtime -sh`
# cd ../..
# 
# 
# 
# 
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

#echo BEGINNING HLT STEP
#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_HLT_cfg_2018.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --fileout file:Y1S3S11_Z_4Mu_HO_SPS_HLT_2018.root --conditions 102X_upgrade2018_realistic_v15 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:2018v32 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_2018.root --era Run2_2018 --no_exec --mc -n $EVENTS
#echo FINISHED HLT STEP

#
#echo BEGINNING AOD step

#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_AOD_cfg_2018.py --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_AOD_2018.root --conditions 106X_upgrade2018_realistic_v11_L1v1 --step RAW2DIGI,L1Reco,RECO,RECOSIM,EI --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_HLT_2018.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $EVENTS

#echo FINISHED AOD step 

#10_6_20 for miniAOD

if [ -r CMSSW_10_6_20/src ] ; then
  echo release CMSSW_10_6_20 already exists
else
  scram p CMSSW CMSSW_10_6_20
fi

cd CMSSW_10_6_20/src
eval `scram runtime -sh`
cd ../..


#### THIS PART IS UNNECESSARY BECAUSE YOU NO LONGER ARE USING THE FRAGMENT AT THIS STAGE
#mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
#cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
### END UNNECESSARY PART THAT I WILL PROBABLY REMOVE LATER

cd $CMSSW_BASE/src
# 
scram b

cd -

echo BEGINNING miniAOD Step

cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_miniAOD_cfg_2018.py --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_miniAOD_2018.root --conditions 106X_upgrade2018_realistic_v16_L1v1 --step PAT --procModifiers run2_miniAOD_UL --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_AOD_2018.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $EVENTS

echo FINISHED miniAOD Step


