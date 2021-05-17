#!/bin/bash 

#Run me from ho_dir, the main Helac-Onia directory 
ho_dir = /afs/cern.ch/work/m/mhadley/public/GOOD_HELAC-Onia_MinimalSetup/HELAC-Onia-2.5.5
runDir =FIXME

echo HAVE YOU MADE THE APPROPRIATE CHANGE TO THE FIRST ONIA PDGID LINE IN input/py8_onia_user.inp? 
echo REMEMBER YOU ARE RUNNING Y 2S SAMPLES NOW! 
echo CONFIRM YOU HAVE MADE THE CHANGE BY ENTERING YES OR yes or Yes or Y or y

read yesorno

if [ "$yesorno" = yes ] || [ "$yesorno" = YES ] || [ "$yesorno" = Yes ]  || [ "$yesorno" = Y ]  || [ "$yesorno" = y ]

then
    echo PROCEEDING WITH GROOMING...
    
    echo GROOMING Y2S3S11 SAMPLES...
    #Y2S3S11 samples 
    for i in 0 1 2 3 4 5 6 7 8
    do
       cp input/py8_onia_user.inp $runDir/PROC_HO_0/P0_calc_$i/output
       cd $runDir/PROC_HO_0/P0_calc_$i/output
       $ho_dir/bin/lhe_pythia6_pythia8
       cd ho_dir
    done
    
        
else
    echo EXITING SO YOU CAN FIX THE input/py8_onia_user.inp...
    echo GOODBYE!

fi
