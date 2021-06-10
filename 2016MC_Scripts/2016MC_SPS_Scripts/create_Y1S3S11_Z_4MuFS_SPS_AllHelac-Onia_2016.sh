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
#Used for both LHE steps 
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

#cd - #this one back is important!

EVENTS=44
# 
#echo NOTE: LHE step has  2 parts, the 0th and the first 
# echo BEGINNING 0th LHE step
# 
# cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_0th_LHE_cfg.py --eventcontent LHE --customise Configuration/DataProcessing/Utils.addMonitoring --datatier LHE --fileout file:Y1S3S11_Z_4Mu_HO_SPS_Oth_LHE.root --conditions 106X_mcRun2_asymptotic_v13 --beamspot Realistic25ns13TeV2016Collision --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(100)" --step NONE --filein file:bigOut12_Test.lhe --era Run2_2016 --no_exec --mc -n $EVENTS
# 
# echo FINISHED 0th LHE step

#echo BEGINNING first LHE step, the GEN step
#cmsDriver.py Configuration/GenProduction/python/standardFragment.py --python_filename Y1S3S11_Z_4Mu_HO_SPS_1st_LHE_cfg.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN --fileout file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE.root --conditions 106X_mcRun2_asymptotic_v13 --beamspot Realistic25ns13TeV2016Collision --step GEN --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_Oth_LHE.root --era Run2_2016 --no_exec --mc -n $EVENTS
# 
#echo FINISHED 1st LHE step, the GEN Step
# 


## CHECK WITH LEO TO MAKE SURE THIS SWITCH TO A NEW RELEASE HERE IS RIGHT, I think it is though

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
#
#echo BEGINNING SIM Step
#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_SIM_cfg.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_SIM.root --conditions 106X_mcRun2_asymptotic_v13 --beamspot Realistic25ns13TeV2016Collision --step SIM --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_1st_LHE.root --era Run2_2016 --runUnscheduled --no_exec --mc -n $EVENTS
# 
 #echo FINISHED SIM step
 
 #echo BEGINNING DIGIPremix Step
 #cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix_cfg.py --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix.root --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer20ULPrePremix-UL16_106X_mcRun2_asymptotic_v13-v1/PREMIX" --conditions 106X_mcRun2_asymptotic_v13 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_SIM.root --datamix PreMix --era Run2_2016 --runUnscheduled --no_exec --mc -n $EVENTS

#echo FINISHED DIGIPremix Step

#export SCRAM_ARCH=slc7_amd64_gcc530 #note new SCRAM_ARCH for HLT step!!
#source /cvmfs/cms.cern.ch/cmsset_default.sh
# if [ -r CMSSW_8_0_33_UL/src ] ; then
#   echo release CMSSW_8_0_33_UL already exists
# else
#   scram p CMSSW CMSSW_8_0_33_UL
# fi
# 
# cd CMSSW_8_0_33_UL/src
# eval `scram runtime -sh`
# cd ../..
# # 
# # #### THIS PART IS UNNECESSARY BECAUSE YOU NO LONGER ARE USING THE FRAGMENT AT THIS STAGE
# #mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# #cp standardFragment.py $CMSSW_BASE/src/Configuration/GenProduction/python
# # ### END UNNECESSARY PART THAT I WILL PROBABLY REMOVE LATER
# # 
# cd $CMSSW_BASE/src
# # # 
# scram b
# # 
# cd -
# 
# echo BEGINNING HLT Step
# 
# cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_HLT_cfg.py --eventcontent RAWSIM --outputCommand "keep *_mix_*_*,keep *_genPUProtons_*_*" --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --inputCommands "keep *","drop *_*_BMTF_*","drop *PixelFEDChannel*_*_*_*" --fileout file:Y1S3S11_Z_4Mu_HO_SPS_HLT.root --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:25ns15e33_v4 --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_DIGIPremix.root --era Run2_2016 --no_exec --mc -n $EVENTS 
# 
# echo FINISHED HLT step 

#cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_AOD_cfg.py --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_AOD.root --conditions 106X_mcRun2_asymptotic_v13 --step RAW2DIGI,L1Reco,RECO,RECOSIM --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_HLT.root --era Run2_2016 --runUnscheduled --no_exec --mc -n $EVENTS

#these two commands are already done at the very top
#export SCRAM_ARCH=slc7_amd64_gcc700

#source /cvmfs/cms.cern.ch/cmsset_default.sh

if [ -r CMSSW_10_6_25/src ] ; then
  echo release CMSSW_10_6_25 already exists
else
  scram p CMSSW CMSSW_10_6_25
fi
cd CMSSW_10_6_25/src
eval `scram runtime -sh`

scram b
cd ../..




cmsDriver.py  --python_filename Y1S3S11_Z_4Mu_HO_SPS_MiniAOD_cfg.py  --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:Y1S3S11_Z_4Mu_HO_SPS_MiniAOD.root --conditions 106X_mcRun2_asymptotic_v17 --step PAT --procModifiers run2_miniAOD_UL --geometry DB:Extended --filein file:Y1S3S11_Z_4Mu_HO_SPS_AOD.root --era Run2_2016 --runUnscheduled --no_exec --mc -n $EVENTS
