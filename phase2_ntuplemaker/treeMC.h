//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep  2 07:09:37 2020 by ROOT version 5.34/11
// from TTree treemc/treemc
// found on file: mc_ZUpsi.root
//////////////////////////////////////////////////////////

#ifndef treeMC_h
#define treeMC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class treeMC {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<double>  *truth_Zmuon_pt;
   vector<double>  *truth_Zmuon_eta;
   vector<double>  *truth_Zmuon_phi;
   vector<double>  *truth_Z_pt;
   vector<double>  *truth_Z_eta;
   vector<double>  *truth_Z_phi;
   vector<double>  *truth_Z_mass;
   vector<double>  *truth_Z_pdgid;
   vector<double>  *truth_Jpsimuon_pt;
   vector<double>  *truth_Jpsimuon_eta;
   vector<double>  *truth_Jpsimuon_phi;
   vector<double>  *truth_Jpsi_pt;
   vector<double>  *truth_Jpsi_eta;
   vector<double>  *truth_Jpsi_phi;
   vector<double>  *truth_Jpsi_mass;
   vector<double>  *truth_Jpsi_pdgid;

   // List of branches
   TBranch        *b_truth_Zmuon_pt;   //!
   TBranch        *b_truth_Zmuon_eta;   //!
   TBranch        *b_truth_Zmuon_phi;   //!
   TBranch        *b_truth_Z_pt;   //!
   TBranch        *b_truth_Z_eta;   //!
   TBranch        *b_truth_Z_phi;   //!
   TBranch        *b_truth_Z_mass;   //!
   TBranch        *b_truth_Z_pdgid;   //!
   TBranch        *b_truth_Jpsimuon_pt;   //!
   TBranch        *b_truth_Jpsimuon_eta;   //!
   TBranch        *b_truth_Jpsimuon_phi;   //!
   TBranch        *b_truth_Jpsi_pt;   //!
   TBranch        *b_truth_Jpsi_eta;   //!
   TBranch        *b_truth_Jpsi_phi;   //!
   TBranch        *b_truth_Jpsi_mass;   //!
   TBranch        *b_truth_Jpsi_pdgid;   //!

   treeMC(TTree *tree=0);
   virtual ~treeMC();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef treeMC_cxx
treeMC::treeMC(TTree *tree) : fChain(0) 
{
   if (tree == 0) {
      TFile *f = root_file;
      if (!f || !f->IsOpen()) {
         f = root_file;
      }
      TDirectory * dir = (TDirectory*)f->Get("ZmuonAnalyzer");
      dir->GetObject("treemc",tree);
   }
   Init(tree);
}

treeMC::~treeMC()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t treeMC::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t treeMC::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void treeMC::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   truth_Zmuon_pt = 0;
   truth_Zmuon_eta = 0;
   truth_Zmuon_phi = 0;
   truth_Z_pt = 0;
   truth_Z_eta = 0;
   truth_Z_phi = 0;
   truth_Z_mass = 0;
   truth_Z_pdgid = 0;
   truth_Jpsimuon_pt = 0;
   truth_Jpsimuon_eta = 0;
   truth_Jpsimuon_phi = 0;
   truth_Jpsi_pt = 0;
   truth_Jpsi_eta = 0;
   truth_Jpsi_phi = 0;
   truth_Jpsi_mass = 0;
   truth_Jpsi_pdgid = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("truth_Zmuon_pt", &truth_Zmuon_pt, &b_truth_Zmuon_pt);
   fChain->SetBranchAddress("truth_Zmuon_eta", &truth_Zmuon_eta, &b_truth_Zmuon_eta);
   fChain->SetBranchAddress("truth_Zmuon_phi", &truth_Zmuon_phi, &b_truth_Zmuon_phi);
   fChain->SetBranchAddress("truth_Z_pt", &truth_Z_pt, &b_truth_Z_pt);
   fChain->SetBranchAddress("truth_Z_eta", &truth_Z_eta, &b_truth_Z_eta);
   fChain->SetBranchAddress("truth_Z_phi", &truth_Z_phi, &b_truth_Z_phi);
   fChain->SetBranchAddress("truth_Z_mass", &truth_Z_mass, &b_truth_Z_mass);
   fChain->SetBranchAddress("truth_Z_pdgid", &truth_Z_pdgid, &b_truth_Z_pdgid);
   fChain->SetBranchAddress("truth_Jpsimuon_pt", &truth_Jpsimuon_pt, &b_truth_Jpsimuon_pt);
   fChain->SetBranchAddress("truth_Jpsimuon_eta", &truth_Jpsimuon_eta, &b_truth_Jpsimuon_eta);
   fChain->SetBranchAddress("truth_Jpsimuon_phi", &truth_Jpsimuon_phi, &b_truth_Jpsimuon_phi);
   fChain->SetBranchAddress("truth_Jpsi_pt", &truth_Jpsi_pt, &b_truth_Jpsi_pt);
   fChain->SetBranchAddress("truth_Jpsi_eta", &truth_Jpsi_eta, &b_truth_Jpsi_eta);
   fChain->SetBranchAddress("truth_Jpsi_phi", &truth_Jpsi_phi, &b_truth_Jpsi_phi);
   fChain->SetBranchAddress("truth_Jpsi_mass", &truth_Jpsi_mass, &b_truth_Jpsi_mass);
   fChain->SetBranchAddress("truth_Jpsi_pdgid", &truth_Jpsi_pdgid, &b_truth_Jpsi_pdgid);
   Notify();
}

Bool_t treeMC::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void treeMC::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t treeMC::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef treeMC_cxx
