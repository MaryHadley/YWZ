#!/bin/bash

###Notes####
##########
#YZ->4 mu (DPS) 2018 UL 
#Thank you to Jordan Martins, Leonardo Lunerti for their  help with this


export SCRAM_ARCH=slc7_amd64_gcc700

source /cvmfs/cms.cern.ch/cmsset_default.sh
if [ -r CMSSW_10_6_17_patch1/src ] ; then
  echo release CMSSW_10_6_17_patch1 already exists
else
  scram p CMSSW CMSSW_10_6_17_patch1
fi
cd CMSSW_10_6_17_patch1/src
eval `scram runtime -sh`


mkdir -pv $CMSSW_BASE/src/Configuration/GenProduction/python
#cp YZ_4MuFS_DPS_fragment_2016UL.py $CMSSW_BASE/src/Configuration/GenProduction/python
cd $CMSSW_BASE/src


scram b
cd ../..

nEVENTS=100 #for testing purposes

echo CREATING $nEVENTS EVENTS...

#GEN cmsDriver command 
echo RUNNING GEN STEP
cmsDriver.py Configuration/GenProduction/python/YZ_4MuFS_DPS_fragment.py --python_filename run_cfg_YZ_4Mu_FS_DPS_GENStep_2018UL.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN --fileout file:YZ_4MuFS_DPS_GEN_2018UL.root  --conditions 106X_upgrade2018_realistic_v4 --beamspot Realistic25ns13TeVEarly2018Collision --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(100)" --step GEN --geometry DB:Extended --era Run2_2018 --no_exec --mc -n $nEVENTS
echo FINISHED GEN STEP

# SIM cmsDriver command
echo RUNNING SIM STEP
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_SIMStep_2018UL.py  --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:YZ_4MuFS_DPS_SIM_2018UL.root --conditions 106X_upgrade2018_realistic_v11_L1v1 --beamspot Realistic25ns13TeVEarly2018Collision --step SIM --geometry DB:Extended --filein file:YZ_4MuFS_DPS_GEN_2018UL.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $nEVENTS
echo FINISHED SIM STEP 


# PREMIX cmsDriver command
echo RUNNING PREMIX STEP
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_PREMIXStep_2018UL.py --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:YZ_4MuFS_DPS_PREMIX_2018UL.root --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer19ULPrePremix-UL18_106X_upgrade2018_realistic_v11_L1v1-v2/PREMIX" --conditions 106X_upgrade2018_realistic_v11_L1v1 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:YZ_4MuFS_DPS_SIM_2018UL.root --datamix PreMix --era Run2_2018 --runUnscheduled --no_exec --mc -n $nEVENTS
echo FINISHED PREMIX STEP 

#HLT cmsDriver command
echo RUNNING HLT STEP 
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_HLTStep_2018UL.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --fileout file:YZ_4MuFS_DPS_HLT_2018UL.root --conditions 102X_upgrade2018_realistic_v15 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:2018v32 --geometry DB:Extended --filein file:YZ_4MuFS_DPS_PREMIX_2018UL.root --era Run2_2018 --no_exec --mc -n $nEVENTS
echo FINISHED HLT STEP 

# AOD cmsDriver command
echo RUNNING AOD STEP
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_AODStep_2018UL.py --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:YZ_4MuFS_DPS_AOD_2018UL.root --conditions 106X_upgrade2018_realistic_v11_L1v1 --step RAW2DIGI,L1Reco,RECO,RECOSIM,EI --geometry DB:Extended --filein file:YZ_4MuFS_DPS_HLT_2018UL.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $nEVENTS
echo FINISHED AOD STEP 


#MINIAOD cmsDriver command
echo RUNNING MINIAOD STEP 
cmsDriver.py  --python_filename run_cfg_YZ_4Mu_FS_DPS_MiniAODStep_2018UL.py --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:YZ_4MuFS_DPS_MiniAOD_2018UL.root --conditions 106X_upgrade2018_realistic_v11_L1v1 --step PAT --geometry DB:Extended --filein file:YZ_4MuFS_DPS_AOD_2018UL.root --era Run2_2018 --runUnscheduled --no_exec --mc -n $nEVENTS
echo FINISHED MINIAOD STEP 
