import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import * # this setting is probably fine
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import * #Checked, this is fine, the 2017 refers to when they made the module, does NOT indicate it should only be used for 2017 MC 
from GeneratorInterface.EvtGenInterface.EvtGenSetting_cff import * #to make the FourLep Filter work 
#from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import * #I'm going to guess not needed 

generator = cms.EDFilter("Pythia8GeneratorFilter",
                                 comEnergy = cms.double(13000.0),
                                 crossSection = cms.untracked.double(8.45E-6), #David helped me confirm that this setting is obsolete and doesn't do anything, see FAQ of this page: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer
                                 filterEfficiency = cms.untracked.double(1.0), # check this setting, I think fine
                                 maxEventsToPrint = cms.untracked.int32(0),
                                 pythiaPylistVerbosity = cms.untracked.int32(0),
                                 pythiaHepMCVerbosity = cms.untracked.bool(False),
                                 PythiaParameters = cms.PSet(

    pythia8CommonSettingsBlock,
    pythia8CP5SettingsBlock,
#   pythia8PSweightsSettingsBlock, #I guess not needed 
    processParameters = cms.vstring(
      'Main:timesAllowErrors    = 10000',
      #'HiggsSM:all=true',
      #'25:m0 = 125.0',
      #'25:onMode = off',
      #'25:addChannel = 1  1.00   103   22   553',
      #'Bottomonium:all = on',
      # 'Bottomonium:gg2bbbar(3S1)[3S1(1)]g    = on,off,off',
#       'Bottomonium:gg2bbbar(3S1)[3S1(1)]gm   = on,off,off',
#       'Bottomonium:gg2bbbar(3S1)[3S1(8)]g    = on,off,off',
#       'Bottomonium:qg2bbbar(3S1)[3S1(8)]q    = on,off,off',
#       'Bottomonium:qqbar2bbbar(3S1)[3S1(8)]g = on,off,off',
#       'Bottomonium:gg2bbbar(3S1)[1S0(8)]g    = on,off,off',
#       'Bottomonium:qg2bbbar(3S1)[1S0(8)]q    = on,off,off',
#       'Bottomonium:qqbar2bbbar(3S1)[1S0(8)]g = on,off,off',
#       'Bottomonium:gg2bbbar(3S1)[3PJ(8)]g    = on,off,off',
#       'Bottomonium:qg2bbbar(3S1)[3PJ(8)]q    = on,off,off',
#       'Bottomonium:qqbar2bbbar(3S1)[3PJ(8)]g = on,off,off',
        
        #Make Y with first hard scatter 
        'Bottomonium:all = on' #Again, question if we want to do this or use the long list from above, doing this will include the chi b states #We decided to go this direction for now!
        #Don't bother to force decays, deal with it with a filter as Steve Mrenna suggested 
       
       #Make Z with second hard scatter  hard scatter 
       'SecondHard:generate = on',
       'SecondHard:SingleGmZ = on', #see http://home.thep.lu.se/~torbjorn/pythia82html/ASecondHardProcess.html
       'WeakZ0:gmZmode = 0', #0 is the default and includes full Z/gamma* structure with interference included. see: http://home.thep.lu.se/~torbjorn/pythia81html/Welcome.html Note that all output particles will have the Z code, 23.
       'PhaseSpace:mHatMin = 75.', # suggested by: http://skands.physics.monash.edu/slides/worksheet8165-asp.pdf
       'PhaseSpace:mHatMax = 120.', #suggested by: http://skands.physics.monash.edu/slides/worksheet8165-asp.pdf
       '23:onMode = off',
       '23:onIfMatch = 13 -13', #mu- mu+ pair
#       'PartonLevel:MPI = on', #check this? is it needed?  Looks like not needed, is the same as the default, see: http://home.thep.lu.se/~torbjorn/pythia82html/MasterSwitches.html
        ),
    parameterSets = cms.vstring(
      'pythia8CommonSettings',
      'pythia8CP5Settings',
#      'pythia8PSweightsSettings', # think not needed 
      'processParameters')
    )
)
FourMuonFilter = cms.EDFilter("FourLepFilter", #see here: https://cmssdt.cern.ch/lxr/source/GeneratorInterface/GenFilters/src/FourLepFilter.cc?v=CMSSW_10_2_21 and thanks to the authors of this presentation here: https://indico.cern.ch/event/962103/contributions/4050461/attachments/2115829/3560034/SPS_and_DPS.pdf for their help finding where this guy lives 
  Status = cms.untracked.int32(2), # what is the status code, check
  MaxEta = cms.untracked.double(4.0), # check this
  MinEta = cms.untracked.double(0.0), #Looking at the FourLepFilter more closely, think I should just set this to 0, since absolute values are used 
  MinPt = cms.untracked.double(2), # check where we want to put this cut 
  ParticleID = cms.untracked.int32(13)
)

ProductionFilterSequence = cms.Sequence(generator * FourMuonFilter) #*upsilonfilter

#ProductionFilterSequence = cms.Sequence(generator) #*upsilonfilter
