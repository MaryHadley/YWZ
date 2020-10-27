import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import * # this setting is probably fine
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import * # to be checked 
from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import * #I'm going to guess not needed 

generator = cms.EDFilter("Pythia8GeneratorFilter",
                                 comEnergy = cms.double(13000.0),
                                 crossSection = cms.untracked.double(8.45E-6), #where did this number come from...check this 
                                 filterEfficiency = cms.untracked.double(1.0), # check this setting, I think fine
                                 maxEventsToPrint = cms.untracked.int32(0),
                                 pythiaPylistVerbosity = cms.untracked.int32(0),
                                 pythiaHepMCVerbosity = cms.untracked.bool(False),
                                 PythiaParameters = cms.PSet(

    pythia8CommonSettingsBlock,
    pythia8CP5SettingsBlock,
   pythia8PSweightsSettingsBlock, #I guess not needed 
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

       #Make Z with first hard scatter 
       'WeakZ0:gmZmode = 0', #0 is the default and includes full Z/gamma* structure with interference included. see: http://home.thep.lu.se/~torbjorn/pythia81html/Welcome.html Note that all output particles will have the Z code, 23.
       '23:onMode = off',
       '23:onIfMatch = 13 -13', #mu- mu+ pair
       'PartonLevel:MPI = on', #check this? is it needed?  Looks like not needed, is the same as the default, see: http://home.thep.lu.se/~torbjorn/pythia82html/MasterSwitches.html
       'SecondHard:generate = on',
       'SecondHard:Bottomonium = on', 
       #I think Bottomonium will decay automatically, see: mayDecay: a flag telling whether a particle species may decay or not, offering the main user switch. Whether a given particle of this kind then actually will decay also depends on it having allowed decay channels, and on other flags for particle decays (or resonance decays). All particles with tau0 below 1000 mm (aka 1 m)are by default initialized to allow decays. http://home.thep.lu.se/~torbjorn/pythia82html/ParticleDataScheme.html
      # Per Steve Mrenna's advice, don't restrict decays, deal with things with a filter 
     
      ),

    parameterSets = cms.vstring(
      'pythia8CommonSettings',
      'pythia8CP5Settings',
#      'pythia8PSweightsSettings', # think not needed 
      'processParameters')
    )
)
FourMuonFilter = cms.EDFilter("FourMuonFilter", #I know how to make this guy, I just need to make sure I include it correctly...or is there a pre existing 4 lep filter? Email these folks: https://indico.cern.ch/event/962103/contributions/4050461/attachments/2115829/3560034/SPS_and_DPS.pdf Have emailed them, will update when they reply 
   Status = cms.untracked.int32(2), # what is the status code, check
   MaxEta = cms.untracked.double(3.0), # check this
   MinEta = cms.untracked.double(-3.0),
   MinPt = cms.untracked.double(2), # check where we want to put this cut 
   ParticleID = cms.untracked.int32(13)
)

ProductionFilterSequence = cms.Sequence(generator * FourMuonFilter) #*upsilonfilter




