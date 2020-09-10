// -*- C++ -*-
//
// Package:    filters/ZmuonFilter
// Class:      ZmuonFilter
// 
/**\class ZmuonFilter ZmuonFilter.cc filters/ZmuonFilter/plugins/ZmuonFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mary Hill Hadley
//         Created:  Tue, 08 Sep 2020 18:57:22 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

//new includes  //need to specify these in the BuildFile.xml in the plugins directory, thanks to Kevin Pedro for explaining this
//By these I mean things like PhysicsTools, DataFormats, then what you use specifically you specify here in the .cc
//so for example in my BuildFile.xml I have: <use name="PhysicsTools/UtilAlgos"/> and then here I have stuff that comes out of PhysicsTools
//Ask John to elaborate on this because I have for instance DataFormats/VertexReco in my BuildFile.xml but then here I actually use DataFormats/PatCandidates and it doesn't complain...
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"


//
// class declaration
//

class ZmuonFilter : public edm::stream::EDFilter<> {
   public:
      explicit ZmuonFilter(const edm::ParameterSet&);
      ~ZmuonFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
//I might just want to kill off the fillDescriptions
//Do I want to kill off beginStream and endStream? I might just leave them, unlike fillDescriptions I don't think leaving them will mess anything up

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
      
      edm::EDGetTokenT<std::vector<pat::Muon>> muonsToken_;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ZmuonFilter::ZmuonFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
//put needed tokens here
// define eta cuts, 4 object invariant mass cut, pT cut (if needed), triggercuts
// it looks like, based on Frank's example here: https://github.com/fojensen/ExcitedTau/blob/master/excitingAnalyzer_cfg.py#L137-L143, that this is how to add it to the cfg

muonsToken_        = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection"));
//triggerBits_       = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
//triggerObjects_ = consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"));

//in the analyzer, we put these tokens in the analyze function as well as in the class definition like I have here, but based on Frank's example, I don't need to include then up above here, maybe because filter is a bool? Or maybe it's just not strictly necessary 

}


ZmuonFilter::~ZmuonFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
ZmuonFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{//I think this is the one I want and I can kill of the EVENTSETUP_EXAMPLE
   using namespace edm;
//since using namespace edm probably don't need to write edm before Handle below, leaving it for now
 //Trigger
//   edm::Handle<edm::TriggerResults> triggerBits;
//   iEvent.getByToken(triggerBits_, triggerBits);
//   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
//   edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
//   iEvent.getByToken(triggerObjects_, triggerObjects);
   

   //write trigger bool
   //
   //
   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonsToken_, muons);

    //write muon bool
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
ZmuonFilter::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
ZmuonFilter::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
ZmuonFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ZmuonFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ZmuonFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ZmuonFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZmuonFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
//I might just kill this guy off
}
//define this as a plug-in
DEFINE_FWK_MODULE(ZmuonFilter);
