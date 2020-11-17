#!/bin/bash

## Notes ###
############
#ZY -> 4 mu (DPS)
#Thank you to Jordan Martins for his help!
#################

#Note that all steps except HLT use this release, HLT uses the below

#start everything but HLT block 
export SCRAM_ARCH=slc7_amd64_gcc700

source /cvmfs/cms.cern.ch/cmsset_default.sh
if [ -r CMSSW_10_6_17_patch1/src ] ; then
  echo release CMSSW_10_6_17_patch1 already exists
else
  scram p CMSSW CMSSW_10_6_17_patch1
fi
cd CMSSW_10_6_17_patch1/src
eval `scram runtime -sh`

cd ../..

mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
cp YZ_4MuFS_DPS_fragment_Zfirst_Ysecond.py  $CMSSW_BASE/src/Configuration/GenProduction/python
cd $CMSSW_BASE/src

scram b

cd - #this one back is important!

#End everything but HLT block

#nEVENTS=50000 #for gen step testing purposes
nEVENTS=241 #after gen step (because of filter )
DATE=03_10_2020

#HLT release special

#Start HLT Block
# export SCRAM_ARCH=slc7_amd64_gcc530
# 
# source /cvmfs/cms.cern.ch/cmsset_default.sh
# if [ -r CMSSW_8_0_33_UL/src ] ; then
#   echo release CMSSW_8_0_33_UL already exists
# else
#   scram p CMSSW CMSSW_8_0_33_UL
# fi
# cd CMSSW_8_0_33_UL/src
# eval `scram runtime -sh`
# cd ../..
# mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
# cp YZ_4MuFS_DPS_fragment_Zfirst_Ysecond.py $CMSSW_BASE/src/Configuration/GenProduction/python
# cd $CMSSW_BASE/src
# # 
# scram b
# 
# cd -

##end HLT special block


#nEVENTS=6

echo CREATING $nEVENTS EVENTS...
#GEN cmsDriver command

#echo RUNNING GEN STEP 
#cmsDriver.py Configuration/GenProduction/python/YZ_4MuFS_DPS_fragment_Zfirst_Ysecond.py --python_filename run_cfg_YZ_4Mu_FS_DPS_GENStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN --fileout file:YZ_4MuFS_DPS_GEN_2016UL_Zfirst_Ysecond_${DATE}.root --conditions 106X_mcRun2_asymptotic_v13 --beamspot Realistic25ns13TeV2016Collision --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(100)" --step GEN --geometry DB:Extended --era Run2_2016 --no_exec --mc -n $nEVENTS 
#echo FINISHED GEN STEP 

# SIM cmsDriver command
#echo RUNNING SIM STEP
#cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_SIMStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:YZ_4MuFS_DPS_SIM_2016UL_Zfirst_Ysecond_${DATE}.root --conditions 106X_mcRun2_asymptotic_v13 --beamspot Realistic25ns13TeV2016Collision --step SIM --geometry DB:Extended --filein file:YZ_4MuFS_DPS_GEN_2016UL_Zfirst_Ysecond_${DATE}.root --era Run2_2016 --runUnscheduled --no_exec --mc -n $nEVENTS
#echo FINISHED SIM STEP 
# 
# PREMIX cmsDriver command
#  
#  Is the pileup part right? <CHECK ME>  
#echo RUNNING PREMIX STEP     
#cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_PREMIXStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:YZ_4MuFS_DPS_PREMIX_2016UL_Zfirst_Ysecond_${DATE}.root --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer19ULPrePremix-UL16_106X_mcRun2_asymptotic_v10-v2/PREMIX" --conditions 106X_mcRun2_asymptotic_v13 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:YZ_4MuFS_DPS_SIM_2016UL_Zfirst_Ysecond_${DATE}.root --datamix PreMix --era Run2_2016 --runUnscheduled --no_exec --mc -n $nEVENTS
#echo FINISHED PREMIX STEP 
# 
# 
# HLT cmsDriver command
# Are these conditions right <CHECK ME>
#echo RUNNING HLT STEP 
#echo USING OLDER RELEASE THAT IS RIGHT FOR HLT -- MARY, MAKE SURE YOU CHANGED THE RELEASE TO THE CORRECT ONE IN THE RELEASE BLOCK ABOVE
#cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_HLTStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent RAWSIM --outputCommand "keep *_mix_*_*,keep *_genPUProtons_*_*" --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --inputCommands "keep *","drop *_*_BMTF_*","drop *PixelFEDChannel*_*_*_*" --fileout file:YZ_4MuFS_DPS_HLT_2016UL_Zfirst_Ysecond_${DATE}.root --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:25ns15e33_v4 --geometry DB:Extended --filein file:YZ_4MuFS_DPS_PREMIX_2016UL_Zfirst_Ysecond_${DATE}.root --era Run2_2016 --no_exec --mc -n $nEVENTS 
#echo FINISHED HLT STEP 
# 
# 
# AOD cmsDriver command
#echo RUNNING AOD STEP
#echo BACK TO USING USUAL RELASE -- MARY, MAKE SURE YOU HAVE CHANGED THE RELEASE BACK TO THE NORMAL ONE
#cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_AODStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:YZ_4MuFS_DPS_AOD_2016UL_Zfirst_Ysecond_${DATE}.root --conditions 106X_mcRun2_asymptotic_v13 --step RAW2DIGI,L1Reco,RECO,RECOSIM --geometry DB:Extended --filein file:YZ_4MuFS_DPS_HLT_2016UL_Zfirst_Ysecond_${DATE}.root  --era Run2_2016 --runUnscheduled --no_exec --mc -n $nEVENTS
#echo FINISHED AOD STEP 
# 
# MiniAOD cmsDriver command 
echo RUNNING MINIAOD STEP
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_MiniAODStep_2016UL_Zfirst_Ysecond_${DATE}.py --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:YZ_4MuFS_DPS_MiniAOD_2016UL_Zfirst_Ysecond_${DATE}.root --conditions 106X_mcRun2_asymptotic_v13 --step PAT --geometry DB:Extended --filein file:YZ_4MuFS_DPS_AOD_2016UL_Zfirst_Ysecond_${DATE}.root --era Run2_2016 --runUnscheduled --no_exec --mc -n $nEVENTS 
echo FINISHED MINIAOD STEP
# 
# 
# 
#echo DONE CREATING YZ_4MUFS_2016UL_DPS MC

