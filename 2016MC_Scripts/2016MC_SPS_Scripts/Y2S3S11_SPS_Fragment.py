import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *
from GeneratorInterface.EvtGenInterface.EvtGenSetting_cff import * #to make the FourLep Filter work 

generator = cms.EDFilter("Pythia8HadronizerFilter",
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(13000.),
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CP5SettingsBlock,
    processParameters = cms.vstring(
            'SpaceShower:pTmaxMatch = 1',#Do I still need this, ask Leo/Google #Looks like 0 is the default from here: http://uaf-10.t2.ucsd.edu/~phchang/analysis/pseudojet/pythia/pythia8230/share/Pythia8/phpdoc/SpacelikeShowers.php?filepath= 
            'TimeShower:pTmaxMatch = 1', #Do I still need this, ask Leo/Google #1 is default, see: http://th-www.if.uj.edu.pl/~erichter/TauAnalFrame/external/pythia8185/htmldoc/TimelikeShowers.html
            '23:onMode = off',
            '23:onIfMatch = 13 -13', #mu- mu+ pair
            ),
        parameterSets = cms.vstring('pythia8CommonSettings',
                                    'pythia8CP5Settings',
                                    'processParameters',
                                    )
    )
)


FourMuonFilter = cms.EDFilter("FourLepFilter", #see here: https://cmssdt.cern.ch/lxr/source/GeneratorInterface/GenFilters/src/FourLepFilter.cc?v=CMSSW_10_2_21 and thanks to the authors of this presentation here: https://indico.cern.ch/event/962103/contributions/4050461/attachments/2115829/3560034/SPS_and_DPS.pdf for their help finding where this guy lives 
  Status = cms.untracked.int32(2), # what is the status code, check
  MaxEta = cms.untracked.double(2.5), # check this
  MinEta = cms.untracked.double(0.0), #Looking at the FourLepFilter more closely, think I should just set this to 0, since absolute values are used 
  MinPt = cms.untracked.double(2), # check where we want to put this cut 
  ParticleID = cms.untracked.int32(13)
)

ProductionFilterSequence = cms.Sequence(generator * FourMuonFilter)

#ProductionFilterSequence = cms.Sequence(generator)
