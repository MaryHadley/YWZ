// h o w   t o   r u n
// root -l
// .L loop.C++
// run("mc_ZUpsi.root") 

#include <vector>
#include <iostream>
#include <string>
using namespace std;
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLorentzVector.h"

TFile *root_file;
#include "tree.C"
#include "treeMC.C"

tree *TREE;
treeMC *TREEMC;

void run(string file){//, string file2){

  // l o a d   t h e   t r e e s 
  root_file = new TFile(file.c_str(),"READ");
  TREE   = new tree((TTree*)root_file->Get("tree"));
  TREEMC = new treeMC((TTree*)root_file->Get("treemc"));


  // h i s t o g r a m s
  TH1F *h_reco_Z_mass    = new TH1F("h_reco_Z_mass",    "h_reco_Z_mass", 20, 66., 116.);  h_reco_Z_mass   ->SetXTitle("m_{#mu#mu} [GeV]"); 
  TH1F *h_reco_Upsi_mass = new TH1F("h_reco_Upsi_mass", "h_reco_Upsi_mass", 20, 8., 12.); h_reco_Upsi_mass->SetXTitle("m_{#mu#mu} [GeV]");

  TH1F *h_truth_Z_mass    = new TH1F("h_truth_Z_mass",    "h_truth_Z_mass", 20, 66., 116.);  h_truth_Z_mass->SetMarkerSize(0);
  TH1F *h_truth_Upsi_mass = new TH1F("h_truth_Upsi_mass", "h_truth_Upsi_mass", 20, 8., 12.); h_truth_Upsi_mass->SetMarkerSize(0);


  // v a r i a b l e s
  double muon_mass = 105.6583 / 1000.;

  // n e w  s k i m m e d   r o o t   f i l e
  double mass1, mass2;
  TFile *ntuple = new TFile("ntuple_skimmed.root", "RECREATE");
  TTree *aux;
  aux = new TTree("tree", "tree");
  aux->Branch("mass1", &mass1);
  aux->Branch("mass2", &mass2);

///////////////////////////
//////    D A T A    //////
///////////////////////////

  int entries = (TREE->fChain)->GetEntries();
  for(int iEntry=0; iEntry<entries; iEntry++) {
    (TREE->fChain)->GetEntry(iEntry);

    double temp_comparison_pt_upsilon = 0;
    double temp_comparison_pt_z = 0;
    mass1 = 0.; mass2 = 0.;

    for (int i=0; i<(int)TREE->lepton1_pt->size(); i++) {

      TLorentzVector lepton1, lepton2, lepton3, lepton4;
      lepton1.SetPtEtaPhiM ( TREE->lepton1_pt->at(i), TREE->lepton1_eta->at(i), TREE->lepton1_phi->at(i), muon_mass);
      lepton2.SetPtEtaPhiM ( TREE->lepton2_pt->at(i), TREE->lepton2_eta->at(i), TREE->lepton2_phi->at(i), muon_mass);
      lepton3.SetPtEtaPhiM ( TREE->lepton3_pt->at(i), TREE->lepton3_eta->at(i), TREE->lepton3_phi->at(i), muon_mass);
      lepton4.SetPtEtaPhiM ( TREE->lepton4_pt->at(i), TREE->lepton4_eta->at(i), TREE->lepton4_phi->at(i), muon_mass);

      h_reco_Upsi_mass->Fill( (lepton3+lepton4).M() );
      h_reco_Z_mass   ->Fill( (lepton1+lepton2).M() );

      // the lines below pick up the highest in pt candidate but makes
      // the assumption that lepton 1 2 are from Z and lepton 3 4 from
      // the Upsilon. Which can be wrong.
      if (temp_comparison_pt_upsilon < (lepton3+lepton4).Pt()) {
        mass1 = (lepton3+lepton4).M();
        temp_comparison_pt_upsilon = (lepton3+lepton4).Pt();
      }
      if (temp_comparison_pt_z < (lepton1+lepton2).Pt()) {
        mass2 = (lepton1+lepton2).M();
        temp_comparison_pt_z = (lepton1+lepton2).Pt();
      }

    } // loop over the size of the leptons

    if (mass1 > 0. && mass2 > 0.)
      aux->Fill();

  } // loop over the entries


///////////////////////
//////    M C    //////
///////////////////////

  double plot_normalization_Z = 0.;
  double plot_normalization_Upsi = 0.;
  int entriesMC = (TREEMC->fChain)->GetEntries();
  for(int iEntry=0; iEntry<entriesMC; iEntry++) {
    (TREEMC->fChain)->GetEntry(iEntry);

    for (int i=0; i<(int)TREEMC->truth_Z_mass->size(); i++) {
      h_truth_Z_mass->Fill(TREEMC->truth_Z_mass->at(i));
      plot_normalization_Z++;
    }

    for (int i=0; i<(int)TREEMC->truth_Jpsi_mass->size(); i++) {
      h_truth_Upsi_mass->Fill(TREEMC->truth_Jpsi_mass->at(i));
      plot_normalization_Upsi++;
    }

  }

  h_truth_Z_mass->Scale(h_reco_Z_mass->GetEntries() / plot_normalization_Z);
  h_truth_Upsi_mass->Scale(h_reco_Upsi_mass->GetEntries() / plot_normalization_Upsi);

/////////////////////////////////////////////////////////
////////////////     P L O T T I N G     ////////////////
/////////////////////////////////////////////////////////

  TCanvas *c_masses = new TCanvas("c_masses", "c_masses", 1000, 500); c_masses->Divide(2,1);
  c_masses->cd(1); h_reco_Upsi_mass->Draw("e1"); h_truth_Upsi_mass->Draw("hesame");
  c_masses->cd(2); h_reco_Z_mass->Draw("e1"); h_truth_Z_mass->Draw("hesame");

  ntuple->Write();
  ntuple->Close();

}
