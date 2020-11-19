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
  TH1F *h_reco_Z_mass_noNewCuts    = new TH1F("h_reco_Z_mass_noNewCuts",    "h_reco_Z_mass_noNewCuts", 20, 66., 116.);  h_reco_Z_mass_noNewCuts   ->SetXTitle("m_{#mu#mu} [GeV]"); //might want to change the binning, this is currently 20 bins to cover a range of 5
  TH1F *h_reco_Upsi_mass_noNewCuts = new TH1F("h_reco_Upsi_mass_noNewCuts", "h_reco_Upsi_mass_noNewCuts", 20, 8., 12.); h_reco_Upsi_mass_noNewCuts->SetXTitle("m_{#mu#mu} [GeV]"); //20 bins here to cover a range of 4 
  
  TH1F *h_big4MuVtxProb_before_big4MuVtx_Prob_Cut = new TH1F("h_big4MuVtxProb_before_big4MuVtx_Prob_Cut", "h_big4MuVtxProb_before_big4MuVtx_Prob_Cut",200, 0, 1); h_big4MuVtxProb_before_big4MuVtx_Prob_Cut->SetXTitle("big4MuVtxProb_before_big4MuVtx_Prob_Cut");
  
  //Ignoring MC for the moment 
  TH1F *h_truth_Z_mass    = new TH1F("h_truth_Z_mass",    "h_truth_Z_mass", 20, 66., 116.);  h_truth_Z_mass->SetMarkerSize(0); //If I change the binning above, would also want to change it here so the truth and recovered plots have same scale 
  TH1F *h_truth_Upsi_mass = new TH1F("h_truth_Upsi_mass", "h_truth_Upsi_mass", 20, 8., 12.); h_truth_Upsi_mass->SetMarkerSize(0); //same comment as above for the upsi truth and recovered mass plots 


  // v a r i a b l e s
  double muon_mass = 105.6583 / 1000.; //get mass in GeV
  
  //counters
  int pair_12_34_56_count = 0;
  
  int pair_13_24_56_count = 0;
  
  int pair_14_23_56_count = 0; 
  
  int pair_AMBIGOUS_muQuad_count = 0;
  
  int big4MuVtx_Prob_Cut_fail_count = 0;
  
  int Z_first_upsi_phase1_second_pair_12_34_56_count = 0;
  
  int upsi_phase1_first_Z_second_pair_12_34_56_count = 0;
  
  int Z_first_upsi_phase1_second_pair_13_24_56_count = 0;
  
  int upsi_phase1_first_Z_second_pair_13_24_56_count = 0;
  
  int Z_first_upsi_phase1_second_pair_14_23_56_count = 0;
  
  int upsi_phase1_first_Z_second_pair_14_23_56_count = 0;
  
  
  //Cuts 
  double big4MuVtx_Prob_Cut = 0.01; 
  
  double Z_mass_low  = 66.;
  
  double Z_mass_high = 116.;
  
  double upsi_mass_low_phase1 = 8;
  
  double upsi_mass_high_phase1 = 12;
  
  double upsi_mass_low_phase2 = 8.5;
  
  double upsi_mass_high_phase2 = 11; 
  
  double lead_mu_from_Z_pT_Cut = 30;
  
  double sublead_mu_from_Z_pT_Cut = 15;
  
  double mu_from_upsi_pT_Cut = 4; 
  
  double mu_from_Z_eta_Cut = 2.4;
  
  double mu_from_upsi_eta_Cut = 2.4;
  
  double mu_from_Z_3DIPSig_Cut = 4;
  
  double mu_from_upsi_RAPIDITY_Cut = 2.4;
  
  //TO DO vertex prob cut for the Z-> mu mu vertex
  
  //To DO vertex prob cut for upsi -> mu mu vertex 
  
  
  
 
  // n e w  s k i m m e d   r o o t   f i l e
  double mass1_quickAndDirty, mass2_quickAndDirty;
  TFile *ntuple = new TFile("ntuple_skimmed_maryTest_19Nov2020.root", "RECREATE");
  TTree *aux;
  aux = new TTree("tree", "tree");
  aux->Branch("mass1_quickAndDirty", &mass1_quickAndDirty);
  aux->Branch("mass2_quickAndDirty", &mass2_quickAndDirty);

///////////////////////////
//////    D A T A    //////
///////////////////////////

  int entries = (TREE->fChain)->GetEntries(); //might want to change to GetEntriesFast by that can wait
  for(int iEntry=0; iEntry<entries; iEntry++) {
    (TREE->fChain)->GetEntry(iEntry);

    double temp_comparison_pt_upsilon = 0;
    double temp_comparison_pt_z = 0;
    mass1_quickAndDirty = 0.; mass2_quickAndDirty = 0.;

    for (int i=0; i<(int)TREE->lepton1_pt->size(); i++) { //note to self: check that lepton1 size will always = lepton 2 size = lepton 3 size = lepton 4 size, I think so but need to double check  //this should be true by definition 
      // lepton1_pt > lepton2_pt > lepton3_pt > lepton4_pt, this is an artifact of how things were done in phase 1 code 
      TLorentzVector lepton1, lepton2, lepton3, lepton4;
      lepton1.SetPtEtaPhiM ( TREE->lepton1_pt->at(i), TREE->lepton1_eta->at(i), TREE->lepton1_phi->at(i), muon_mass);
      lepton2.SetPtEtaPhiM ( TREE->lepton2_pt->at(i), TREE->lepton2_eta->at(i), TREE->lepton2_phi->at(i), muon_mass);
      lepton3.SetPtEtaPhiM ( TREE->lepton3_pt->at(i), TREE->lepton3_eta->at(i), TREE->lepton3_phi->at(i), muon_mass);
      lepton4.SetPtEtaPhiM ( TREE->lepton4_pt->at(i), TREE->lepton4_eta->at(i), TREE->lepton4_phi->at(i), muon_mass);
 
      h_reco_Upsi_mass_noNewCuts->Fill( (lepton3+lepton4).M() );
      h_reco_Z_mass_noNewCuts->Fill( (lepton1+lepton2).M() );
      
      //Cuts involving the overall quad //
      /////////////////////////////////////
      
      //deal with pairing ambiguous muon quads, eliminate those quads from our consideration 
   //    if ( (TREE->pair_12_34_56->at(i) == 1 && TREE->pair_13_24_56->at(i) == 1) || (TREE->pair_12_34_56->at(i) == 1 && TREE->pair_14_23_56->at(i) == 1)
//            || (TREE->pair_13_24_56->at(i) == 1 && TREE->pair_14_23_56->at(i) == 1)
//            || (TREE->pair_12_34_56->at(i) == 1 && TREE->pair_13_24_56->at(i) == 1 && TREE->pair_14_23_56->at(i) == 1) )

              if ( TREE->pair_12_34_56->at(i) + TREE->pair_13_24_56->at(i) + TREE->pair_14_23_56->at(i) > 1) { //cleaner way suggested by S.L., equivalent to what I tried above but shorter!
             std::cout << "FOUND PAIRING AMBIGUOUS QUAD OF MUONS, WILL THROW IT AWAY" << std::endl;
             pair_AMBIGOUS_muQuad_count += 1;
             continue;
        }
      
      h_big4MuVtxProb_before_big4MuVtx_Prob_Cut->Fill(TREE->big4MuVtx->at(i)); //fill it  before we cut on it
        
      if (TREE->big4MuVtx->at(i) > big4MuVtx_Prob_Cut){ //I think the greater than, which is currently here, is correct. Before I had less than, but I think that was wrong. //Earlier comments hereWARNING I THINK THIS CUT WAS INVERTED!!!! WARNING WARNING WARNING CHECK ME!!!!! I think this is essentially returning a p value, and we want low p values, so should be less than the cut 
//         std::cout << "FAILED big4MuVtx_Prob_Cut! Throwing away this quad!" << std::endl; 
//         std::cout << TREE->big4MuVtx->at(i) << std::endl; 
         big4MuVtx_Prob_Cut_fail_count +=1;
         continue;
         }  
      
      //Put in iso003 cuts here, that's the last cut involving the quad TO DO
    
      //end cuts involving overall quad /////////////
      //////////////////////////////////////////////
    
  
     //start pair specific cuts ////
     ///////////////////////////// 
     
     //Note to self: see here: http://www.hep.shef.ac.uk/edaw/PHY206/Site/2012_course_files/phy206rlec7.pdf, equation 6, for rapidity definition (on page 5)

      if (TREE->pair_12_34_56->at(i) ==1){
         
         bool Z_first_upsi_phase1_second_pair_12_34_56 = false;
         bool upsi_phase1_first_Z_second_pair_12_34_56 = false; 
        // std::cout << "TREE->pair_12_34_56->at(i) ==1" << std::endl;
         pair_12_34_56_count += 1;
         
//         std::cout << (lepton1 + lepton2).M()<< std::endl; 
         
         if ( (lepton1 + lepton2).M() > Z_mass_low && (lepton1 + lepton2).M() < Z_mass_high && (lepton3+lepton4).M()  > upsi_mass_low_phase1 && (lepton3+lepton4).M() < upsi_mass_high_phase1){
            if ( (TREE->lepton1_charge->at(i) + TREE->lepton2_charge->at(i) == 0) && (TREE->lepton3_charge->at(i) + TREE->lepton4_charge->at(i) == 0) ) {
              Z_first_upsi_phase1_second_pair_12_34_56 = true;
              Z_first_upsi_phase1_second_pair_12_34_56_count +=1;
              std::cout << "Z_first_upsi_phase1_second_pair_12_34_56 = true!" <<std::endl; 
           }
        }
         
         
         if ( (lepton1 + lepton2).M() > upsi_mass_low_phase1 && (lepton1 + lepton2).M() < upsi_mass_high_phase1 && (lepton3+lepton4).M()  > Z_mass_low && (lepton3+lepton4).M() < Z_mass_high ){
            if ( (TREE->lepton1_charge->at(i) + TREE->lepton2_charge->at(i) == 0) && (TREE->lepton3_charge->at(i) + TREE->lepton4_charge->at(i) == 0) ) {
              upsi_phase1_first_Z_second_pair_12_34_56 = true;
              upsi_phase1_first_Z_second_pair_12_34_56_count +=1;
              std::cout << "upsi_phase1_first_Z_second_pair_12_34_56 is true!" << std::endl; 
            }
         }
 
 ///Mary note to self 19 Nov. 2020: start here when you return!!
           
         if (Z_first_upsi_phase1_second_pair_12_34_56) {
            
            //Start Z cuts 
            if (lepton1.Pt() < lead_mu_from_Z_pT_Cut  || lepton2.Pt() < sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts" << std::endl;
                continue;
             }
            
            if (fabs(lepton1.Eta()) > mu_from_Z_eta_Cut || fabs(lepton2.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
            
            //std::cout << "TREE->lepton1_isTightMuon->at(i): " << TREE->lepton1_isTightMuon->at(i) << std::endl;
         
           if (TREE->lepton1_isTightMuon->at(i) + TREE->lepton2_isTightMuon->at(i) != 2){  //both of them need to be tight, which has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
           
           //vertex probabilit cut that I don't want to deal with me now, FIX ME 
           
           //End Z cuts
           
           //Start upsi cuts
           
         }
         
         
         if  (upsi_phase1_first_Z_second_pair_12_34_56) { 
             
            //start Z cuts  
            if (lepton3.Pt() < lead_mu_from_Z_pT_Cut || lepton4.Pt() <sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts!" << std::endl;
                continue; 
            }
            
            if (fabs(lepton3.Eta()) > mu_from_Z_eta_Cut || fabs(lepton4.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
            
            if (TREE->lepton3_isTightMuon->at(i) + TREE->lepton4_isTightMuon->at(i) != 2){  //both of them need to be tight, which has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
            
            //vertex prob cut that I don't want to deal with now, FIX ME 
            
            //End Z cuts
            
         }
    
      }
      
      if (TREE->pair_13_24_56->at(i) == 1){
         
         bool Z_first_upsi_phase1_second_pair_13_24_56 = false;
         bool upsi_phase1_first_Z_second_pair_13_24_56 = false; 
         //std::cout << "TREE->pair_13_24_56->at(i) == 1" << std::endl; 
         pair_13_24_56_count += 1;
         
         if ( (lepton1 + lepton3).M() > Z_mass_low && (lepton1 + lepton3).M() < Z_mass_high && (lepton2+lepton4).M()  > upsi_mass_low_phase1 && (lepton2+lepton4).M() < upsi_mass_high_phase1){
           if  ( (TREE->lepton1_charge->at(i) + TREE->lepton3_charge->at(i) == 0) && (TREE->lepton2_charge->at(i) + TREE->lepton4_charge->at(i) == 0) ) {
           
            Z_first_upsi_phase1_second_pair_13_24_56 = true;
           Z_first_upsi_phase1_second_pair_13_24_56_count +=1;
           std::cout << "Z_first_upsi_phase1_second_pair_13_24_56 = true!" <<std::endl; 
           
           }
        }
         
         if ( (lepton1 + lepton3).M() > upsi_mass_low_phase1 && (lepton1 + lepton3).M() < upsi_mass_high_phase1 && (lepton2+lepton4).M()  > Z_mass_low && (lepton2+lepton4).M() < Z_mass_high ){
            if  ( (TREE->lepton1_charge->at(i) + TREE->lepton3_charge->at(i) == 0) && (TREE->lepton2_charge->at(i) + TREE->lepton4_charge->at(i) == 0) ) {
              upsi_phase1_first_Z_second_pair_13_24_56 = true;
              upsi_phase1_first_Z_second_pair_13_24_56_count +=1;
              std::cout << "upsi_phase1_first_Z_second_pair_13_24_56 is true!" << std::endl; 
            }
      
         }
    
        
         if (Z_first_upsi_phase1_second_pair_13_24_56){
            //std::cout << "PLACEHOLDER!" << std::endl;      
            if (lepton1.Pt() < lead_mu_from_Z_pT_Cut  || lepton3.Pt() < sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts" << std::endl;
                continue;
             }    
            
            if (fabs(lepton1.Eta()) > mu_from_Z_eta_Cut || fabs(lepton3.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
            
            if (TREE->lepton1_isTightMuon->at(i) + TREE->lepton3_isTightMuon->at(i) != 2){  //both of them need to be tight, tight has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
            
            //vertex prob cuts that I don't want to deal with right now, FIX ME
         
         }
    
         if (upsi_phase1_first_Z_second_pair_13_24_56) {
           // std::cout << "PLACEHOLDER!" << std::endl; 
           
            if (lepton2.Pt() < lead_mu_from_Z_pT_Cut  || lepton4.Pt() < sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts" << std::endl;
                continue;
             }  
            
            if (fabs(lepton2.Eta()) > mu_from_Z_eta_Cut || fabs(lepton4.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
            
            if (TREE->lepton2_isTightMuon->at(i) + TREE->lepton4_isTightMuon->at(i) != 2){  //both of them need to be tight, tight has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
            
            //vertex prob cuts I don't want to deal with right now, FIX ME 
         
         }
    
    }
      
      if (TREE->pair_14_23_56->at(i) == 1){
       //  std::cout << "TREE->pair_14_23_56->at(i) == 1" << std::endl; 
        
         bool Z_first_upsi_phase1_second_pair_14_23_56 = false;
         bool upsi_phase1_first_Z_second_pair_14_23_56 = false; 
         pair_14_23_56_count += 1; 
         
         if ( (lepton1 + lepton4).M() > Z_mass_low && (lepton1 + lepton4).M() < Z_mass_high && (lepton2+lepton3).M()  > upsi_mass_low_phase1 && (lepton2+lepton3).M() < upsi_mass_high_phase1){
            if ( (TREE->lepton1_charge->at(i) + TREE->lepton4_charge->at(i) == 0) && (TREE->lepton2_charge->at(i) + TREE->lepton3_charge->at(i) == 0) ) {
              Z_first_upsi_phase1_second_pair_14_23_56 = true;
              Z_first_upsi_phase1_second_pair_14_23_56_count +=1;
              std::cout << "Z_first_upsi_phase1_second_pair_14_23_56 = true!" <<std::endl; 
             }
          }
         
         if ( (lepton1 + lepton4).M() > upsi_mass_low_phase1 && (lepton1 + lepton4).M() < upsi_mass_high_phase1 && (lepton2+lepton3).M()  > Z_mass_low && (lepton2+lepton3).M() < Z_mass_high ){
            if ( (TREE->lepton1_charge->at(i) + TREE->lepton4_charge->at(i) == 0) && (TREE->lepton2_charge->at(i) + TREE->lepton3_charge->at(i) == 0) ) {
              upsi_phase1_first_Z_second_pair_14_23_56 = true;
              upsi_phase1_first_Z_second_pair_14_23_56_count +=1;
              std::cout << "upsi_phase1_first_Z_second_pair_14_23_56 is true!" << std::endl; 
            }
         }
         
         if (Z_first_upsi_phase1_second_pair_14_23_56){
          //  std::cout << "PLACEHOLDER!" << std::endl; 
            
            if (lepton1.Pt() < lead_mu_from_Z_pT_Cut  || lepton4.Pt() < sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts" << std::endl;
                continue;
             }  
            
            if (fabs(lepton1.Eta()) > mu_from_Z_eta_Cut || fabs(lepton4.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
            
            if (TREE->lepton1_isTightMuon->at(i) + TREE->lepton4_isTightMuon->at(i) != 2){  //both of them need to be tight, tight has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
         
            // vertex prob cuts I don't want to deal with right now FIX ME
         }
         
         if (upsi_phase1_first_Z_second_pair_14_23_56) {
           // std::cout << "PLACEHOLDER" << std::endl; 
             if (lepton2.Pt() < lead_mu_from_Z_pT_Cut  || lepton3.Pt() < sublead_mu_from_Z_pT_Cut){
                std::cout << "FAILED Z mu Pt Cuts" << std::endl;
                continue;
             }  
             
             if (fabs(lepton2.Eta()) > mu_from_Z_eta_Cut || fabs(lepton3.Eta()) > mu_from_Z_eta_Cut){
                std::cout << "FAILED Z mu eta Cuts!" << std::endl; 
                continue;
            }
             
             if (TREE->lepton2_isTightMuon->at(i) + TREE->lepton3_isTightMuon->at(i) != 2){  //both of them need to be tight, tight has a value of 1, 1 +1 =2 
               std::cout << "AT LEAST ONE OF THE MUS FROM A Z WAS NOT TIGHT, FAILED THE Z->MU MU BOTH MU MOST BE TIGHT CUT" << std::endl;
               continue;
           
           } 
             
             //vertex prob I don't want to deal with now FIX ME 
             
         
         }
      }
    
    
 // Checking that we have the right syntax here      
 //     std::cout << "lepton1.Pt(): " << lepton1.Pt() << std::endl;
      
    

    //Deal with this later

      // the lines below pick up the highest in pt candidate but makes
      // the assumption that lepton 1 2 are from Z and lepton 3 4 from
      // the Upsilon. Which can be wrong.
       //QUESTION: how would you suggest we improve on this?
      //QUESTION: unless I'm mistaken here, looks like we still haven't dealt with the matching ambiguous cases, I need to review the phase 1 code and remind myself of how it works though to be sure 
      if (temp_comparison_pt_upsilon < (lepton3+lepton4).Pt()) {
        mass1_quickAndDirty = (lepton3+lepton4).M();
        temp_comparison_pt_upsilon = (lepton3+lepton4).Pt();
      }
      if (temp_comparison_pt_z < (lepton1+lepton2).Pt()) {
        mass2_quickAndDirty = (lepton1+lepton2).M();
        temp_comparison_pt_z = (lepton1+lepton2).Pt();
      }

    } // loop over the size of the leptons

    if (mass1_quickAndDirty > 0. && mass2_quickAndDirty > 0.) //assuming we have found good candidates for both mass1 (upsi) and mass2 (Z), book them. The "good" candidates are taken to be the highest pT ones 
      aux->Fill();
    
    
    
  } // loop over the entries


std::cout << "pair_12_34_56_count: " << pair_12_34_56_count << std::endl; 
std::cout << "pair_13_24_56_count: " << pair_13_24_56_count << std::endl;
std::cout << "pair_14_23_56_count: " << pair_14_23_56_count << std::endl; 
std::cout << "pair_AMBIGOUS_muQuad_count: " << pair_AMBIGOUS_muQuad_count << std::endl;
std::cout << "big4MuVtx_Prob_Cut_fail_count: " << big4MuVtx_Prob_Cut_fail_count << std::endl; 
std::cout << "Z_first_upsi_phase1_second_pair_12_34_56_count: " << Z_first_upsi_phase1_second_pair_12_34_56_count << std::endl;
std::cout << "upsi_phase1_first_Z_second_pair_12_34_56_count: " << upsi_phase1_first_Z_second_pair_12_34_56_count << std::endl; 
std::cout << "Z_first_upsi_phase1_second_pair_13_24_56_count: " << Z_first_upsi_phase1_second_pair_13_24_56_count << std::endl;
std::cout << "upsi_phase1_first_Z_second_pair_13_24_56_count: " << upsi_phase1_first_Z_second_pair_13_24_56_count << std::endl; 
std::cout << "Z_first_upsi_phase1_second_pair_14_23_56_count: " << Z_first_upsi_phase1_second_pair_14_23_56_count << std::endl;
std::cout << "upsi_phase1_first_Z_second_pair_14_23_56_count: " << upsi_phase1_first_Z_second_pair_14_23_56_count << std::endl; 




///////////////////////
//////    M C    //////
///////////////////////

  // double plot_normalization_Z = 0.;
//   double plot_normalization_Upsi = 0.;
//   int entriesMC = (TREEMC->fChain)->GetEntries();
//   for(int iEntry=0; iEntry<entriesMC; iEntry++) {
//     (TREEMC->fChain)->GetEntry(iEntry);
// 
//     for (int i=0; i<(int)TREEMC->truth_Z_mass->size(); i++) {
//       h_truth_Z_mass->Fill(TREEMC->truth_Z_mass->at(i));
//       plot_normalization_Z++;
//     }
// 
//     for (int i=0; i<(int)TREEMC->truth_Jpsi_mass->size(); i++) {
//       h_truth_Upsi_mass->Fill(TREEMC->truth_Jpsi_mass->at(i));
//       plot_normalization_Upsi++;
//     }
// 
//   }
// 
//   h_truth_Z_mass->Scale(h_reco_Z_mass->GetEntries() / plot_normalization_Z);
//   h_truth_Upsi_mass->Scale(h_reco_Upsi_mass->GetEntries() / plot_normalization_Upsi);

/////////////////////////////////////////////////////////
////////////////     P L O T T I N G     ////////////////
/////////////////////////////////////////////////////////

  TCanvas *c_masses = new TCanvas("c_masses", "c_masses", 1000, 500); c_masses->Divide(2,1);
  c_masses->cd(1); h_reco_Upsi_mass_noNewCuts->Draw("e1"); //h_truth_Upsi_mass->Draw("hesame"); //ignoring MC for the moment
  c_masses->cd(2); h_reco_Z_mass_noNewCuts->Draw("e1"); //h_truth_Z_mass->Draw("hesame"); //ignoring MC for the moment 
  h_reco_Upsi_mass_noNewCuts->Write();
  h_reco_Z_mass_noNewCuts->Write();
  c_masses->SaveAs("c_masses.pdf"); //want to save the canvas here because we split it and made it look nice 
  
  
  TCanvas *c_big4MuVtxProb_before_big4MuVtx_Prob_Cut = new TCanvas("c_big4MuVtxProb_before_big4MuVtx_Prob_Cut","c_big4MuVtxProb_before_big4MuVtx_Prob_Cut"); //last 2 are width and height
  c_big4MuVtxProb_before_big4MuVtx_Prob_Cut->cd(); h_big4MuVtxProb_before_big4MuVtx_Prob_Cut->Draw();
  h_big4MuVtxProb_before_big4MuVtx_Prob_Cut->Write();
  h_big4MuVtxProb_before_big4MuVtx_Prob_Cut->SaveAs("h_big4MuVtxProb_before_big4MuVtx_Prob_Cut.pdf");

  ntuple->Write();
  ntuple->Close();

}
