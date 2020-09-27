// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>

// for vertexing                                                                                                                                                                                        
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"  // edm::Handle
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"                                                                                             
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
     
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"                                                                                                       
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonQuality.h"
             
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"                                                                                                 
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

#include <math.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <algorithm>  // std::sort, std::swap
#include <iostream>  // std::cout, std::endl
#include <string>

class ZmuonAnalyzer : public edm::EDAnalyzer {
public:
	explicit ZmuonAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<pat::Muon>> muonsToken_;
   edm::EDGetTokenT<std::vector<reco::Vertex>> token_vertices;
   edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
//   edm::EDGetTokenT<pat::TriggerEvent> triggerEventToken_;
//   edm::EDGetTokenT<std::vector<std::string>> muonMatch_;
   edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
   edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
   edm::EDGetTokenT<pat::PackedCandidateCollection> pfToken_;

   TTree * tree;
   TTree * treemc;

   std::vector<double> lepton1_pt, lepton1_eta, lepton1_phi;
   std::vector<double> lepton1_charge;
   std::vector<double> lepton1_d0, lepton1_dz, lepton1_dxy; //recall d0 is the 3D IP
   std::vector<double> lepton1_iso03particle, lepton1_iso03hadron, lepton1_iso04hadron, lepton1_iso04particle; //these are the R = 0.4 or 0.3 isolation variables 
   std::vector<double> lepton1_impactParameterSignificance;
   std::vector<bool> lepton1_isLooseMuon, lepton1_isSoftMuon, lepton1_isTightMuon;
   std::vector<bool> lepton1_isPFMuon, lepton1_isGlobalMuon, lepton1_isTrackerMuon;
   std::vector<double> lepton1_deltaEta, lepton1_deltaPhi, lepton1_sigmaiEtaEta, lepton1_HoverE;
   std::vector<double> lepton1_OoEmOoP, lepton1_vtxFitProb, lepton1_missingHits; //QUESTION! OoEmOop means what? //Note to self: looks these guys on this line aren't filled in yet in this code  

   std::vector<double> lepton2_pt, lepton2_eta, lepton2_phi;
   std::vector<double> lepton2_charge;
   std::vector<double> lepton2_d0, lepton2_dz, lepton2_dxy;
   std::vector<double> lepton2_iso03particle, lepton2_iso03hadron, lepton2_iso04hadron, lepton2_iso04particle;
   std::vector<double> lepton2_impactParameterSignificance;
   std::vector<bool> lepton2_isLooseMuon, lepton2_isSoftMuon, lepton2_isTightMuon;
   std::vector<bool> lepton2_isPFMuon, lepton2_isGlobalMuon, lepton2_isTrackerMuon;
   std::vector<double> lepton2_deltaEta, lepton2_deltaPhi, lepton2_sigmaiEtaEta, lepton2_HoverE;
   std::vector<double> lepton2_OoEmOoP, lepton2_vtxFitProb, lepton2_missingHits; //again, these guys don't appear to be filled yet 

   std::vector<double> lepton3_pt, lepton3_eta, lepton3_phi;
   std::vector<double> lepton3_charge;
   std::vector<double> lepton3_d0, lepton3_dz, lepton3_dxy;
   std::vector<double> lepton3_iso03particle, lepton3_iso03hadron, lepton3_iso04hadron, lepton3_iso04particle;
   std::vector<double> lepton3_impactParameterSignificance;
   std::vector<bool> lepton3_isLooseMuon, lepton3_isSoftMuon, lepton3_isTightMuon;
   std::vector<bool> lepton3_isPFMuon, lepton3_isGlobalMuon, lepton3_isTrackerMuon;
   std::vector<double> lepton3_deltaEta, lepton3_deltaPhi, lepton3_sigmaiEtaEta, lepton3_HoverE;
   std::vector<double> lepton3_OoEmOoP, lepton3_vtxFitProb, lepton3_missingHits; //again, these guys don't appear to be filled yet 

   std::vector<double> lepton4_pt, lepton4_eta, lepton4_phi;
   std::vector<double> lepton4_charge;
   std::vector<double> lepton4_d0, lepton4_dz, lepton4_dxy;
   std::vector<double> lepton4_iso03particle, lepton4_iso03hadron, lepton4_iso04hadron, lepton4_iso04particle;
   std::vector<double> lepton4_impactParameterSignificance;
   std::vector<bool> lepton4_isLooseMuon, lepton4_isSoftMuon, lepton4_isTightMuon;
   std::vector<bool> lepton4_isPFMuon, lepton4_isGlobalMuon, lepton4_isTrackerMuon;
   std::vector<double> lepton4_deltaEta, lepton4_deltaPhi, lepton4_sigmaiEtaEta, lepton4_HoverE;
   std::vector<double> lepton4_OoEmOoP, lepton4_vtxFitProb, lepton4_missingHits; //again, these guys don't appear to be filled yet 

   std::vector<double> dimuon1mass, dimuon2mass;
   std::vector<double> dimuon1pt,   dimuon2pt ;
   std::vector<double> dimuon1eta,  dimuon2eta;
   std::vector<double> dimuon1phi,  dimuon2phi;
   std::vector<double> dimuon1vtx,  dimuon2vtx;
   std::vector<double> dimuon1lxy,  dimuon2lxy;
   std::vector<double> dimuon1lxysig,     dimuon2lxysig  ;
   std::vector<double> dimuon1lxyctauPV,  dimuon2lxyctauPV;
   std::vector<double> sixmuonvtx;
   std::vector<double> dimuon1vtx_xpos, dimuon2vtx_xpos;
   std::vector<double> dimuon1vtx_ypos, dimuon2vtx_ypos;
   std::vector<double> dimuon1vtx_zpos, dimuon2vtx_zpos;
   std::vector<double> dimuon1vtx_xposError, dimuon2vtx_xposError;
   std::vector<double> dimuon1vtx_yposError, dimuon2vtx_yposError;
   std::vector<double> dimuon1vtx_zposError, dimuon2vtx_zposError;
   std::vector<unsigned int>  event_number;
   std::vector<unsigned int>  run_number;

   std::vector<double> PVx, PVy, PVz;

   std::vector<double> truth_Zmuon_pt, truth_Zmuon_eta, truth_Zmuon_phi;
   std::vector<double> truth_Z_pt, truth_Z_eta, truth_Z_phi, truth_Z_mass, truth_Z_pdgid;

   std::vector<double> truth_Jpsimuon_pt, truth_Jpsimuon_eta, truth_Jpsimuon_phi;
   std::vector<double> truth_Jpsi_pt, truth_Jpsi_eta, truth_Jpsi_phi, truth_Jpsi_mass;

   std::vector<double> truth_Jpsi_pdgid;

   std::vector<std::string> triggerlist;

   std::vector<double> numberOfVertices;
   std::vector<double> zOfVerticesError;
   std::vector<double> zOfVertices;

   std::vector<bool> pair_12_34_56;
   std::vector<bool> pair_13_24_56;
   std::vector<bool> pair_14_23_56;

};

ZmuonAnalyzer::ZmuonAnalyzer(const edm::ParameterSet& iConfig)
{
   muonsToken_        = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection"));
   token_vertices     = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   triggerBits_       = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
//   triggerEventToken_ = consumes<pat::TriggerEvent>(iConfig.getParameter<edm::InputTag>("triggerEvent"));
//   muonMatch_         = consumes<std::vector<std::string>>(iConfig.getParameter< std::string >( "muonMatch" ) );
   triggerObjects_ = consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"));
   genParticlesToken_ = consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"));
   pfToken_ = consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCands"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");

   tree->Branch("event_number", &event_number);
   tree->Branch("run_number", &run_number);

   tree->Branch("lepton1_pt",  &lepton1_pt);
   tree->Branch("lepton1_eta", &lepton1_eta);
   tree->Branch("lepton1_phi", &lepton1_phi);
   tree->Branch("lepton1_charge", &lepton1_charge);
   tree->Branch("lepton1_d0",  &lepton1_d0);
   tree->Branch("lepton1_dxy", &lepton1_dxy);
   tree->Branch("lepton1_dz",  &lepton1_dz);
   tree->Branch("lepton1_iso03particle", &lepton1_iso03particle);
   tree->Branch("lepton1_iso04particle", &lepton1_iso04particle);
   tree->Branch("lepton1_iso03hadron", &lepton1_iso03hadron);
   tree->Branch("lepton1_iso04hadron", &lepton1_iso04hadron);
   tree->Branch("lepton1_impactParameterSignificance", &lepton1_impactParameterSignificance);
   tree->Branch("lepton1_isLooseMuon", &lepton1_isLooseMuon);    // these are only for muons 
   tree->Branch("lepton1_isSoftMuon",  &lepton1_isSoftMuon);     // these are only for muons  
   tree->Branch("lepton1_isTightMuon", &lepton1_isTightMuon);    // these are only for muons  
   tree->Branch("lepton1_isPFMuon", &lepton1_isPFMuon);          // these are only for muons  
   tree->Branch("lepton1_isGlobalMuon", &lepton1_isGlobalMuon);
   tree->Branch("lepton1_isTrackerMuon", &lepton1_isTrackerMuon);

   tree->Branch("lepton2_pt",  &lepton2_pt);
   tree->Branch("lepton2_eta", &lepton2_eta);
   tree->Branch("lepton2_phi", &lepton2_phi);
   tree->Branch("lepton2_charge", &lepton2_charge);
   tree->Branch("lepton2_d0",  &lepton2_d0);
   tree->Branch("lepton2_dxy", &lepton2_dxy);
   tree->Branch("lepton2_dz",  &lepton2_dz);
   tree->Branch("lepton2_iso03particle", &lepton2_iso03particle); 
   tree->Branch("lepton2_iso04particle", &lepton2_iso04particle);
   tree->Branch("lepton2_iso03hadron",   &lepton2_iso03hadron);
   tree->Branch("lepton2_iso04hadron",   &lepton2_iso04hadron);
   tree->Branch("lepton2_impactParameterSignificance", &lepton2_impactParameterSignificance);
   tree->Branch("lepton2_isLooseMuon", &lepton2_isLooseMuon);    // these are only for muons 
   tree->Branch("lepton2_isSoftMuon",  &lepton2_isSoftMuon);     // these are only for muons  
   tree->Branch("lepton2_isTightMuon", &lepton2_isTightMuon);    // these are only for muons  
   tree->Branch("lepton2_isPFMuon", &lepton2_isPFMuon);          // these are only for muons  
   tree->Branch("lepton2_isGlobalMuon", &lepton2_isGlobalMuon);
   tree->Branch("lepton2_isTrackerMuon", &lepton2_isTrackerMuon);

   tree->Branch("lepton3_pt",  &lepton3_pt);
   tree->Branch("lepton3_eta", &lepton3_eta);
   tree->Branch("lepton3_phi", &lepton3_phi);
   tree->Branch("lepton3_charge", &lepton3_charge);
   tree->Branch("lepton3_d0",  &lepton3_d0);
   tree->Branch("lepton3_dxy", &lepton3_dxy);
   tree->Branch("lepton3_dz",  &lepton3_dz);
   tree->Branch("lepton3_iso03particle", &lepton3_iso03particle);
   tree->Branch("lepton3_iso04particle", &lepton3_iso04particle);
   tree->Branch("lepton3_iso03hadron", &lepton3_iso03hadron);
   tree->Branch("lepton3_iso04hadron", &lepton3_iso04hadron);
   tree->Branch("lepton3_impactParameterSignificance", &lepton3_impactParameterSignificance);
   tree->Branch("lepton3_isLooseMuon", &lepton3_isLooseMuon);    // these are only for muons 
   tree->Branch("lepton3_isSoftMuon",  &lepton3_isSoftMuon);     // these are only for muons  
   tree->Branch("lepton3_isTightMuon", &lepton3_isTightMuon);    // these are only for muons  
   tree->Branch("lepton3_isPFMuon", &lepton3_isPFMuon);          // these are only for muons  
   tree->Branch("lepton3_isGlobalMuon", &lepton3_isGlobalMuon);
   tree->Branch("lepton3_isTrackerMuon", &lepton3_isTrackerMuon);

   tree->Branch("lepton4_pt",  &lepton4_pt);
   tree->Branch("lepton4_eta", &lepton4_eta);
   tree->Branch("lepton4_phi", &lepton4_phi);
   tree->Branch("lepton4_charge", &lepton4_charge);
   tree->Branch("lepton4_d0",  &lepton4_d0);
   tree->Branch("lepton4_dxy", &lepton4_dxy);
   tree->Branch("lepton4_dz",  &lepton4_dz);
   tree->Branch("lepton4_iso03particle", &lepton4_iso03particle); 
   tree->Branch("lepton4_iso04particle", &lepton4_iso04particle);
   tree->Branch("lepton4_iso03hadron",   &lepton4_iso03hadron);
   tree->Branch("lepton4_iso04hadron",   &lepton4_iso04hadron);
   tree->Branch("lepton4_impactParameterSignificance", &lepton4_impactParameterSignificance);
   tree->Branch("lepton4_isLooseMuon", &lepton4_isLooseMuon);    // these are only for muons 
   tree->Branch("lepton4_isSoftMuon",  &lepton4_isSoftMuon);     // these are only for muons  
   tree->Branch("lepton4_isTightMuon", &lepton4_isTightMuon);    // these are only for muons  
   tree->Branch("lepton4_isPFMuon", &lepton4_isPFMuon);          // these are only for muons  
   tree->Branch("lepton4_isGlobalMuon", &lepton4_isGlobalMuon);
   tree->Branch("lepton4_isTrackerMuon", &lepton4_isTrackerMuon);

   tree->Branch("dimuon1mass", &dimuon1mass);
   tree->Branch("dimuon2mass", &dimuon2mass);
   tree->Branch("dimuon1pt", &dimuon1pt);
   tree->Branch("dimuon2pt", &dimuon2pt);
   tree->Branch("dimuon1eta", &dimuon1eta);
   tree->Branch("dimuon2eta", &dimuon2eta);
   tree->Branch("dimuon1phi", &dimuon1phi);
   tree->Branch("dimuon2phi", &dimuon2phi);

   tree->Branch("dimuon1vtx", &dimuon1vtx);
   tree->Branch("dimuon2vtx", &dimuon2vtx);
   tree->Branch("dimuon1vtx_xpos", &dimuon1vtx_xpos);
   tree->Branch("dimuon2vtx_xpos", &dimuon2vtx_xpos);
   tree->Branch("dimuon1vtx_ypos", &dimuon1vtx_ypos);
   tree->Branch("dimuon2vtx_ypos", &dimuon2vtx_ypos);
   tree->Branch("dimuon1vtx_zpos", &dimuon1vtx_zpos);
   tree->Branch("dimuon2vtx_zpos", &dimuon2vtx_zpos);
   tree->Branch("dimuon1vtx_xposError", &dimuon1vtx_xposError);
   tree->Branch("dimuon2vtx_xposError", &dimuon2vtx_xposError);
   tree->Branch("dimuon1vtx_yposError", &dimuon1vtx_yposError);
   tree->Branch("dimuon2vtx_yposError", &dimuon2vtx_yposError);
   tree->Branch("dimuon1vtx_zposError", &dimuon1vtx_zposError);
   tree->Branch("dimuon2vtx_zposError", &dimuon2vtx_zposError);

   tree->Branch("dimuon1lxy", &dimuon1lxy);
   tree->Branch("dimuon2lxy", &dimuon2lxy);
   tree->Branch("dimuon1lxysig", &dimuon1lxysig);
   tree->Branch("dimuon2lxysig", &dimuon2lxysig);
   tree->Branch("dimuon1lxyctauPV", &dimuon1lxyctauPV);
   tree->Branch("dimuon2lxyctauPV", &dimuon2lxyctauPV);

   tree->Branch("sixmuonvtx", &sixmuonvtx);

   tree->Branch("triggerlist", &triggerlist);

   tree->Branch("numberOfVertices", &numberOfVertices);
   tree->Branch("zOfVertices",    &zOfVertices);
   tree->Branch("zOfVerticesError",    &zOfVerticesError);

   tree->Branch("pair_12_34_56", &pair_12_34_56);
   tree->Branch("pair_13_24_56", &pair_13_24_56);
   tree->Branch("pair_14_23_56", &pair_14_23_56);

   tree->Branch("PVx", &PVx);
   tree->Branch("PVy", &PVy);
   tree->Branch("PVz", &PVz);

   treemc = fs->make<TTree>("treemc", "treemc");
   treemc->Branch("truth_Zmuon_pt",  &truth_Zmuon_pt);
   treemc->Branch("truth_Zmuon_eta", &truth_Zmuon_eta);
   treemc->Branch("truth_Zmuon_phi", &truth_Zmuon_phi);
   treemc->Branch("truth_Z_pt",   &truth_Z_pt);
   treemc->Branch("truth_Z_eta",  &truth_Z_eta);
   treemc->Branch("truth_Z_phi",  &truth_Z_phi);
   treemc->Branch("truth_Z_mass", &truth_Z_mass);
   treemc->Branch("truth_Z_pdgid",&truth_Z_pdgid);
   treemc->Branch("truth_Jpsimuon_pt",  &truth_Jpsimuon_pt);
   treemc->Branch("truth_Jpsimuon_eta", &truth_Jpsimuon_eta);
   treemc->Branch("truth_Jpsimuon_phi", &truth_Jpsimuon_phi);
   treemc->Branch("truth_Jpsi_pt",   &truth_Jpsi_pt);
   treemc->Branch("truth_Jpsi_eta",  &truth_Jpsi_eta);
   treemc->Branch("truth_Jpsi_phi",  &truth_Jpsi_phi);
   treemc->Branch("truth_Jpsi_mass", &truth_Jpsi_mass);
   treemc->Branch("truth_Jpsi_pdgid", &truth_Jpsi_pdgid);

}

void ZmuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonsToken_, muons);

   edm::ESHandle<TransientTrackBuilder> builder;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);

// *******
// TRIGGER
// *******
   edm::Handle<edm::TriggerResults> triggerBits;
   iEvent.getByToken(triggerBits_, triggerBits);
   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

   edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
   iEvent.getByToken(triggerObjects_, triggerObjects);

   edm::Handle<pat::PackedCandidateCollection> pfs;
   iEvent.getByToken(pfToken_, pfs);

//   edm::Handle< pat::TriggerEvent > triggerEvent;
//   iEvent.getByToken( triggerEventToken_, triggerEvent );
//   const pat::helper::TriggerMatchHelper matchHelper;

// **************************************************
// FLAG FOR ASSOCIATED PRODUCTION OR RESONANCE SEARCH
// **************************************************
  // bool mc = true;
  bool mc = false; //testing 2018A SingleMu data
   //will need to add something to do if mc is not true aka how to handle the data

   triggerlist.clear();
   dimuon1mass.clear(); dimuon2mass.clear();
   dimuon1pt.clear(); dimuon2pt.clear();    
   dimuon1eta.clear(); dimuon2eta.clear();  
   dimuon1phi.clear(); dimuon2phi.clear();  
   dimuon1vtx.clear(); dimuon2vtx.clear();  
   dimuon1vtx_xpos.clear(); dimuon2vtx_xpos.clear(); 
   dimuon1vtx_ypos.clear(); dimuon2vtx_ypos.clear(); 
   dimuon1vtx_zpos.clear(); dimuon2vtx_zpos.clear(); 
   dimuon1vtx_xposError.clear(); dimuon2vtx_xposError.clear();
   dimuon1vtx_yposError.clear(); dimuon2vtx_yposError.clear();
   dimuon1vtx_zposError.clear(); dimuon2vtx_zposError.clear();
   sixmuonvtx.clear();

   dimuon1lxy.clear();       dimuon2lxy.clear();      
   dimuon1lxysig.clear();    dimuon2lxysig.clear();   
   dimuon1lxyctauPV.clear(); dimuon2lxyctauPV.clear();

   numberOfVertices.clear();
   zOfVerticesError.clear();
   zOfVertices.clear();

   PVx.clear();
   PVy.clear();
   PVz.clear();

   run_number.clear(); event_number.clear();

   lepton1_pt                         .clear();  lepton2_pt                         .clear();
   lepton1_eta                        .clear();  lepton2_eta                        .clear();
   lepton1_phi                        .clear();  lepton2_phi                        .clear();
   lepton1_charge                     .clear();  lepton2_charge                     .clear();
   lepton1_dxy                        .clear();  lepton2_dxy                        .clear();
   lepton1_dz                         .clear();  lepton2_dz                         .clear();
   lepton1_d0                         .clear();  lepton2_d0                         .clear();
   lepton1_iso03particle              .clear();  lepton2_iso03particle              .clear();
   lepton1_iso03hadron                .clear();  lepton2_iso03hadron                .clear();
   lepton1_iso04hadron                .clear();  lepton2_iso04hadron                .clear();
   lepton1_iso04particle              .clear();  lepton2_iso04particle              .clear();
   lepton1_impactParameterSignificance.clear();  lepton2_impactParameterSignificance.clear();
   lepton1_isLooseMuon                .clear();  lepton2_isLooseMuon                .clear();      
   lepton1_isSoftMuon                 .clear();  lepton2_isSoftMuon                 .clear();        
   lepton1_isTightMuon                .clear();  lepton2_isTightMuon                .clear();
   lepton1_isPFMuon                   .clear();  lepton2_isPFMuon                   .clear();
   lepton1_isGlobalMuon               .clear();  lepton2_isGlobalMuon               .clear();
   lepton1_isTrackerMuon              .clear();  lepton2_isTrackerMuon              .clear();
   lepton1_deltaEta                   .clear();  lepton2_deltaEta                   .clear(); 
   lepton1_deltaPhi                   .clear();  lepton2_deltaPhi                   .clear(); 
   lepton1_sigmaiEtaEta               .clear();  lepton2_sigmaiEtaEta               .clear(); 
   lepton1_HoverE                     .clear();  lepton2_HoverE                     .clear(); 
   lepton1_OoEmOoP                    .clear();  lepton2_OoEmOoP                    .clear(); 
   lepton1_missingHits                .clear();  lepton2_missingHits                .clear(); 

   lepton3_pt                         .clear();  lepton4_pt                         .clear();
   lepton3_eta                        .clear();  lepton4_eta                        .clear();
   lepton3_phi                        .clear();  lepton4_phi                        .clear();
   lepton3_charge                     .clear();  lepton4_charge                     .clear();
   lepton3_dxy                        .clear();  lepton4_dxy                        .clear();
   lepton3_dz                         .clear();  lepton4_dz                         .clear();
   lepton3_d0                         .clear();  lepton4_d0                         .clear();
   lepton3_iso03particle              .clear();  lepton4_iso03particle              .clear();
   lepton3_iso03hadron                .clear();  lepton4_iso03hadron                .clear();
   lepton3_iso04hadron                .clear();  lepton4_iso04hadron                .clear();
   lepton3_iso04particle              .clear();  lepton4_iso04particle              .clear();
   lepton3_impactParameterSignificance.clear();  lepton4_impactParameterSignificance.clear();
   lepton3_isLooseMuon                .clear();  lepton4_isLooseMuon                .clear();      
   lepton3_isSoftMuon                 .clear();  lepton4_isSoftMuon                 .clear();        
   lepton3_isTightMuon                .clear();  lepton4_isTightMuon                .clear();
   lepton3_isPFMuon                   .clear();  lepton4_isPFMuon                   .clear();
   lepton3_isGlobalMuon               .clear();  lepton4_isGlobalMuon               .clear();
   lepton3_isTrackerMuon              .clear();  lepton4_isTrackerMuon              .clear();
   lepton3_deltaEta                   .clear();  lepton4_deltaEta                   .clear(); 
   lepton3_deltaPhi                   .clear();  lepton4_deltaPhi                   .clear(); 
   lepton3_sigmaiEtaEta               .clear();  lepton4_sigmaiEtaEta               .clear(); 
   lepton3_HoverE                     .clear();  lepton4_HoverE                     .clear(); 
   lepton3_OoEmOoP                    .clear();  lepton4_OoEmOoP                    .clear(); 
   lepton3_missingHits                .clear();  lepton4_missingHits                .clear(); 

   pair_12_34_56.clear();    
   pair_13_24_56.clear();    
   pair_14_23_56.clear();

   truth_Zmuon_pt.clear(); 
   truth_Zmuon_eta.clear();
   truth_Zmuon_phi.clear();
   truth_Z_pt.clear(); truth_Z_eta.clear(); truth_Z_phi.clear(); truth_Z_mass.clear(); truth_Z_pdgid.clear();
   truth_Jpsimuon_pt.clear(); 
   truth_Jpsimuon_eta.clear();
   truth_Jpsimuon_phi.clear();
   truth_Jpsi_pt.clear(); truth_Jpsi_eta.clear(); truth_Jpsi_phi.clear(); truth_Jpsi_mass.clear();

   truth_Jpsi_pdgid.clear();  

////   bool nontriggeredevent = true;
//   for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
//    if (triggerBits->accept(i)) {
//      if (names.triggerName(i).find("HLT")<100 && names.triggerName(i).find("Tau")>100 && names.triggerName(i).find("MET")>100 && names.triggerName(i).find("Jet")>100 && names.triggerName(i).find("isplaced")>100 && names.triggerName(i).find("SameSign")>100 && names.triggerName(i).find("Photon")>100) 
//        if (names.triggerName(i).find("Mu")<100 || names.triggerName(i).find("mu")<100) {
//          triggerlist.push_back(names.triggerName(i));
//  //      if (names.triggerName(i).find("Ele")<100 || names.triggerName(i).find("ele")<100)
////          nontriggeredevent = false;
//        }
//    }

   bool keepevent = false; //keepevent defaults to false, which is actually what we want in the end (see line 539)
   for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
    if (triggerBits->accept(i)) {
      triggerlist.push_back(names.triggerName(i));
      std::string str (names.triggerName(i));
 //     std::cout << "str is :" << str << std::endl;
      std::string str2 ("Ele");
      std::string str3 ("Mu");
      std::size_t foundEle = str.find(str2);
      std::size_t foundMu  = str.find(str3);
      if (foundEle!=std::string::npos && foundMu==std::string::npos) //BUG?  //Maybe this is me misunderstanding npos, but I would think this is saying if there is an electron and no muon, keep the event, which doesn't make sense...but looking ahead to line 539, maybe this works out ok...
        keepevent = true;
      if (foundEle!=std::string::npos && foundMu!=std::string::npos) //This potential bug might get worked out down below, because it seems like you want keepevent to be false in order to go forward (see line 539 )
        keepevent = false;
    }
   } //trigger requirements 
//   if (keepevent) {
//      for (int itrig = 0; itrig < (int)triggerlist.size(); itrig++)
//        std::cout << triggerlist.at(itrig) << std::endl;
//      std::cout << "--------------\n";
//   }
// ******
// VERTEX
// ******
   edm::Handle<std::vector<reco::Vertex>> reco_vertices;
   iEvent.getByToken(token_vertices, reco_vertices);

   bool first_vertex = true; //first_vertex defaults to true 
   reco::Vertex primary_vertex;

   numberOfVertices.push_back(reco_vertices->size());
   for (unsigned int vertex=0; vertex < reco_vertices->size(); ++vertex)  { //loop over vertices, do some selections
     if (    // Criteria copied from twiki (mythical)
         !((*reco_vertices)[vertex].isFake()) //vertex must be true 
         && ((*reco_vertices)[vertex].ndof() > 4) //vertex must have ndof >4
         && (fabs((*reco_vertices)[vertex].z()) <= 24.0) //dz of the vertex must be <= 24 cm
         && ((*reco_vertices)[vertex].position().Rho() <= 2.0)//rho of the vertex must be less than or equal to 2 cm 
        ) {
//         reco_vert.num++;
//         reco_vert.x.push_back( (*reco_vertices)[vertex].x() );
//         reco_vert.y.push_back( (*reco_vertices)[vertex].y() );
//         reco_vert.z.push_back( (*reco_vertices)[vertex].z() );
//         // Store first good vertex as "primary"
//         // vertex is ordered by highest pt, in descending order //QUESTION! if by default the first vertex is indeed the one with the highest pT, then the below works. Is this a safe assumption though (I'm not 100% sure miniaod sorts things this way, but you probably know more than I do about this)
//         //std::cout << iEvent.id().event() << "vertex pT: " << sumPtSquared((*reco_vertices)[vertex]) << std::endl;
//         sumOfVertices.push_back(sumPtSquared((*reco_vertices)[vertex]));

          /// <--------- to be tested
          /// <--------- to be tested
          reco::Vertex it_vertex = (*reco_vertices)[vertex]; //if the vertex meets the requirements, go ahead and do this loop
          double sum = 0.;
          double pT;
          for (reco::Vertex::trackRef_iterator it = it_vertex.tracks_begin(); it != it_vertex.tracks_end(); it++) {
            pT = (**it).pt();
            sum += pT; //sum up the pT of all the tracks associated with that vertex
            std::cout << pT << " " << sum << std::endl;
          }
          zOfVerticesError.push_back((*reco_vertices)[vertex].zError());
          zOfVertices.push_back((*reco_vertices)[vertex].z());
          /// <--------- to be tested
          /// <--------- to be tested


         if (first_vertex) { //this ensures that the first vertex that passes the criteria is taken as the primary_vertex, assuming that miniaod does actually order its vertices, this is all good 
           first_vertex = false;
           primary_vertex = (*reco_vertices)[vertex];
           PVx.push_back((*reco_vertices)[vertex].x());
           PVy.push_back((*reco_vertices)[vertex].y());
           PVz.push_back((*reco_vertices)[vertex].z());
         }
     }
   }

   bool doit = false;
   if (iEvent.run() == 319077 || iEvent.run() == 319659 || iEvent.run() == 324205 || iEvent.run() == 322348)
      doit = true;

   // bypass for non-global muons check
   doit = true; //QUESTION! so doit is always true, so what is the point of the code above where doit was contingent on the event belonging to run NNN

   double muon_mass = 0.1056583715; 
   double upsi_mass_low  = 8.; //selections as from AN 
   double upsi_mass_high = 12.; //was 11 in AN but I don't think this really matters so much ()
   double Z_mass_low  = 66.;
   double Z_mass_high = 116.;

// high pt muons - low pt muons
//  if (!nontriggeredevent)
   if ((int)muons->size()>3 && !keepevent && doit) { //Fixed bug, initially we accidentally were demanding 6 mu, now we have it right Shouldn't there be four muons in this final state, so anything with size over 3 should be ok? I think doit is not really doing anything here. And keepevent apparently should be false, so except for the six muon thing, I think this line is ok. Also note that changing this to 3 exposed a weakness/lack of protection in the code in the Lxy part. Sometimes dimuon1vtx_xpos is empty, so needed to add a statement right before the Lxy part that tells the code to continue if dimuon1vtx_xpos is empty to prevent things from blowing up
     bool save_event = false;
     for (auto iM1 = muons->begin(); iM1 != muons->end(); ++iM1) {
      for (auto iM2 = iM1+1; iM2 != muons->end(); ++iM2) {
        for (auto iM3 = iM2+1; iM3 != muons->end(); ++iM3) {
          for (auto iM4 = iM3+1; iM4 != muons->end(); ++iM4) {

                double pT_cut = 2.5; //QUESTION: where did the 2.5 come from? In the AN i see mu with pT > 4...

//                std::cout << " <<<<<<<<<<<<< ------------------------- entered " << std::endl;
    
                if (iM1->charge() + iM2->charge() + iM3->charge() + iM4->charge() !=0)
                  continue;
                if (fabs(iM1->muonBestTrack()->dz(primary_vertex.position()))>20. || fabs(iM2->muonBestTrack()->dz(primary_vertex.position()))>20.)
                  continue;
                if (fabs(iM1->muonBestTrack()->dxy(primary_vertex.position()))>1. || fabs(iM2->muonBestTrack()->dxy(primary_vertex.position()))>1.)
                  continue;
                if (fabs(iM1->eta())>2.5 || fabs(iM2->eta())>2.5)
                  continue;
                if (iM1->pt()<pT_cut || iM2->pt()<pT_cut)
                  continue;
                if (iM1->isTrackerMuon()==false || iM2->isTrackerMuon()==false)
                  continue;
                if (iM1->isSoftMuon(primary_vertex)==false || iM2->isSoftMuon(primary_vertex)==false) //QUESTION!: I wouldn't have known isSoftMuon needed to (primary_vertex) argument, is this something you just knew?
                  continue;
//                 if (iM1->isGlobalMuon()==false || iM2->isGlobalMuon()==false)
//                  continue;
    
                if (fabs(iM3->muonBestTrack()->dz(primary_vertex.position()))>20. || fabs(iM4->muonBestTrack()->dz(primary_vertex.position()))>20.)
                  continue;
                if (fabs(iM3->muonBestTrack()->dxy(primary_vertex.position()))>1. || fabs(iM4->muonBestTrack()->dxy(primary_vertex.position()))>1.)
                  continue;
                if (fabs(iM3->eta())>2.5 || fabs(iM4->eta())>2.5)
                  continue;
                if (iM3->pt()<pT_cut || iM4->pt()<pT_cut)
                  continue;
                if (iM3->isTrackerMuon()==false || iM4->isTrackerMuon()==false)
                  continue;
                if (iM3->isSoftMuon(primary_vertex)==false || iM4->isSoftMuon(primary_vertex)==false)
                  continue;
//                if (iM3->isGlobalMuon()==false || iM4->isGlobalMuon()==false)
//                  continue;
               //QUESTION! How did we come up with the dz = 20 and  dxy = 1 cuts?

                math::PtEtaPhiMLorentzVector lepton1(iM1->pt(), iM1->eta(), iM1->phi(), muon_mass);
                math::PtEtaPhiMLorentzVector lepton2(iM2->pt(), iM2->eta(), iM2->phi(), muon_mass);
                math::PtEtaPhiMLorentzVector lepton3(iM3->pt(), iM3->eta(), iM3->phi(), muon_mass);
                math::PtEtaPhiMLorentzVector lepton4(iM4->pt(), iM4->eta(), iM4->phi(), muon_mass);
    
                double mass_1_2=0.; double mass_1_3=0.; double mass_1_4 = 0.; 
                double mass_2_3=0.; double mass_2_4=0.;  
                double mass_3_4=0.;  
                double pt_1_2=0.;  double pt_1_3=0.;  double pt_1_4 = 0.;  
                double pt_2_3=0.;  double pt_2_4=0.;  
                double pt_3_4=0.; 
                double eta_1_2=0.; double eta_1_3=0.; double eta_1_4 = 0.; 
                double eta_2_3=0.; double eta_2_4=0.; 
                double eta_3_4=0.;
                double phi_1_2=0.; double phi_1_3=0.; double phi_1_4 = 0.; 
                double phi_2_3=0.; double phi_2_4=0.; 
                double phi_3_4=0.; 
                
                //build all the possible pairs of oppositely charged mu
                if (iM1->charge() + iM2->charge() == 0) { mass_1_2 = (lepton1 + lepton2).mass(); pt_1_2 = (lepton1 + lepton2).pt(); eta_1_2 = (lepton1 + lepton2).eta(); eta_1_2 = (lepton1 + lepton2).eta(); phi_1_2 = (lepton1 + lepton2).phi();}
                if (iM1->charge() + iM3->charge() == 0) { mass_1_3 = (lepton1 + lepton3).mass(); pt_1_3 = (lepton1 + lepton3).pt(); eta_1_3 = (lepton1 + lepton3).eta(); eta_1_3 = (lepton1 + lepton3).eta(); phi_1_3 = (lepton1 + lepton3).phi();}
                if (iM1->charge() + iM4->charge() == 0) { mass_1_4 = (lepton1 + lepton4).mass(); pt_1_4 = (lepton1 + lepton4).pt(); eta_1_4 = (lepton1 + lepton4).eta(); eta_1_4 = (lepton1 + lepton4).eta(); phi_1_4 = (lepton1 + lepton4).phi();}
                if (iM2->charge() + iM3->charge() == 0) { mass_2_3 = (lepton2 + lepton3).mass(); pt_2_3 = (lepton2 + lepton3).pt(); eta_2_3 = (lepton2 + lepton3).eta(); eta_2_3 = (lepton2 + lepton3).eta(); phi_2_3 = (lepton2 + lepton3).phi();}
                if (iM2->charge() + iM4->charge() == 0) { mass_2_4 = (lepton2 + lepton4).mass(); pt_2_4 = (lepton2 + lepton4).pt(); eta_2_4 = (lepton2 + lepton4).eta(); eta_2_4 = (lepton2 + lepton4).eta(); phi_2_4 = (lepton2 + lepton4).phi();}
                if (iM3->charge() + iM4->charge() == 0) { mass_3_4 = (lepton3 + lepton4).mass(); pt_3_4 = (lepton3 + lepton4).pt(); eta_3_4 = (lepton3 + lepton4).eta(); eta_3_4 = (lepton3 + lepton4).eta(); phi_3_4 = (lepton3 + lepton4).phi();}


                // ********************

                bool match_12_34_56 = false;

                if (mass_1_2 > upsi_mass_low && mass_1_2 < upsi_mass_high) 
                  if (mass_3_4 > Z_mass_low && mass_3_4 < Z_mass_high) 
                      match_12_34_56 = true;

                if (mass_1_2 > Z_mass_low && mass_1_2 < Z_mass_high) 
                  if (mass_3_4 > upsi_mass_low && mass_3_4 < upsi_mass_high) 
                      match_12_34_56 = true;

                // ********************

                bool match_13_24_56 = false;

                if (mass_1_3 > upsi_mass_low && mass_1_3 < upsi_mass_high) 
                  if (mass_2_4 > Z_mass_low && mass_2_4 < Z_mass_high) 
                      match_13_24_56 = true;

                if (mass_1_3 > Z_mass_low && mass_1_3 < Z_mass_high) 
                  if (mass_2_4 > upsi_mass_low && mass_2_4 < upsi_mass_high) 
                      match_13_24_56 = true;

                // ********************

                bool match_14_23_56 = false;

                if (mass_1_4 > upsi_mass_low && mass_1_4 < upsi_mass_high) 
                  if (mass_2_3 > Z_mass_low && mass_2_3 < Z_mass_high) 
                      match_14_23_56 = true;

                if (mass_1_4 > Z_mass_low && mass_1_4 < Z_mass_high) 
                  if (mass_2_3 > upsi_mass_low && mass_2_3 < upsi_mass_high) 
                      match_14_23_56 = true;


                if (!(match_12_34_56 || match_13_24_56 || 
                      match_14_23_56)) 
                  continue;
                  //QUESTION: do we need some protection in case there was a matching ambiguity?

                save_event = true; //if we found a good set, turn save_event to true 

                if (match_12_34_56) {dimuon1mass.push_back(mass_1_2); dimuon2mass.push_back(mass_3_4);  dimuon1pt.push_back(pt_1_2); dimuon2pt.push_back(pt_3_4);  dimuon1eta.push_back(eta_1_2); dimuon2eta.push_back(eta_3_4);  dimuon1phi.push_back(phi_1_2); dimuon2phi.push_back(phi_3_4); }
                                                                                                                                                                                                                                                                                                                                                                                                                                                
                if (match_13_24_56) {dimuon1mass.push_back(mass_1_3); dimuon2mass.push_back(mass_2_4); dimuon1pt.push_back(pt_1_3); dimuon2pt.push_back(pt_2_4);dimuon1eta.push_back(eta_1_3); dimuon2eta.push_back(eta_2_4); dimuon1phi.push_back(phi_1_3); dimuon2phi.push_back(phi_2_4); }
                                                                                                                                                                                                                                                                                                                    
                if (match_14_23_56) {dimuon1mass.push_back(mass_1_4); dimuon2mass.push_back(mass_2_3); dimuon1pt.push_back(pt_1_4); dimuon2pt.push_back(pt_2_3);dimuon1eta.push_back(eta_1_4); dimuon2eta.push_back(eta_2_3); dimuon1phi.push_back(phi_1_4); dimuon2phi.push_back(phi_2_3); }


//// ****************
//// trigger matching
//// ****************
//            pat::TriggerObjectStandAlone TO;
////            for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
////              std::string name = "HLT_IsoMu18_v5"; //names.triggerName(i);
//            std::string name2 = "HLT_Dimuon0_Jpsi3p5_Muon2_v2";
//            std::string name4 = "HLT_Dimuon0_Jpsi3p5_Muon2_v4";
//            std::string name5 = "HLT_Dimuon0_Jpsi3p5_Muon2_v5";
//            std::string name6 = "HLT_Dimuon0_Jpsi3p5_Muon2_v6";
//            int matched = 0;
//            for (unsigned int iTO = 0, n = triggerObjects->size(); iTO < n; ++iTO) {
//              TO = triggerObjects->at(iTO);
//              TO.unpackPathNames( names );
//              std::cout << "found trigger\n";
//              if (TO.hasPathName(name4)) {
////                double tempDR1 = TMath::Sqrt(TMath::Power(TO.eta() - iM1->eta(),2) + TMath::Power(TO.phi() - iM1->phi(),2));
////                double tempDR2 = TMath::Sqrt(TMath::Power(TO.eta() - iM2->eta(),2) + TMath::Power(TO.phi() - iM2->phi(),2));
////                double tempDR3 = TMath::Sqrt(TMath::Power(TO.eta() - iM3->eta(),2) + TMath::Power(TO.phi() - iM3->phi(),2));
////                double tempDR4 = TMath::Sqrt(TMath::Power(TO.eta() - iM4->eta(),2) + TMath::Power(TO.phi() - iM4->phi(),2));
////                std::cout << iTO << " " << tempDR1 << " " << tempDR2 << " " << tempDR3 << " " << tempDR4 << std::endl;
//                std::cout << TO.pt() << " " << iM1->pt() << " " << iM2->pt() <<  " " << iM3->pt() <<  " " << iM4->pt() << std::endl;
////                if (tempDR1<0.1 || tempDR2<0.1 || tempDR3<0.1 || tempDR4<0.1)
////                  matched++;
//              }
//            }
//            if (matched<2)
//              continue;
////            std::cout << "end of muon match thing: " << matched << std::endl;
////            }

// *********
// VERTEXING 
// *********
                const reco::TrackRef track1 = iM1->muonBestTrack();
                const reco::TrackRef track2 = iM2->muonBestTrack();
                const reco::TrackRef track3 = iM3->muonBestTrack();
                const reco::TrackRef track4 = iM4->muonBestTrack();
                
                //build all the track pairs
                std::vector<reco::TransientTrack> transient_tracks_muon12; transient_tracks_muon12.clear(); 
                std::vector<reco::TransientTrack> transient_tracks_muon13; transient_tracks_muon13.clear(); 
                std::vector<reco::TransientTrack> transient_tracks_muon14; transient_tracks_muon14.clear(); 
                std::vector<reco::TransientTrack> transient_tracks_muon23; transient_tracks_muon23.clear(); 
                std::vector<reco::TransientTrack> transient_tracks_muon24; transient_tracks_muon24.clear(); 
                std::vector<reco::TransientTrack> transient_tracks_muon34; transient_tracks_muon34.clear(); 
                

                transient_tracks_muon12.push_back((*builder).build(track1.get())); transient_tracks_muon12.push_back((*builder).build(track2.get()));
                transient_tracks_muon13.push_back((*builder).build(track1.get())); transient_tracks_muon13.push_back((*builder).build(track3.get()));
                transient_tracks_muon14.push_back((*builder).build(track1.get())); transient_tracks_muon14.push_back((*builder).build(track4.get()));
                transient_tracks_muon23.push_back((*builder).build(track2.get())); transient_tracks_muon23.push_back((*builder).build(track3.get()));
                transient_tracks_muon24.push_back((*builder).build(track2.get())); transient_tracks_muon24.push_back((*builder).build(track4.get()));
                transient_tracks_muon34.push_back((*builder).build(track3.get())); transient_tracks_muon34.push_back((*builder).build(track4.get()));

                KalmanVertexFitter kalman_fitter;
                TransientVertex vtx12; vtx12 = kalman_fitter.vertex(transient_tracks_muon12);
                TransientVertex vtx13; vtx13 = kalman_fitter.vertex(transient_tracks_muon13);
                TransientVertex vtx14; vtx14 = kalman_fitter.vertex(transient_tracks_muon14);
                TransientVertex vtx23; vtx23 = kalman_fitter.vertex(transient_tracks_muon23);
                TransientVertex vtx24; vtx24 = kalman_fitter.vertex(transient_tracks_muon24);
                TransientVertex vtx34; vtx34 = kalman_fitter.vertex(transient_tracks_muon34);
                
                //get vertex info for the pair vertices (assuming they are valid, else push back -1000) //QUESTION: are requirements like the pair having a common vertex with CL > 5% burried in the isValid()
                if (match_12_34_56) {
                  if (vtx12.isValid()) {
                    dimuon1vtx.push_back(TMath::Prob(vtx12.totalChiSquared(), int(vtx12.degreesOfFreedom())));
                    dimuon1vtx_xpos.push_back(vtx12.position().x()); dimuon1vtx_xposError.push_back(vtx12.positionError().cxx());
                    dimuon1vtx_ypos.push_back(vtx12.position().y()); dimuon1vtx_yposError.push_back(vtx12.positionError().cyy());
                    dimuon1vtx_zpos.push_back(vtx12.position().z()); dimuon1vtx_zposError.push_back(vtx12.positionError().czz());
                  } else
                    dimuon1vtx.push_back(-1000.);

                  if (vtx34.isValid()) {
                    dimuon2vtx.push_back(TMath::Prob(vtx34.totalChiSquared(), int(vtx34.degreesOfFreedom())));
                    dimuon2vtx_xpos.push_back(vtx34.position().x()); dimuon2vtx_xposError.push_back(vtx34.positionError().cxx());
                    dimuon2vtx_ypos.push_back(vtx34.position().y()); dimuon2vtx_yposError.push_back(vtx34.positionError().cyy());
                    dimuon2vtx_zpos.push_back(vtx34.position().z()); dimuon2vtx_zposError.push_back(vtx34.positionError().czz());
                  } else
                    dimuon2vtx.push_back(-1000.);

                }
                // <<<<<<<<<-------------------------->>>>>>>>>>>>>
                if (match_13_24_56) {
                  if (vtx13.isValid()) {
                    dimuon1vtx.push_back(TMath::Prob(vtx13.totalChiSquared(), int(vtx13.degreesOfFreedom())));
                    dimuon1vtx_xpos.push_back(vtx13.position().x()); dimuon1vtx_xposError.push_back(vtx13.positionError().cxx());
                    dimuon1vtx_ypos.push_back(vtx13.position().y()); dimuon1vtx_yposError.push_back(vtx13.positionError().cyy());
                    dimuon1vtx_zpos.push_back(vtx13.position().z()); dimuon1vtx_zposError.push_back(vtx13.positionError().czz());
                  } else
                    dimuon1vtx.push_back(-1000.);

                  if (vtx24.isValid()) {
                    dimuon2vtx.push_back(TMath::Prob(vtx24.totalChiSquared(), int(vtx24.degreesOfFreedom())));
                    dimuon2vtx_xpos.push_back(vtx24.position().x()); dimuon2vtx_xposError.push_back(vtx24.positionError().cxx());
                    dimuon2vtx_ypos.push_back(vtx24.position().y()); dimuon2vtx_yposError.push_back(vtx24.positionError().cyy());
                    dimuon2vtx_zpos.push_back(vtx24.position().z()); dimuon2vtx_zposError.push_back(vtx24.positionError().czz());
                  } else
                    dimuon2vtx.push_back(-1000.);

                }
                // <<<<<<<<<-------------------------->>>>>>>>>>>>>
                if (match_14_23_56) {
                  if (vtx14.isValid()) {
                    dimuon1vtx.push_back(TMath::Prob(vtx14.totalChiSquared(), int(vtx14.degreesOfFreedom())));
                    dimuon1vtx_xpos.push_back(vtx14.position().x()); dimuon1vtx_xposError.push_back(vtx14.positionError().cxx());
                    dimuon1vtx_ypos.push_back(vtx14.position().y()); dimuon1vtx_yposError.push_back(vtx14.positionError().cyy());
                    dimuon1vtx_zpos.push_back(vtx14.position().z()); dimuon1vtx_zposError.push_back(vtx14.positionError().czz());
                  } else
                    dimuon1vtx.push_back(-1000.);

                  if (vtx23.isValid()) {
                    dimuon2vtx.push_back(TMath::Prob(vtx23.totalChiSquared(), int(vtx23.degreesOfFreedom())));
                    dimuon2vtx_xpos.push_back(vtx23.position().x()); dimuon2vtx_xposError.push_back(vtx23.positionError().cxx());
                    dimuon2vtx_ypos.push_back(vtx23.position().y()); dimuon2vtx_yposError.push_back(vtx23.positionError().cyy());
                    dimuon2vtx_zpos.push_back(vtx23.position().z()); dimuon2vtx_zposError.push_back(vtx23.positionError().czz());
                  } else
                    dimuon2vtx.push_back(-1000.);

                }
                //protection here in case dimuon1vtx_xpos does not exist. without this protection, code fails with an unhelpful complaint about vector sizes. S.L. taught me with these kinds of errors, check the vectors first and see what is unprotected (like this was)
                //I also put in an analogous protection for diumuon2vtx_xpos
                if (dimuon1vtx_xpos.size()==0 || dimuon2vtx_xpos.size()==0){
                    continue;
                }

                // Lxy part
                // dimuon1lxyctauPV, dimuon1lxy, dimuon1lxysig
                TVector3 pvtx;
                pvtx.SetXYZ(PVx.at(0),PVy.at(0),0);
                TVector3 vtx1, vtx2;
                vtx1.SetXYZ(dimuon1vtx_xpos.at(0),dimuon1vtx_ypos.at(0),0);
                vtx2.SetXYZ(dimuon2vtx_xpos.at(0),dimuon2vtx_ypos.at(0),0);
                TVector3 vdiff1 = vtx1 - pvtx;
                TVector3 vdiff2 = vtx2 - pvtx;
                VertexDistanceXY vdistXY1, vdistXY2; //look up VertexDistanceXY documentation here: https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/plugins/VertexTableProducer.cc, that is for NanoAOD but likely the right idea
                Measurement1D distXY1, distXY2;
                double cosAlphaXY1, cosAlphaXY2;
                if (match_13_24_56) {
                  TVector3 pperp1((lepton1+lepton3).px(), (lepton1+lepton3).py(), 0);
                  TVector3 pperp2((lepton2+lepton4).px(), (lepton2+lepton4).py(), 0);
                  distXY1 = vdistXY1.distance(vtx13.vertexState(), primary_vertex);
                  distXY2 = vdistXY2.distance(vtx24.vertexState(), primary_vertex);
                  cosAlphaXY1 = vdiff1.Dot(pperp1)/(vdiff1.Perp()*pperp1.Perp());
                  cosAlphaXY2 = vdiff2.Dot(pperp2)/(vdiff2.Perp()*pperp2.Perp());
                  dimuon1lxyctauPV.push_back (distXY1.value()*cosAlphaXY1 * (lepton1+lepton3).mass()/pperp1.Perp()); 
                  dimuon2lxyctauPV.push_back (distXY2.value()*cosAlphaXY2 * (lepton2+lepton4).mass()/pperp2.Perp());
                  dimuon1lxy.push_back(distXY1.value()); dimuon1lxysig.push_back(distXY1.value()/distXY1.error());
                  dimuon2lxy.push_back(distXY2.value()); dimuon2lxysig.push_back(distXY2.value()/distXY2.error());

                }
                if (match_14_23_56) {
                  TVector3 pperp1((lepton1+lepton4).px(), (lepton1+lepton4).py(), 0);
                  TVector3 pperp2((lepton2+lepton3).px(), (lepton2+lepton3).py(), 0);
                  distXY1 = vdistXY1.distance(vtx14.vertexState(), primary_vertex);
                  distXY2 = vdistXY2.distance(vtx23.vertexState(), primary_vertex);
                  cosAlphaXY1 = vdiff1.Dot(pperp1)/(vdiff1.Perp()*pperp1.Perp());
                  cosAlphaXY2 = vdiff2.Dot(pperp2)/(vdiff2.Perp()*pperp2.Perp());
                  dimuon1lxyctauPV.push_back (distXY1.value()*cosAlphaXY1 * (lepton1+lepton4).mass()/pperp1.Perp()); 
                  dimuon2lxyctauPV.push_back (distXY2.value()*cosAlphaXY2 * (lepton2+lepton3).mass()/pperp2.Perp());
                  dimuon1lxy.push_back(distXY1.value()); dimuon1lxysig.push_back(distXY1.value()/distXY1.error());
                  dimuon2lxy.push_back(distXY2.value()); dimuon2lxysig.push_back(distXY2.value()/distXY2.error());
                }
                
                if (match_12_34_56) {
                  TVector3 pperp1((lepton1+lepton2).px(), (lepton1+lepton2).py(), 0); 
                  TVector3 pperp2((lepton3+lepton4).px(), (lepton3+lepton4).py(), 0);
                  distXY1 = vdistXY1.distance(vtx12.vertexState(), primary_vertex);
                  distXY2 = vdistXY2.distance(vtx34.vertexState(), primary_vertex);
                  cosAlphaXY1 = vdiff1.Dot(pperp1)/(vdiff1.Perp()*pperp1.Perp());
                  cosAlphaXY2 = vdiff2.Dot(pperp2)/(vdiff2.Perp()*pperp2.Perp());
                  dimuon1lxyctauPV.push_back (distXY1.value()*cosAlphaXY1 * (lepton1+lepton2).mass()/pperp1.Perp()); 
                  dimuon2lxyctauPV.push_back (distXY2.value()*cosAlphaXY2 * (lepton3+lepton4).mass()/pperp2.Perp());
                  dimuon1lxy.push_back(distXY1.value()); dimuon1lxysig.push_back(distXY1.value()/distXY1.error());
                  dimuon2lxy.push_back(distXY2.value()); dimuon2lxysig.push_back(distXY2.value()/distXY2.error());
                }
    
                pair_12_34_56.push_back(match_12_34_56);
                pair_13_24_56.push_back(match_13_24_56);
                pair_14_23_56.push_back(match_14_23_56);

                event_number.push_back(iEvent.id().event());
                run_number.push_back(iEvent.run());
    
                lepton1_pt                         .push_back(iM1->pt());  
                lepton1_eta                        .push_back(iM1->eta());
                lepton1_phi                        .push_back(iM1->phi());
                lepton1_charge                     .push_back(iM1->charge());
                lepton1_dxy                        .push_back(iM1->muonBestTrack()->dxy(primary_vertex.position()));
                lepton1_dz                         .push_back(iM1->muonBestTrack()->dz(primary_vertex.position()));
                lepton1_d0                         .push_back(iM1->muonBestTrack()->d0());
                lepton1_iso03particle              .push_back(iM1->pfIsolationR03().sumChargedParticlePt);
                lepton1_iso03hadron                .push_back(iM1->pfIsolationR03().sumChargedHadronPt);
                lepton1_iso04hadron                .push_back(iM1->pfIsolationR04().sumChargedHadronPt);
                lepton1_iso04particle              .push_back(iM1->pfIsolationR04().sumChargedParticlePt);
                lepton1_impactParameterSignificance.push_back(fabs(iM1->dB(pat::Muon::PV3D)/iM1->edB(pat::Muon::PV3D)));
                lepton1_isLooseMuon                .push_back(iM1->isLooseMuon());
                lepton1_isSoftMuon                 .push_back(iM1->isSoftMuon(primary_vertex));
                lepton1_isTightMuon                .push_back(iM1->isTightMuon(primary_vertex));
                lepton1_isPFMuon                   .push_back(iM1->isPFMuon());
                lepton1_isGlobalMuon               .push_back(iM1->isGlobalMuon());
                lepton1_isTrackerMuon              .push_back(iM1->isTrackerMuon());
            
                lepton2_pt                         .push_back(iM2->pt());  
                lepton2_eta                        .push_back(iM2->eta());
                lepton2_phi                        .push_back(iM2->phi());
                lepton2_charge                     .push_back(iM2->charge());
                lepton2_dxy                        .push_back(iM2->muonBestTrack()->dxy(primary_vertex.position()));
                lepton2_dz                         .push_back(iM2->muonBestTrack()->dz(primary_vertex.position()));
                lepton2_d0                         .push_back(iM2->muonBestTrack()->d0());
                lepton2_iso03particle              .push_back(iM2->pfIsolationR03().sumChargedParticlePt);
                lepton2_iso03hadron                .push_back(iM2->pfIsolationR03().sumChargedHadronPt);
                lepton2_iso04hadron                .push_back(iM2->pfIsolationR04().sumChargedHadronPt);
                lepton2_iso04particle              .push_back(iM2->pfIsolationR04().sumChargedParticlePt);
                lepton2_impactParameterSignificance.push_back(fabs(iM2->dB(pat::Muon::PV3D)/iM2->edB(pat::Muon::PV3D)));
                lepton2_isLooseMuon                .push_back(iM2->isLooseMuon());
                lepton2_isSoftMuon                 .push_back(iM2->isSoftMuon(primary_vertex));
                lepton2_isTightMuon                .push_back(iM2->isTightMuon(primary_vertex));
                lepton2_isPFMuon                   .push_back(iM2->isPFMuon());
                lepton2_isGlobalMuon               .push_back(iM2->isGlobalMuon());
                lepton2_isTrackerMuon              .push_back(iM2->isTrackerMuon());
        
                lepton3_pt                         .push_back(iM3->pt());  
                lepton3_eta                        .push_back(iM3->eta());
                lepton3_phi                        .push_back(iM3->phi());
                lepton3_charge                     .push_back(iM3->charge());
                lepton3_dxy                        .push_back(iM3->muonBestTrack()->dxy(primary_vertex.position()));
                lepton3_dz                         .push_back(iM3->muonBestTrack()->dz(primary_vertex.position()));
                lepton3_d0                         .push_back(iM3->muonBestTrack()->d0());
                lepton3_iso03particle              .push_back(iM3->pfIsolationR03().sumChargedParticlePt);
                lepton3_iso03hadron                .push_back(iM3->pfIsolationR03().sumChargedHadronPt);
                lepton3_iso04hadron                .push_back(iM3->pfIsolationR04().sumChargedHadronPt);
                lepton3_iso04particle              .push_back(iM3->pfIsolationR04().sumChargedParticlePt);
                lepton3_impactParameterSignificance.push_back(fabs(iM3->dB(pat::Muon::PV3D)/iM3->edB(pat::Muon::PV3D)));
                lepton3_isLooseMuon                .push_back(iM3->isLooseMuon());
                lepton3_isSoftMuon                 .push_back(iM3->isSoftMuon(primary_vertex));
                lepton3_isTightMuon                .push_back(iM3->isTightMuon(primary_vertex));
                lepton3_isPFMuon                   .push_back(iM3->isPFMuon());
                lepton3_isGlobalMuon               .push_back(iM3->isGlobalMuon());
                lepton3_isTrackerMuon              .push_back(iM3->isTrackerMuon());
            
                lepton4_pt                         .push_back(iM4->pt());  
                lepton4_eta                        .push_back(iM4->eta());
                lepton4_phi                        .push_back(iM4->phi());
                lepton4_charge                     .push_back(iM4->charge());
                lepton4_dxy                        .push_back(iM4->muonBestTrack()->dxy(primary_vertex.position()));
                lepton4_dz                         .push_back(iM4->muonBestTrack()->dz(primary_vertex.position()));
                lepton4_d0                         .push_back(iM4->muonBestTrack()->d0());
                lepton4_iso03particle              .push_back(iM4->pfIsolationR03().sumChargedParticlePt);
                lepton4_iso03hadron                .push_back(iM4->pfIsolationR03().sumChargedHadronPt);
                lepton4_iso04hadron                .push_back(iM4->pfIsolationR04().sumChargedHadronPt);
                lepton4_iso04particle              .push_back(iM4->pfIsolationR04().sumChargedParticlePt);
                lepton4_impactParameterSignificance.push_back(fabs(iM4->dB(pat::Muon::PV3D)/iM4->edB(pat::Muon::PV3D)));
                lepton4_isLooseMuon                .push_back(iM4->isLooseMuon());
                lepton4_isSoftMuon                 .push_back(iM4->isSoftMuon(primary_vertex));
                lepton4_isTightMuon                .push_back(iM4->isTightMuon(primary_vertex));
                lepton4_isPFMuon                   .push_back(iM4->isPFMuon());
                lepton4_isGlobalMuon               .push_back(iM4->isGlobalMuon());
                lepton4_isTrackerMuon              .push_back(iM4->isTrackerMuon());


          }
        }
       }
     }
     if (save_event)
       tree->Fill();
   }

// **************
// MC starts here
// **************
   if (mc) {
    edm::Handle<reco::GenParticleCollection> mc_particles;
    iEvent.getByToken(genParticlesToken_, mc_particles);

    int UPSI = 553;
    int Z = 23;
    int MUON = 13;

    std::vector<double> upsi_pt_found;
    upsi_pt_found.clear();
    std::vector<double> Z_pt_found;
    Z_pt_found.clear();
    
//    std::cout << "upsi_pt_found.size() initially:  " << upsi_pt_found.size() <<std::endl; //Mary added for testing 
    //QUESTION!: I probably just missed something, but is there a reason you don't fill the upsi_pt_found until line 953 (similar question for Z_pt_found). I guess I'm not sure what this loop 
    //is doing, you said it was a protection against the pythia weirdos (when pythia has a lot of things with similar pT/eta) but I'm not sure I'm understanding what's going on here
    //wouldn't it make more sense to test the truth pT of the mc particle versus the reco pT of the mc particle?
    bool registered_upsi = false;
    bool registered_Z = false;

    for(unsigned int i = 0; i < mc_particles->size(); ++i) {
      const reco::GenParticle* gen_particle = &mc_particles->at(i);
      registered_upsi = false;
      if (gen_particle->pdgId() == UPSI) { 
        for (int iupsi = 0; iupsi < (int)upsi_pt_found.size(); iupsi++) {
          if (TMath::Abs( gen_particle->pt() - upsi_pt_found.at(iupsi) ) < 0.1)
            registered_upsi = true;
//          std::cout << "diff " << (TMath::Abs( gen_particle->pt() - upsi_pt_found.at(iupsi) )) << std::endl; //mary added for testing/understanding this loop
        }
        for (size_t j = 0; j < gen_particle->numberOfDaughters(); ++j) {
          if (TMath::Abs(gen_particle->daughter(j)->pdgId()) == MUON && !registered_upsi) {
            truth_Jpsimuon_pt .push_back(gen_particle->daughter(j)->pt());
            truth_Jpsimuon_eta.push_back(gen_particle->daughter(j)->eta());
            truth_Jpsimuon_phi.push_back(gen_particle->daughter(j)->phi());
            truth_Jpsi_pt  .push_back(gen_particle->pt());
            truth_Jpsi_eta .push_back(gen_particle->eta());
            truth_Jpsi_phi .push_back(gen_particle->phi());
            truth_Jpsi_mass.push_back(gen_particle->mass());
            truth_Jpsi_pdgid.push_back(gen_particle->pdgId());
          }
        }
        upsi_pt_found  .push_back(gen_particle->pt());
//        std::cout << "upsi_pt_found.size() now: " << upsi_pt_found.size() << std::endl; //mary added for testing 
//        for (unsigned int i = 0; i < upsi_pt_found.size(); i++) {
 //       std::cout<< "upsi_pt_found.at(i): " << upsi_pt_found.at(i) << std::endl; 
 //       }
      }
      if (gen_particle->pdgId() == Z) {
        for (int iZ = 0; iZ < (int)Z_pt_found.size(); iZ++) {
          if (TMath::Abs( gen_particle->pt() - Z_pt_found.at(iZ) ) < 0.1)
            registered_Z = true;
        }
        for (size_t j = 0; j < gen_particle->numberOfDaughters(); ++j) {
          if (TMath::Abs(gen_particle->daughter(j)->pdgId()) == MUON && !registered_Z) {
            truth_Zmuon_pt .push_back(gen_particle->daughter(j)->pt());
            truth_Zmuon_eta.push_back(gen_particle->daughter(j)->eta());
            truth_Zmuon_phi.push_back(gen_particle->daughter(j)->phi());
            truth_Z_pt  .push_back(gen_particle->pt());
            truth_Z_eta .push_back(gen_particle->eta());
            truth_Z_phi .push_back(gen_particle->phi());
            truth_Z_mass.push_back(gen_particle->mass());
            truth_Z_pdgid.push_back(gen_particle->pdgId());
          }
        }
        Z_pt_found  .push_back(gen_particle->pt());
      }


    }
    treemc->Fill();
   } // end of mc

}

//define this as a plug-in
DEFINE_FWK_MODULE(ZmuonAnalyzer);

