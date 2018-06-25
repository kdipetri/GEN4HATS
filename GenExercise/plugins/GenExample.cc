// -*- C++ -*-
//
// Package:    GenExample/GenExample
// Class:      GenExample
// 
/**\class GenExample GenExample.cc GenExample/GenExample/plugins/GenExample.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Saptaparna Bhattacharya
//         Created:  Sun, 24 Jun 2018 23:08:04 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "RecoEgamma/Examples/plugins/GsfElectronMCAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class GenExample : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit GenExample(const edm::ParameterSet&);
      ~GenExample();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      edm::EDGetTokenT<std::vector<reco::GenParticle> > srcGenParticles_;
      TBranch *branch_;
      TTree *tree_;
      Int_t run_,event_,lumi_;
      std::vector<float> genParticle_Pt;
      std::vector<float> genParticle_Eta;
      std::vector<float> genParticle_Phi;
      std::vector<float> genParticle_Energy;
      std::vector<int> genParticle_pdgId;
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
GenExample::GenExample(const edm::ParameterSet& iConfig)
: srcGenParticles_ (consumes<std::vector<reco::GenParticle> >(iConfig.getParameter<edm::InputTag> ("srcGenParticles")))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

}


GenExample::~GenExample()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   run_   = iEvent.id().run();
   event_ = iEvent.id().event();
   lumi_  = iEvent.luminosityBlock();
   genParticle_Pt.clear();
   genParticle_Eta.clear();
   genParticle_Phi.clear();
   genParticle_Energy.clear();
   genParticle_pdgId.clear();

   Handle<std::vector<reco::GenParticle> > genParticles;
   iEvent.getByToken(srcGenParticles_, genParticles);

   for(size_t i = 0; i < genParticles->size(); ++ i)
   {
     const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>((*genParticles)[i]);
     if(p.status()>=21 and p.status()<=30)
     {
       genParticle_Pt.push_back(p.pt());
       genParticle_Eta.push_back(p.eta());
       genParticle_Phi.push_back(p.phi());
       genParticle_Energy.push_back(p.energy());
       genParticle_pdgId.push_back(p.pdgId());
     }
   }
   tree_->Fill(); 
}

// ------------ method called once each job just before starting event loop  ------------
void 
GenExample::beginJob()
{
  edm::Service<TFileService> fs;
  tree_=fs->make<TTree>("GenInfo","GenInfo");
  branch_=tree_->Branch("run",   &run_,   "run/I");
  branch_=tree_->Branch("event", &event_, "event/I");
  branch_=tree_->Branch("lumi",  &lumi_,  "lumi/I");
  branch_=tree_->Branch("genParticle_Pt", &genParticle_Pt);
  branch_=tree_->Branch("genParticle_Phi", &genParticle_Phi);
  branch_=tree_->Branch("genParticle_Eta", &genParticle_Eta);
  branch_=tree_->Branch("genParticle_Energy", &genParticle_Energy);
  branch_=tree_->Branch("genParticle_pdgId", &genParticle_pdgId);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenExample::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenExample::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenExample);
