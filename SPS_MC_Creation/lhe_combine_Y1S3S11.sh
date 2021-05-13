#!/bin/bash 

#pattern is: <runtDir>/PROC_HO_0/P0_calc_1/output

#Run me from the main Helac-Onia directory 
workDir=$PWD
runDir=testForSL_Y1S3S11_noDecay


./lhe_combine.py --files=$runDir/PROC_HO_0/P0_calc_0/output/samplessbarupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_1/output/sampleubaruupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_2/output/sampleuubarupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_3/output/samplecbarcupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_4/output/sampledbardupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_5/output/sampleccbarupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_6/output/samplesbarsupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_7/output/sampleddbarupsilon1Z.lhe,$runDir/PROC_HO_0/P0_calc_8/output/sampleggupsilon1Z.lhe  --out=Y1S3S11_total.lhe