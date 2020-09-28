import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing #https://twiki.cern.ch/CMSPublic/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T

process = cms.Process("analysis")

options = VarParsing.VarParsing("analysis")



#List of Options
options.register( "applyZmuonFilter",
    True,
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.bool,
    "Shall we prefilter out some events and never bother giving them to the analyzer? Defaults to True."
)

# This needs to go here, after we have defined all the options!
options.parseArguments()
print "Options are:", options

process.ZmuonAnalyzer = cms.EDAnalyzer("ZmuonAnalyzer",
   muonCollection = cms.InputTag("slimmedMuons"),
   electronCollection = cms.InputTag("slimmedElectrons"),
   bits = cms.InputTag("TriggerResults","", "HLT"),
   objects = cms.InputTag("selectedPatTrigger"),
   genParticles = cms.InputTag("prunedGenParticles"),
   vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
   metTag = cms.InputTag("slimmedMETs"),
   pfCands = cms.InputTag("packedPFCandidates")

)

if options.applyZmuonFilter:
   process.ZmuonFilter = cms.EDFilter("ZmuonFilter",
     muonCollection = cms.InputTag("slimmedMuons"),
     bits = cms.InputTag("TriggerResults","", "HLT"),
     objects = cms.InputTag("selectedPatTrigger"),
#     genParticles = cms.InputTag("prunedGenParticles"), #not needed here
#     pfCands = cms.InputTag("packedPFCandidates")  #not needed here
     pTCut = cms.double(0.),
     etaCut = cms.double(4.),
     invMass4MuCut = cms.double(0.),
  )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
                                          "file:../../../miniAOD_01.root",
                                          "file:../../../miniAOD_02.root",
                                          "file:../../../miniAOD_03.root",
                                          "file:../../../miniAOD_04.root",
                                          "file:../../../miniAOD_05.root",
                                          "file:../../../miniAOD_06.root"
#                                          "file:../../../miniAOD_lowStats.root",
 #                                          "file:../SingleMu_Run2018A-17Sep2018-v2_F8CDAAA9-11C7-A34F-A059-409CF95EB82A.root"
#                                            "file:../SingleMu_2017B-31Mar2018-v1_FC2B7874-F538-E811-9C29-0025905A60A8.root"
                                      ),
   duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("mc_ZUpsi_mary_28Sept2020_test.root")
)

process.maxEvents = cms.untracked.PSet(
#   input = cms.untracked.int32(40000)
   input = cms.untracked.int32(-1)

)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#process.MessageLogger = cms.Service("MessageLogger",
#                    destinations   = cms.untracked.vstring('messages.txt')
#)

if options.applyZmuonFilter:
    process.p = cms.Path(
        process.ZmuonFilter * process.ZmuonAnalyzer
        )

else:


    process.p = cms.Path(
        process.ZmuonAnalyzer
)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data')
